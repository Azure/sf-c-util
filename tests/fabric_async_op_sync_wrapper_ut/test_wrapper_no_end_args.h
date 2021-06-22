// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_WRAPPER_NO_END_ARGS_H
#define TEST_WRAPPER_NO_END_ARGS_H

#include "fabric_async_op_sync_wrapper.h"
#include "testasyncoperation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FABRIC_OPERATION_WITH_NO_END_ARGS_SIGNATURE \
    BEGIN_ARGS(int, arg1), \
    END_ARGS()

DECLARE_FABRIC_ASYNC_OPERATION_SYNC(ITestAsyncOperation, TestOperationWithNoEndArgs, TEST_FABRIC_OPERATION_WITH_NO_END_ARGS_SIGNATURE)

#ifdef __cplusplus
}
#endif

#endif // TEST_WRAPPER_NO_END_ARGS_H
