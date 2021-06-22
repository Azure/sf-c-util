// Copyright (c) Microsoft. All rights reserved.

#ifndef TEST_WRAPPER_NO_BEGIN_ARGS_H
#define TEST_WRAPPER_NO_BEGIN_ARGS_H

#include "fabric_async_op_sync_wrapper.h"
#include "testasyncoperation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FABRIC_OPERATION_WITH_NO_BEGIN_ARGS_SIGNATURE \
    BEGIN_ARGS(), \
    END_ARGS(int, operation_result_1, double, operation_result_2)

DECLARE_FABRIC_ASYNC_OPERATION_SYNC(ITestAsyncOperation, TestOperationWithNoBeginArgs, TEST_FABRIC_OPERATION_WITH_NO_BEGIN_ARGS_SIGNATURE);

#ifdef __cplusplus
}
#endif

#endif // TEST_WRAPPER_NO_BEGIN_ARGS_H
