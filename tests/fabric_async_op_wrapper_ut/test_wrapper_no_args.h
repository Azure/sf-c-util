// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_WRAPPER_NO_ARGS_H
#define TEST_WRAPPER_NO_ARGS_H

#include "fabric_async_op_wrapper.h"
#include "testasyncoperation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FABRIC_OPERATION_WITH_NO_ARGS_SIGNATURE \
    BEGIN_ARGS(), \
    END_ARGS()

DECLARE_FABRIC_ASYNC_OPERATION(ITestAsyncOperation, TestOperationWithNoArgs, TEST_FABRIC_OPERATION_WITH_NO_ARGS_SIGNATURE)

#ifdef __cplusplus
}
#endif

#endif // TEST_WRAPPER_NO_ARGS_H
