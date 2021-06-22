// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef FABRIC_OP_COMPLETED_SYNC_CTX_COM_H
#define FABRIC_OP_COMPLETED_SYNC_CTX_COM_H


#include "windows.h"
#include "fabriccommon.h"
#include "unknwn.h"
#include "com_wrapper/com_wrapper.h"
#include "fabric_op_completed_sync_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricAsyncOperationContext, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(BOOLEAN, fabric_op_completed_sync_ctx_IsCompleted), \
        COM_WRAPPER_FUNCTION_WRAPPER(BOOLEAN, fabric_op_completed_sync_ctx_CompletedSynchronously), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, fabric_op_completed_sync_ctx_get_Callback, IFabricAsyncOperationCallback**, callback), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, fabric_op_completed_sync_ctx_Cancel) \
    )

    DECLARE_COM_WRAPPER_OBJECT(FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_OP_COMPLETED_SYNC_CTX_COM_H */
