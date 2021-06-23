// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_WRAPPER_NO_BEGIN_ARGS_H
#define TEST_WRAPPER_NO_BEGIN_ARGS_H

#include "sf_c_util/fabric_async_op_wrapper.h"
#include "testasyncoperation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FABRIC_OPERATION_WITH_NO_BEGIN_ARGS_SIGNATURE \
    BEGIN_ARGS(), \
    END_ARGS(int, operation_result_1, double, operation_result_2)

DECLARE_FABRIC_ASYNC_OPERATION(ITestAsyncOperation, TestOperationWithNoBeginArgs, TEST_FABRIC_OPERATION_WITH_NO_BEGIN_ARGS_SIGNATURE);

#ifdef __cplusplus
}
#endif

#endif // TEST_WRAPPER_NO_BEGIN_ARGS_H
