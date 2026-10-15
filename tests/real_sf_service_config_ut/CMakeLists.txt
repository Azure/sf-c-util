#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName real_sf_service_config_ut)

set(${theseTestsName}_test_files
    ${theseTestsName}.c
)

set(${theseTestsName}_c_files
    real_sample_service_config.c
    real_sample_config_consumer.c
)

#the plain sources are only compiled through their reals, compiling them on their own would clash with the mocks of the test
set(${theseTestsName}_plain_c_files
    sample_service_config.c
    sample_config_consumer.c
)
set_source_files_properties(${${theseTestsName}_plain_c_files} PROPERTIES HEADER_FILE_ONLY ON)

set(${theseTestsName}_h_files
    sample_service_config.h
    sample_config_consumer.h
    real_sample_service_config_renames.h
    ${${theseTestsName}_plain_c_files}
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS FabricUUID c_pal_reals c_util_reals sf_c_util sf_c_util_reals
    ENABLE_TEST_FILES_PRECOMPILED_HEADERS "${CMAKE_CURRENT_LIST_DIR}/${theseTestsName}_pch.h")
