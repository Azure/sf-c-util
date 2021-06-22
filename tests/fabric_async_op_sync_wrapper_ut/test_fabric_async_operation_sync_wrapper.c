// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "test_fabric_async_operation_sync_wrapper.h"
#include "fabric_async_op_sync_wrapper.h"
#include "testasyncoperation.h"

DEFINE_FABRIC_ASYNC_OPERATION_SYNC(ITestAsyncOperation, TestOperation, TEST_FABRIC_OPERATION_SIGNATURE)
