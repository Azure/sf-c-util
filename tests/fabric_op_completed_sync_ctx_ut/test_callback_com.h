// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef TEST_CALLBACK_COM_H
#define TEST_CALLBACK_COM_H

#include "test_callback.h"
#include "com_wrapper/com_wrapper.h"


#include "Unknwnbase.h"
#include "windows.h"
#include "fabriccommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_CALLBACK_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricAsyncOperationCallback, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, test_callback_Invoke, IFabricAsyncOperationContext*, context) \
    )

DECLARE_COM_WRAPPER_OBJECT(TEST_CALLBACK_HANDLE, TEST_CALLBACK_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* TEST_CALLBACK_COM_H */
