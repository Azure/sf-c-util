#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

compileAsC11()
set(theseTestsName fabric_async_op_cb_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/fabric_async_op_cb.c
)

set(${theseTestsName}_h_files
../../inc/sf_c_util/fabric_async_op_cb.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS c_util debug FabricUUIDD optimized FabricUUID c_pal_reals)
