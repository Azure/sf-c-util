#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName fabric_op_completed_sync_ctx_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../../src/fabric_op_completed_sync_ctx.c
test_callback.c
)

set(${theseTestsName}_h_files
../../inc/sf_c_util/fabric_op_completed_sync_ctx.h
test_callback.h
test_callback_com.h
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS c_util debug FabricUUIDD optimized FabricUUID com_wrapper c_pal_reals
    ENABLE_TEST_FILES_PRECOMPILED_HEADERS "${CMAKE_CURRENT_LIST_DIR}/fabric_op_completed_sync_ctx_ut_pch.h")
