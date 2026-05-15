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

# Find a nuget.config to drive feed resolution. We walk from the
# packages.config directory up to the filesystem root looking for the
# first nuget.config (case-insensitive on Windows). This works for both
# the embedded build (sf-c-util as a submodule, where the parent repo
# typically owns nuget.config at its root) and the standalone build
# (sf-c-util built on its own, where the developer is expected to drop
# a nuget.config into the source tree or rely on user/machine config).
# If nothing is found we omit -ConfigFile and let nuget.exe fall back to
# its standard discovery (%AppData%\NuGet\NuGet.Config etc.).
set(_sf_search_dir "${CMAKE_CURRENT_SOURCE_DIR}")
set(SF_NUGET_CONFIG "")
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
    INTERFACE_INCLUDE_DIRECTORIES "${SF_INC_DIRS}")

