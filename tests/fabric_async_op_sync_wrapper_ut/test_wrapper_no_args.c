// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "test_wrapper_no_args.h"
#include "sf_c_util/fabric_async_op_sync_wrapper.h"
#include "testasyncoperation.h"

DEFINE_FABRIC_ASYNC_OPERATION_SYNC(ITestAsyncOperation, TestOperationWithNoArgs, TEST_FABRIC_OPERATION_WITH_NO_ARGS_SIGNATURE)
