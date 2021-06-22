// Copyright (c) Microsoft. All rights reserved.

#include "test_wrapper_no_end_args.h"
#include "fabric_async_op_wrapper.h"
#include "testasyncoperation.h"

DEFINE_FABRIC_ASYNC_OPERATION(ITestAsyncOperation, TestOperationWithNoEndArgs, TEST_FABRIC_OPERATION_WITH_NO_END_ARGS_SIGNATURE)
