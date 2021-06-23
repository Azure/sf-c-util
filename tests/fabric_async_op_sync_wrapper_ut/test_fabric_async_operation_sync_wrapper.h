// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_FABRIC_ASYNC_OPERATION_CALL_H
#define TEST_FABRIC_ASYNC_OPERATION_CALL_H

#include "sf_c_util/fabric_async_op_sync_wrapper.h"
#include "testasyncoperation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FABRIC_OPERATION_SIGNATURE \
    BEGIN_ARGS(int, arg1), \
    END_ARGS(int, operation_result_1, double, operation_result_2)

DECLARE_FABRIC_ASYNC_OPERATION_SYNC(ITestAsyncOperation, TestOperation, TEST_FABRIC_OPERATION_SIGNATURE)

#ifdef __cplusplus
}
#endif

#endif // TEST_FABRIC_ASYNC_OPERATION_CALL_H
