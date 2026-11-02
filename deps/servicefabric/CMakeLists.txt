#Copyright (C) Microsoft Corporation. All rights reserved.

# Service Fabric native libs/headers from NuGet. See readme.md.

set(SF_NUGET_PKG_VERSION "11.9.121-internal")

if (CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|ARM64|aarch64|AARCH64)$")
    set(SF_NUGET_PKG_ID "ServiceFabric.NativeLibsHeaders.retail.win-arm64")
else()
    set(SF_NUGET_PKG_ID "ServiceFabric.NativeLibsHeaders.retail")
endif()

set(SF_PACKAGES_DIR "${CMAKE_BINARY_DIR}/sf_packages")
file(MAKE_DIRECTORY "${SF_PACKAGES_DIR}")

find_program(NUGET_EXE NAMES nuget nuget.exe)
if (NOT NUGET_EXE)
    message(FATAL_ERROR "nuget.exe not found on PATH.")
endif()

set(SF_PACKAGES_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/packages.config")

# nuget.config used for the Service Fabric NuGet restore. Defaults to
# sf-c-util's own (so standalone builds work without setup). A parent
# project that wants its own to win can override before
# add_subdirectory(deps/sf-c-util):
#   set(SF_NUGET_CONFIG "${CMAKE_SOURCE_DIR}/nuget.config" CACHE FILEPATH "" FORCE)
set(SF_NUGET_CONFIG "${CMAKE_CURRENT_LIST_DIR}/../../nuget.config"
    CACHE FILEPATH "nuget.config used for the ServiceFabric NuGet restore")

if (NOT EXISTS "${SF_NUGET_CONFIG}")
    message(FATAL_ERROR "SF_NUGET_CONFIG points to a non-existent file: ${SF_NUGET_CONFIG}")
endif()

message(STATUS "Restoring Service Fabric NuGet packages from ${SF_PACKAGES_CONFIG} (using ${SF_NUGET_CONFIG})")

execute_process(
    COMMAND ${NUGET_EXE} restore ${SF_PACKAGES_CONFIG}
            -PackagesDirectory ${SF_PACKAGES_DIR}
            -ConfigFile ${SF_NUGET_CONFIG}
            -NonInteractive
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE _sf_restore_result
    OUTPUT_VARIABLE _sf_restore_output
    ERROR_VARIABLE _sf_restore_error)

if (NOT _sf_restore_result EQUAL 0)
    message(FATAL_ERROR
        "Service Fabric NuGet restore failed (exit ${_sf_restore_result}):\n"
        "stdout:\n${_sf_restore_output}\n"
        "stderr:\n${_sf_restore_error}")
endif()

set(SF_PKG_ROOT "${SF_PACKAGES_DIR}/${SF_NUGET_PKG_ID}.${SF_NUGET_PKG_VERSION}")

if (NOT EXISTS "${SF_PKG_ROOT}/lib/native/FabricClient.lib")
    message(FATAL_ERROR
        "Expected Service Fabric package was not restored at ${SF_PKG_ROOT}. "
        "Check packages.config and the ServiceFabric feed authentication.")
endif()

set(SF_INC_DIRS
    "${SF_PKG_ROOT}/content/native/prebuilt/sdk"
    "${SF_PKG_ROOT}/content/native/prebuilt/internal")

add_library(FabricClient STATIC IMPORTED GLOBAL)
set_target_properties(FabricClient PROPERTIES
    IMPORTED_LOCATION "${SF_PKG_ROOT}/lib/native/FabricClient.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SF_INC_DIRS}")

add_library(FabricRuntime STATIC IMPORTED GLOBAL)
set_target_properties(FabricRuntime PROPERTIES
    IMPORTED_LOCATION "${SF_PKG_ROOT}/lib/native/FabricRuntime.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SF_INC_DIRS}")

add_library(FabricUUID STATIC IMPORTED GLOBAL)
set_target_properties(FabricUUID PROPERTIES
    IMPORTED_LOCATION "${SF_PKG_ROOT}/lib/native/FabricUUID.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SF_INC_DIRS}"
    # Starting with 11.9.121-internal (x64 and win-arm64) the objects in
    # FabricUUID.lib and InternalFabricUUID.lib are compiled /Zi and point
    # to *.StaticLibrary.compile.pdb, which no SF NuGet ships (11.8.121 and
    # 12.0.997-internal-beta are /Z7 with embedded debug info). Linking with
    # /DEBUG then raises LNK4099, which /WX promotes to LNK1218. The libs only
    # hold GUID/IID data, so linking them without debug info loses nothing.
    # Recheck the size of .debug$T with "dumpbin /headers" on each SF version
    # bump; drop /IGNORE:4099 once both libs are /Z7 again.
    INTERFACE_LINK_OPTIONS "/IGNORE:4099")

add_library(FabricUUIDInternal STATIC IMPORTED GLOBAL)
set_target_properties(FabricUUIDInternal PROPERTIES
    IMPORTED_LOCATION "${SF_PKG_ROOT}/lib/native/InternalFabricUUID.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SF_INC_DIRS}"
    # /IGNORE:4099: same reason as on FabricUUID above.
    # sf-c-util links the retail SF libs in every configuration (no debug
    # SF libs are used, see readme.md). InternalFabricUUID.lib was compiled
    # /MD against the release dynamic CRT and embeds a /DEFAULTLIB:MSVCRT
    # directive (still present in 11.9.121-internal, x64 and win-arm64).
    # When a consumer is built /MDd (Debug) the linker sees both MSVCRT and
    # MSVCRTD requested, raises LNK4098, and any consumer with /WX promotes
    # that to LNK1218. Tell the linker to ignore MSVCRT default-lib requests
    # in Debug; the consumer brings MSVCRTD in via its own /MDd compile flag.
    # Release/RelWithDebInfo are unaffected (consumer is /MD = MSVCRT,
    # matching what the lib asks for). The flag propagates to every
    # consumer that links this target.
    # Recheck with "dumpbin /directives InternalFabricUUID.lib" on each SF
    # version bump; drop this option if the directive disappears.
    INTERFACE_LINK_OPTIONS "/IGNORE:4099;$<$<CONFIG:Debug>:/NODEFAULTLIB:MSVCRT>")

