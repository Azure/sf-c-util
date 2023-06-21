// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "windows.h"


#include "fabrictypes.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "test_async_operation_context.h"


typedef struct TEST_ASYNC_OPERATION_CONTEXT_TAG
{
    int dummy;
} TEST_ASYNC_OPERATION_CONTEXT;

TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context_create(void)
{
    TEST_ASYNC_OPERATION_CONTEXT_HANDLE result = malloc(sizeof(TEST_ASYNC_OPERATION_CONTEXT));
    if (result == NULL)
    {
        LogError("malloc failed");
    }
    else
    {
        // all OK
    }

    return result;
}

void test_async_operation_context_destroy(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context)
{
    if (test_async_operation_context == NULL)
    {
        LogError("Invalid arguments: TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context=%p", test_async_operation_context);
    }
    else
    {
        free(test_async_operation_context);
    }
}

BOOLEAN test_async_operation_context_IsCompleted(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context)
{
    (void)test_async_operation_context;
    return FALSE;
}

BOOLEAN test_async_operation_context_CompletedSynchronously(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context)
{
    (void)test_async_operation_context;
    return FALSE;
}

HRESULT test_async_operation_context_get_Callback(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context, IFabricAsyncOperationCallback** callback)
{
    (void)test_async_operation_context;
    (void)callback;
    return S_OK;
}

HRESULT test_async_operation_context_Cancel(TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context)
{
    (void)test_async_operation_context;
    return S_OK;
}
