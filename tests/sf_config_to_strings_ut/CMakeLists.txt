#Copyright (c) Microsoft. All rights reserved.

set(theseTestsName sf_config_to_strings_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
mocked_sf_configuration.c
)

set(${theseTestsName}_h_files
../../inc/sf_c_util/sf_config_to_strings.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)
include_directories($<TARGET_PROPERTY:sf_c_util,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS debug FabricUUIDD optimized FabricUUID sf_c_util c_pal_reals c_util_reals c_util_test_helpers rpcrt4)
