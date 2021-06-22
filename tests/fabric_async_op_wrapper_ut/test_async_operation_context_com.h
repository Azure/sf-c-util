// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_ASYNC_OPERATION_CONTEXT_COM_H
#define TEST_ASYNC_OPERATION_CONTEXT_COM_H

#include "test_async_operation_context.h"
#include "com_wrapper/com_wrapper.h"


#include "Unknwnbase.h"
#include "windows.h"
#include "fabriccommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_ASYNC_OPERATION_CONTEXT_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricAsyncOperationContext, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(BOOLEAN, test_async_operation_context_IsCompleted), \
        COM_WRAPPER_FUNCTION_WRAPPER(BOOLEAN, test_async_operation_context_CompletedSynchronously), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_async_operation_context_get_Callback, IFabricAsyncOperationCallback**, callback), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_async_operation_context_Cancel) \
    )

DECLARE_COM_WRAPPER_OBJECT(TEST_ASYNC_OPERATION_CONTEXT_HANDLE, TEST_ASYNC_OPERATION_CONTEXT_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* TEST_ASYNC_OPERATION_CONTEXT_COM_H */
