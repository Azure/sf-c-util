#Copyright (c) Microsoft. All rights reserved.

compileAsC11()
set(theseTestsName fabric_op_completed_sync_ctx_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/fabric_op_completed_sync_ctx.c
test_callback.c
)

set(${theseTestsName}_h_files
../../inc/fabric_op_completed_sync_ctx.h
test_callback.h
test_callback_com.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS c_util debug FabricUUIDD optimized FabricUUID com_wrapper c_pal_reals)
