#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

if(NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION)
    cmake_minimum_required(VERSION 3.18)
endif()

# canon way of using sf_native_helpers from another repo is below. It assumes the using repo has placed sf-native-helpers in "deps"

#if ((NOT TARGET sf_native_helpers) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/sf-native-helpers/CMakeLists.txt))
#    add_subdirectory(deps/sf-native-helpers)
#endif() 

if (TARGET sf_native_helpers)
    RETURN()
endif()

project(sf_native_helpers)

#the following variables are project-wide and can be used with cmake-gui
option(run_unittests "set run_unittests to ON to run unittests (default is OFF)" OFF)
option(run_e2e_tests "set run_e2e_tests to ON to run e2e tests (default is OFF). Chsare dutility does not have any e2e tests, but the option needs to exist to evaluate in IF statements" OFF)
option(run_int_tests "set run_int_tests to ON to integration tests (default is OFF)." OFF)
option(run_perf_tests "set run_perf_tests to ON to build performance tests (default is OFF)." OFF)
option(use_cppunittest "set use_cppunittest to ON to build CppUnitTest tests on Windows (default is OFF)" OFF)
option(run_traceability "run traceability tool (default is ON)" ON)

#bring in dependencies
#do not add or build any tests of the dependencies
set(original_run_e2e_tests ${run_e2e_tests})
set(original_run_int_tests ${run_int_tests})
set(original_run_perf_tests ${run_perf_tests})
set(original_run_unittests ${run_unittests})
set(original_run_traceability ${run_traceability})

set(run_e2e_tests OFF)
set(run_int_tests OFF)
set(run_perf_tests OFF)
set(run_unittests OFF)
set(run_traceability OFF)

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

if ((NOT TARGET com_wrapper) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/com-wrapper/CMakeLists.txt))
    add_subdirectory(deps/com-wrapper)
endif()

set(run_e2e_tests ${original_run_e2e_tests})
set(run_int_tests ${original_run_int_tests})
set(run_perf_tests ${original_run_perf_tests})
set(run_unittests ${original_run_unittests})
set(run_traceability ${original_run_traceability})

set(fabric_helpers_h_files
    inc/configuration_reader.h
    inc/fabric_async_op_cb.h
    inc/fabric_async_op_cb_com.h
    inc/fabric_async_op_wrapper.h
    inc/fabric_async_op_sync_wrapper.h
    inc/fabric_op_completed_sync_ctx.h
    inc/fabric_op_completed_sync_ctx_com.h
    inc/fabric_string_result.h
    inc/fabric_string_result_com.h
)

set(fabric_helpers_c_files
    src/configuration_reader.c
    src/fabric_async_op_cb.c
    src/fabric_async_op_cb_com.c
    src/fabric_op_completed_sync_ctx.c
    src/fabric_op_completed_sync_ctx_com.c
    src/fabric_string_result.c
    src/fabric_string_result_com.c
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(../deps/servicefabric/inc)

FILE(GLOB fabric_helpers_md_files "devdoc/*.md")
SOURCE_GROUP(devdoc FILES ${fabric_helpers_md_files})

add_library(fabric_helpers ${fabric_helpers_h_files} ${fabric_helpers_c_files} ${fabric_helpers_md_files})
target_include_directories(fabric_helpers PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)
target_link_libraries(fabric_helpers c_logging c_pal com_wrapper bs_utils)

add_subdirectory(tests)
