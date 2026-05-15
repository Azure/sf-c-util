#Copyright (C) Microsoft Corporation. All rights reserved.

# Service Fabric native libs/headers come from NuGet, restored at CMake
# configure time. Packages are resolved through the consuming repo's
# nuget.config (typically pointing at Azure-MessagingStore-Feed, which
# has the SF team's ServiceFabric feed configured as an upstream
# source). The package id is per-architecture and the pinned version is
# shared. See packages.config in this directory and readme.md for full
# rationale.
#
# The SF team's CI publishes only retail flavors of these packages
# (debug bits stopped being published mid-2024 for x64 and never existed
# for ARM64). FabricClient.lib and FabricRuntime.lib are tiny import libs
# (~4-25 KB) so debug vs retail is cosmetic; FabricUUID.lib and
# InternalFabricUUID.lib are GUID tables so debug vs retail is also
# cosmetic. Consumers should link the four targets unconditionally rather
# than using the legacy debug X D / optimized X idiom.

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
    message(FATAL_ERROR
        "nuget.exe not found on PATH. Install it (download from https://www.nuget.org/downloads "
        "or 'choco install nuget.commandline') so the Service Fabric native libs can be "
        "restored from the ADO ServiceFabric feed.")
endif()

set(SF_PACKAGES_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/packages.config")

# Pick a nuget.config to drive feed resolution. Precedence:
#
#   1. ${CMAKE_SOURCE_DIR}/nuget.config (the TOP-LEVEL project's config,
#      regardless of how deeply this CMakeLists is nested). When
#      sf-c-util is consumed as a submodule of a larger project (e.g.
#      ebs), this means the parent project's nuget.config wins -
#      consistent with the "single feed compliance" pattern where the
#      parent repo controls all dependency sources via upstream-sources
#      on its own feed.
#
#   2. Walk up from CMAKE_CURRENT_SOURCE_DIR looking for the first
#      nuget.config. Catches the case where a parent project has its
#      nuget.config somewhere other than its CMakeLists root, or where
#      sf-c-util is itself the top-level project (standalone build, in
#      which case CMAKE_SOURCE_DIR is sf-c-util's root and step 1
#      already found sf-c-util's own nuget.config).
#
#   3. None found: omit -ConfigFile and let nuget.exe fall back to its
#      standard discovery (%AppData%\NuGet\NuGet.Config etc.). This
#      gives a meaningful "no source can serve this package" error
#      rather than a silent restore failure.
set(SF_NUGET_CONFIG "")
foreach (_candidate IN ITEMS "nuget.config" "NuGet.config" "NuGet.Config")
    if (EXISTS "${CMAKE_SOURCE_DIR}/${_candidate}")
        set(SF_NUGET_CONFIG "${CMAKE_SOURCE_DIR}/${_candidate}")
        break()
    endif()
endforeach()
if (NOT SF_NUGET_CONFIG)
    set(_sf_search_dir "${CMAKE_CURRENT_SOURCE_DIR}")
    while (TRUE)
        foreach (_candidate IN ITEMS "nuget.config" "NuGet.config" "NuGet.Config")
            if (EXISTS "${_sf_search_dir}/${_candidate}")
                set(SF_NUGET_CONFIG "${_sf_search_dir}/${_candidate}")
                break()
            endif()
        endforeach()
        if (SF_NUGET_CONFIG)
            break()
        endif()
        get_filename_component(_sf_parent_dir "${_sf_search_dir}" DIRECTORY)
        if (_sf_parent_dir STREQUAL _sf_search_dir)
            break()
        endif()
        set(_sf_search_dir "${_sf_parent_dir}")
    endwhile()
endif()

# Mirror the chosen nuget.config into the cmake build directory so any
# downstream nuget restore step (e.g. msbuild /t:restore against the
# generated csproj/.slnx files) finds it via cwd-based discovery
# without needing an explicit -ConfigFile argument. This matches the
# pattern ebs uses for its own packaged restores.
if (SF_NUGET_CONFIG)
    configure_file("${SF_NUGET_CONFIG}" "${CMAKE_BINARY_DIR}/nuget.config" COPYONLY)
endif()

set(_sf_restore_args restore ${SF_PACKAGES_CONFIG}
    -PackagesDirectory ${SF_PACKAGES_DIR}
    -NonInteractive)
if (SF_NUGET_CONFIG)
    list(APPEND _sf_restore_args -ConfigFile ${SF_NUGET_CONFIG})
    message(STATUS "Restoring Service Fabric NuGet packages from ${SF_PACKAGES_CONFIG} (using ${SF_NUGET_CONFIG})")
else()
    message(STATUS "Restoring Service Fabric NuGet packages from ${SF_PACKAGES_CONFIG} (using nuget.exe default config discovery)")
endif()

execute_process(
    COMMAND ${NUGET_EXE} ${_sf_restore_args}
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

