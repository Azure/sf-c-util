#Copyright (c) Microsoft. All rights reserved.

compileAsC11()
set(theseTestsName configuration_reader_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/configuration_reader.c
)

set(${theseTestsName}_h_files
../../inc/configuration_reader.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/fabric_helpers" ADDITIONAL_LIBS debug FabricUUIDD optimized FabricUUID bs_utils test_helpers c_pal_reals c_util_reals)
