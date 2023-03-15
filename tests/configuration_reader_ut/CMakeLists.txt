#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName configuration_reader_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/configuration_reader.c
../../src/hresult_to_string.c
../../src/servicefabric_enums_to_strings.c
)

set(${theseTestsName}_h_files
../../inc/sf_c_util/configuration_reader.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS debug FabricUUIDD optimized FabricUUID c_pal_reals c_util_reals c_util_test_helpers)
