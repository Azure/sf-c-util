// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef FABRIC_ASYNC_OP_CB_COM_H
#define FABRIC_ASYNC_OP_CB_COM_H


#include "windows.h"
#include "fabriccommon.h"
#include "unknwn.h"
#include "com_wrapper/com_wrapper.h"
#include "fabric_async_op_cb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FABRIC_ASYNC_OP_CB_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricAsyncOperationCallback, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_async_op_cb_Invoke, IFabricAsyncOperationContext*, context) \
    )

    DECLARE_COM_WRAPPER_OBJECT(FABRIC_ASYNC_OP_CB_HANDLE, FABRIC_ASYNC_OP_CB_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_ASYNC_OP_CB_COM_H */
