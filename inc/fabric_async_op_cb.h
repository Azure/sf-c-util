// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_ASYNC_OP_CB_H
#define FABRIC_ASYNC_OP_CB_H


#include "windows.h"
#include "fabriccommon.h"

#include "umock_c/umock_c_prod.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*USER_INVOKE_CB)(void* user_context, IFabricAsyncOperationContext* fabric_async_operation_context);
    typedef struct FABRIC_ASYNC_OP_CB_TAG* FABRIC_ASYNC_OP_CB_HANDLE;

    MOCKABLE_FUNCTION(, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb_create, USER_INVOKE_CB, user_invoke_cb, void*, user_invoke_cb_context);
    MOCKABLE_FUNCTION(, void, fabric_async_op_cb_destroy, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb);
    MOCKABLE_FUNCTION(, void, fabric_async_op_cb_Invoke, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb, IFabricAsyncOperationContext*, context);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_ASYNC_OP_CB_H */
