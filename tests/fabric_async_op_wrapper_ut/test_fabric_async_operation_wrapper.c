// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "test_fabric_async_operation_wrapper.h"
#include "sf_c_util/fabric_async_op_wrapper.h"
#include "testasyncoperation.h"

DEFINE_FABRIC_ASYNC_OPERATION(ITestAsyncOperation, TestOperation, TEST_FABRIC_OPERATION_SIGNATURE);
