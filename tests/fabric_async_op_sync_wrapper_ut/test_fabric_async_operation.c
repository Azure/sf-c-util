// Copyright (c) Microsoft. All rights reserved.

#include <stdlib.h>


#include "fabriccommon.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "test_fabric_async_operation.h"


typedef struct TEST_FABRIC_ASYNC_OPERATION_TAG
{
    int dummy;
} TEST_FABRIC_ASYNC_OPERATION;

TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation_create(void)
{
    TEST_FABRIC_ASYNC_OPERATION_HANDLE result = malloc(sizeof(TEST_FABRIC_ASYNC_OPERATION));
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

void test_fabric_async_operation_destroy(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation)
{
    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p", test_fabric_async_operation);
    }
    else
    {
        free(test_fabric_async_operation);
    }
}

HRESULT test_fabric_async_operation_BeginTestOperation(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, int arg1, IFabricAsyncOperationCallback* callback, IFabricAsyncOperationContext** context)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, int arg1=%d, IFabricAsyncOperationCallback* callback=%p, IFabricAsyncOperationContext** context=%p",
            test_fabric_async_operation, arg1, callback, context);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}

HRESULT test_fabric_async_operation_EndTestOperation(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, IFabricAsyncOperationContext* context, int* operation_result_1, double* operation_result_2)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, IFabricAsyncOperationContext* context=%p, int* operation_result_1=%p, double* operation_result_2=%p",
            test_fabric_async_operation, context, operation_result_1, operation_result_2);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}

HRESULT test_fabric_async_operation_BeginTestOperationWithNoBeginArgs(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, IFabricAsyncOperationCallback* callback, IFabricAsyncOperationContext** context)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, IFabricAsyncOperationCallback* callback=%p, IFabricAsyncOperationContext** context=%p",
            test_fabric_async_operation, callback, context);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}

HRESULT test_fabric_async_operation_EndTestOperationWithNoBeginArgs(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, IFabricAsyncOperationContext* context, int* operation_result_1, double* operation_result_2)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, IFabricAsyncOperationContext* context=%p, int* operation_result_1=%p, double* operation_result_2=%p",
            test_fabric_async_operation, context, operation_result_1, operation_result_2);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}

HRESULT test_fabric_async_operation_BeginTestOperationWithNoEndArgs(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, int arg1, IFabricAsyncOperationCallback* callback, IFabricAsyncOperationContext** context)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, int arg1=%d, IFabricAsyncOperationCallback* callback=%p, IFabricAsyncOperationContext** context=%p",
            test_fabric_async_operation, arg1, callback, context);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}

HRESULT test_fabric_async_operation_EndTestOperationWithNoEndArgs(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, IFabricAsyncOperationContext* context)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, IFabricAsyncOperationContext* context=%p",
            test_fabric_async_operation, context);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}

HRESULT test_fabric_async_operation_BeginTestOperationWithNoArgs(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, IFabricAsyncOperationCallback* callback, IFabricAsyncOperationContext** context)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, IFabricAsyncOperationCallback* callback=%p, IFabricAsyncOperationContext** context=%p",
            test_fabric_async_operation, callback, context);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}

HRESULT test_fabric_async_operation_EndTestOperationWithNoArgs(TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation, IFabricAsyncOperationContext* context)
{
    HRESULT result;

    if (test_fabric_async_operation == NULL)
    {
        LogError("Invalid arguments: TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation=%p, IFabricAsyncOperationContext* context=%p",
            test_fabric_async_operation, context);
        result = E_INVALIDARG;
    }
    else
    {
        result = S_OK;
    }

    return result;
}
