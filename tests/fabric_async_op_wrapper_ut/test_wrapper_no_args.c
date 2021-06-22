// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "test_wrapper_no_args.h"
#include "fabric_async_op_wrapper.h"
#include "testasyncoperation.h"

DEFINE_FABRIC_ASYNC_OPERATION(ITestAsyncOperation, TestOperationWithNoArgs, TEST_FABRIC_OPERATION_WITH_NO_ARGS_SIGNATURE)
