#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName configuration_package_change_handler_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/configuration_package_change_handler.c
)

set(${theseTestsName}_h_files
../../inc/sf_c_util/configuration_package_change_handler.h
../../inc/sf_c_util/configuration_package_change_handler_com.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS c_util com_wrapper debug FabricUUIDD optimized FabricUUID c_pal_reals
    ENABLE_TEST_FILES_PRECOMPILED_HEADERS "${CMAKE_CURRENT_LIST_DIR}/configuration_package_change_handler_ut_pch.h")
