#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName sf_service_config_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
test_sf_service_config.c
)

set(${theseTestsName}_h_files
sf_service_config_ut_helpers.h
test_sf_service_config.h
../../inc/sf_c_util/sf_service_config.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS debug FabricUUIDD optimized FabricUUID c_pal_reals c_util_reals c_util_test_helpers
    ENABLE_TEST_FILES_PRECOMPILED_HEADERS "${CMAKE_CURRENT_LIST_DIR}/sf_service_config_ut_pch.h")
