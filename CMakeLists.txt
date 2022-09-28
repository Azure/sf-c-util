#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

if(NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION)
    cmake_minimum_required(VERSION 3.18)
endif()

# canon way of using sf_c_util from another repo is below. It assumes the using repo has placed sf-c-util in "deps"

#if ((NOT TARGET sf_c_util) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/sf-c-util/CMakeLists.txt))
#    add_subdirectory(deps/sf-c-util)
#    include_directories(deps/sf-c-util/inc)
#endif()

if (TARGET sf_c_util)
    RETURN()
endif()

project(sf_c_util)

#the following variables are project-wide and can be used with cmake-gui
option(run_unittests "set run_unittests to ON to run unittests (default is OFF)" OFF)
option(run_e2e_tests "set run_e2e_tests to ON to run e2e tests (default is OFF). Chsare dutility does not have any e2e tests, but the option needs to exist to evaluate in IF statements" OFF)
option(run_int_tests "set run_int_tests to ON to integration tests (default is OFF)." OFF)
option(run_perf_tests "set run_perf_tests to ON to build performance tests (default is OFF)." OFF)
option(run_reals_check "set run_reals_check to ON to run reals check (default is OFF)." OFF)
option(use_cppunittest "set use_cppunittest to ON to build CppUnitTest tests on Windows (default is OFF)" OFF)
option(run_traceability "run traceability tool (default is ON)" ON)

#bring in dependencies
#do not add or build any tests of the dependencies
set(original_run_e2e_tests ${run_e2e_tests})
set(original_run_int_tests ${run_int_tests})
set(original_run_perf_tests ${run_perf_tests})
set(original_run_unittests ${run_unittests})
set(original_run_traceability ${run_traceability})
set(original_run_reals_check ${run_reals_check})

set(run_e2e_tests OFF)
set(run_int_tests OFF)
set(run_perf_tests OFF)
set(run_unittests OFF)
set(run_traceability OFF)
set(run_reals_check OFF)

if ((NOT TARGET c_build_tools) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-build-tools/CMakeLists.txt))
    add_subdirectory(deps/c-build-tools)
    set_default_build_options()
endif()

if ((NOT TARGET macro_utils_c) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/macro-utils-c/CMakeLists.txt))
    add_subdirectory(deps/macro-utils-c)
    include_directories(${MACRO_UTILS_INC_FOLDER})
endif()

if ((NOT TARGET c_logging) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-logging/CMakeLists.txt))
    add_subdirectory(deps/c-logging)
    include_directories(deps/c-logging/inc)
endif()

if ((NOT TARGET ctest) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/ctest/CMakeLists.txt))
    add_subdirectory(deps/ctest)
    include_directories(${CTEST_INC_FOLDER})
endif()

if ((NOT TARGET testrunnerswitcher) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-testrunnerswitcher/CMakeLists.txt))
    add_subdirectory(deps/c-testrunnerswitcher)
    include_directories(${TESTRUNNERSWITCHER_INC_FOLDER})
endif()

if ((NOT TARGET umock_c) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/umock-c/CMakeLists.txt))
    add_subdirectory(deps/umock-c)
    include_directories(${UMOCK_C_INC_FOLDER})
endif()

if ((NOT TARGET c_pal) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-pal/CMakeLists.txt))
    add_subdirectory(deps/c-pal)
    include_directories(${C_PAL_INC_FOLDER})
endif()

if ((NOT TARGET c_util) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-util/CMakeLists.txt))
    add_subdirectory(deps/c-util)
    include_directories(${C_UTIL_INC_FOLDER})
endif()

if ((NOT TARGET com_wrapper) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/com-wrapper/CMakeLists.txt))
    add_subdirectory(deps/com-wrapper)
endif()

set(run_e2e_tests ${original_run_e2e_tests})
set(run_int_tests ${original_run_int_tests})
set(run_perf_tests ${original_run_perf_tests})
set(run_unittests ${original_run_unittests})
set(run_traceability ${original_run_traceability})
set(run_reals_check ${original_run_reals_check})

add_subdirectory(deps/servicefabric)

if((CMAKE_GENERATOR MATCHES "Visual Studio") AND (${run_traceability}))
    #add traceability custom target
    add_custom_target(sf_c_util_traceability ALL
        COMMAND traceabilitytool -buildcheck -e ${CMAKE_CURRENT_LIST_DIR}/deps -i ${CMAKE_CURRENT_LIST_DIR})
    add_dependencies(sf_c_util_traceability traceabilitytool)
endif()

set(sf_c_util_h_files
    inc/sf_c_util/configuration_reader.h
    inc/sf_c_util/fabric_async_op_cb.h
    inc/sf_c_util/fabric_async_op_cb_com.h
    inc/sf_c_util/fabric_async_op_wrapper.h
    inc/sf_c_util/fabric_async_op_sync_wrapper.h
    inc/sf_c_util/fabric_op_completed_sync_ctx.h
    inc/sf_c_util/fabric_op_completed_sync_ctx_com.h
    inc/sf_c_util/fabric_string_result.h
    inc/sf_c_util/fabric_string_result_com.h
    inc/sf_c_util/hresult_to_string.h
    inc/sf_c_util/servicefabric_enums_to_strings.h
    inc/sf_c_util/sf_service_config.h
)

set(sf_c_util_c_files
    src/configuration_reader.c
    src/fabric_async_op_cb.c
    src/fabric_async_op_cb_com.c
    src/fabric_op_completed_sync_ctx.c
    src/fabric_op_completed_sync_ctx_com.c
    src/fabric_string_result.c
    src/fabric_string_result_com.c
    src/hresult_to_string.c
    src/servicefabric_enums_to_strings.c
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/deps/servicefabric/inc)

FILE(GLOB sf_c_util_md_files "devdoc/*.md")
SOURCE_GROUP(devdoc FILES ${sf_c_util_md_files})

add_library(sf_c_util ${sf_c_util_h_files} ${sf_c_util_c_files} ${sf_c_util_md_files})
target_include_directories(sf_c_util PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)
target_link_libraries(sf_c_util c_logging c_pal com_wrapper c_util)

add_subdirectory(sfwrapper)
add_subdirectory(tests)

if(${run_reals_check})
    add_reals_check_target()
endif()
