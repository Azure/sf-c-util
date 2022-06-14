#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

compileAsC11()
set(theseTestsName configuration_wrapper_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
test_configuration_wrapper.c
)

set(${theseTestsName}_h_files
configuration_wrapper_ut_helpers.h
test_configuration_wrapper.h
../../inc/sf_c_util/configuration_wrapper.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS debug FabricUUIDD optimized FabricUUID c_pal_reals c_util_reals c_util_test_helpers)
