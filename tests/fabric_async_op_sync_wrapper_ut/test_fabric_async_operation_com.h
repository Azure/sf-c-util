// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_FABRIC_ASYNC_OPERATION_COM_H
#define TEST_FABRIC_ASYNC_OPERATION_COM_H


#include "windows.h"
#include "fabriccommon.h"
#include "unknwn.h"
#include "testasyncoperation.h"
#include "com_wrapper/com_wrapper.h"
#include "test_fabric_async_operation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FABRIC_ASYNC_OPERATION_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(ITestAsyncOperation, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_BeginTestOperation, int, arg1, IFabricAsyncOperationCallback*, callback, IFabricAsyncOperationContext**, context), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_EndTestOperation, IFabricAsyncOperationContext*, context, int*, operation_result_1, double*, operation_result_2), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_BeginTestOperationWithNoBeginArgs, IFabricAsyncOperationCallback*, callback, IFabricAsyncOperationContext**, context), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_EndTestOperationWithNoBeginArgs, IFabricAsyncOperationContext*, context, int*, operation_result_1, double*, operation_result_2), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_BeginTestOperationWithNoEndArgs, int, arg1, IFabricAsyncOperationCallback*, callback, IFabricAsyncOperationContext**, context), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_EndTestOperationWithNoEndArgs, IFabricAsyncOperationContext*, context), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_BeginTestOperationWithNoArgs, IFabricAsyncOperationCallback*, callback, IFabricAsyncOperationContext**, context), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_fabric_async_operation_EndTestOperationWithNoArgs, IFabricAsyncOperationContext*, context) \
    )

    DECLARE_COM_WRAPPER_OBJECT(TEST_FABRIC_ASYNC_OPERATION_HANDLE, TEST_FABRIC_ASYNC_OPERATION_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* TEST_FABRIC_ASYNC_OPERATION_COM_H */
