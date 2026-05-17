#Copyright (C) Microsoft Corporation. All rights reserved.

# Service Fabric native libs/headers from NuGet. See readme.md.

set(SF_NUGET_PKG_VERSION "11.4.268-internal")

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

# Workaround for a bug in the SF-team-published NativeLibsHeaders.retail
# 11.4.x packages: fabricclient.h declares two methods on the
# IFabricQueryClient14Vtbl struct (BeginGetFailoverManagerManagerInformation
# and EndGetFailoverManagerManagerInformation) using a malformed
# function-pointer syntax. The function-pointer parens around
# (STDMETHODCALLTYPE * <name>) are missing, and the implicit
# IFabricQueryClient14 *This first parameter is also missing, so MSVC
# rejects the whole struct with C2165/C2032 the moment fabricclient.h is
# pulled in. Other vtable members and the C++ virtual declaration of the
# same methods are well-formed; the bug appears to be a one-off MIDL/edit
# regression that surfaced in 11.4.205-internal-rc and persists through
# 11.4.268-internal. The bug is absent in 11.3.x and earlier.
#
# Patch the offending two lines in place after restore so the header
# parses. sf-c-util does not call these methods (we only QI to
# IFabricQueryClient10), so the patch is purely about getting the file
# through cl.exe. Idempotent: after the first run the broken substring
# is gone, subsequent configure runs find nothing to patch.
#
# TODO: drop this workaround once the SF team publishes a fixed package
# (or we move to a CU that no longer has the bug).
set(_sf_fabricclient_h "${SF_PKG_ROOT}/content/native/prebuilt/sdk/fabricclient.h")
if (EXISTS "${_sf_fabricclient_h}")
    file(READ "${_sf_fabricclient_h}" _sf_fabricclient_contents)
    set(_sf_patched FALSE)
    foreach (_sf_method IN ITEMS "BeginGetFailoverManagerManagerInformation" "EndGetFailoverManagerManagerInformation")
        set(_sf_broken "        HRESULT STDMETHODCALLTYPE * ${_sf_method}(")
        set(_sf_fixed  "        HRESULT(STDMETHODCALLTYPE * ${_sf_method})(IFabricQueryClient14 * This,")
        string(FIND "${_sf_fabricclient_contents}" "${_sf_broken}" _sf_idx)
        if (NOT _sf_idx EQUAL -1)
            string(REPLACE "${_sf_broken}" "${_sf_fixed}" _sf_fabricclient_contents "${_sf_fabricclient_contents}")
            set(_sf_patched TRUE)
        endif()
    endforeach()
    if (_sf_patched)
        message(STATUS "Patched SF-team bug in ${_sf_fabricclient_h} (IFabricQueryClient14 FailoverManagerManager methods)")
        file(WRITE "${_sf_fabricclient_h}" "${_sf_fabricclient_contents}")
    endif()
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
    INTERFACE_INCLUDE_DIRECTORIES "${SF_INC_DIRS}")

add_library(FabricUUIDInternal STATIC IMPORTED GLOBAL)
set_target_properties(FabricUUIDInternal PROPERTIES
    IMPORTED_LOCATION "${SF_PKG_ROOT}/lib/native/InternalFabricUUID.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SF_INC_DIRS}"
    # InternalFabricUUID.lib in the SF NativeLibsHeaders.retail packages
    # was compiled /MD against the release dynamic CRT and embeds a
    # /DEFAULTLIB:MSVCRT directive. When a consumer is built /MDd
    # (Debug) the linker sees both MSVCRT and MSVCRTD requested, raises
    # LNK4098, and any consumer with /WX promotes that to LNK1218.
    # Tell the linker to ignore MSVCRT default-lib requests in Debug;
    # the consumer brings MSVCRTD in via its own /MDd compile flag.
    # Release/RelWithDebInfo are unaffected (consumer is /MD = MSVCRT,
    # matching what the lib asks for). The flag propagates to every
    # consumer that link this target.
    # TODO: remove once SF publishes a NativeLibsHeaders.debug variant
    # for the win-arm64 / 11.4 line, or a NativeLibsHeaders.retail that
    # stops shipping a /DEFAULTLIB:MSVCRT directive in the lib.
    INTERFACE_LINK_OPTIONS "$<$<CONFIG:Debug>:/NODEFAULTLIB:MSVCRT>")

