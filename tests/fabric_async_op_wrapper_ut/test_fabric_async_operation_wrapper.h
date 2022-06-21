// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_FABRIC_ASYNC_OPERATION_CALL_H
#define TEST_FABRIC_ASYNC_OPERATION_CALL_H

#include "sf_c_util/fabric_async_op_wrapper.h"
#include "testasyncoperation.h"


#define TEST_FABRIC_OPERATION_SIGNATURE \
    BEGIN_ARGS(int, arg1), \
    END_ARGS(int, operation_result)

DECLARE_FABRIC_ASYNC_OPERATION(ITestAsyncOperation, TestOperation, TEST_FABRIC_OPERATION_SIGNATURE);


#endif // TEST_FABRIC_ASYNC_OPERATION_CALL_H
