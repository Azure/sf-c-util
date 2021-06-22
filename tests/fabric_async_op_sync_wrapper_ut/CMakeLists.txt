#Copyright (c) Microsoft. All rights reserved.

compileAsC11()
set(theseTestsName fabric_async_op_sync_wrapper_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
test_fabric_async_operation.c
test_fabric_async_operation_sync_wrapper.c
test_wrapper_no_begin_args.c
test_wrapper_no_end_args.c
test_wrapper_no_args.c
testasyncoperation_i.c
test_async_operation_context.c
)

set(${theseTestsName}_h_files
../../inc/fabric_async_op_sync_wrapper.h
test_fabric_async_operation.h
test_fabric_async_operation_com.h
test_fabric_async_operation_sync_wrapper.h
test_wrapper_no_begin_args.h
test_wrapper_no_end_args.h
test_wrapper_no_args.h
test_async_operation_context.h
test_async_operation_context_com.h
testasyncoperation.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS c_util debug FabricUUIDD optimized FabricUUID com_wrapper synchronization c_pal_reals)
