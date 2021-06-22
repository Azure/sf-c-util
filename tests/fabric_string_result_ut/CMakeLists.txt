#Copyright (c) Microsoft. All rights reserved.

compileAsC11()
set(theseTestsName fabric_string_result_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/fabric_string_result.c
)

set(${theseTestsName}_h_files
../../inc/fabric_string_result.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/fabric_helpers" ADDITIONAL_LIBS c_util debug FabricUUIDD optimized FabricUUID c_pal_reals)
