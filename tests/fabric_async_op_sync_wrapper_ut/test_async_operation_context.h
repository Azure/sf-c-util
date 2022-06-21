// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_ASYNC_OPERATION_CONTEXT_H
#define TEST_ASYNC_OPERATION_CONTEXT_H


#include "windows.h"
#include "fabriccommon.h" // for IFabricAsyncOperationCallback


typedef struct TEST_ASYNC_OPERATION_CONTEXT_TAG* TEST_ASYNC_OPERATION_CONTEXT_HANDLE;

TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context_create(void);
void test_async_operation_context_destroy(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context);
BOOLEAN test_async_operation_context_IsCompleted(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context);
BOOLEAN test_async_operation_context_CompletedSynchronously(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context);
HRESULT test_async_operation_context_get_Callback(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context, IFabricAsyncOperationCallback** callback);
HRESULT test_async_operation_context_Cancel(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context);


#endif /* TEST_ASYNC_OPERATION_CONTEXT_H */
