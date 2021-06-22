// Copyright (c) Microsoft. All rights reserved.

#include "test_wrapper_no_begin_args.h"
#include "fabric_async_op_sync_wrapper.h"
#include "testasyncoperation.h"

DEFINE_FABRIC_ASYNC_OPERATION_SYNC(ITestAsyncOperation, TestOperationWithNoBeginArgs, TEST_FABRIC_OPERATION_WITH_NO_BEGIN_ARGS_SIGNATURE)
