// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_OP_COMPLETED_SYNC_CTX_H
#define FABRIC_OP_COMPLETED_SYNC_CTX_H


#include "fabriccommon.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct FABRIC_OP_COMPLETED_SYNC_CTX_TAG* FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE;

    MOCKABLE_FUNCTION(, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx_create, IFabricAsyncOperationCallback*, callback);
    MOCKABLE_FUNCTION(, void, fabric_op_completed_sync_ctx_destroy, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
    MOCKABLE_FUNCTION(, BOOLEAN, fabric_op_completed_sync_ctx_IsCompleted, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
    MOCKABLE_FUNCTION(, BOOLEAN, fabric_op_completed_sync_ctx_CompletedSynchronously, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
    MOCKABLE_FUNCTION(, HRESULT, fabric_op_completed_sync_ctx_get_Callback, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx, IFabricAsyncOperationCallback**, callback);
    MOCKABLE_FUNCTION(, HRESULT, fabric_op_completed_sync_ctx_Cancel, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_OP_COMPLETED_SYNC_CTX_H */
