// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "windows.h"


#include "fabriccommon.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fabric_op_completed_sync_ctx.h"

typedef struct FABRIC_OP_COMPLETED_SYNC_CTX_TAG
{
    IFabricAsyncOperationCallback* callback;
} FABRIC_OP_COMPLETED_SYNC_CTX;

FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx_create(IFabricAsyncOperationCallback* callback)
{
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE result;

    if (callback == NULL)
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_001: [ If callback is NULL, fabric_op_completed_sync_ctx_create shall fail and return NULL. ]*/
        LogError("Invalid arguments: IFabricAsyncOperationCallback* callback=%p", callback);
    }
    else
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_002: [ Otherwise fabric_op_completed_sync_ctx_create shall allocate a new fabric async operation context and on success return a non-NULL pointer to it. ]*/
        result = malloc(sizeof(FABRIC_OP_COMPLETED_SYNC_CTX));
        if (result == NULL)
        {
            /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_004: [ If any error occurs, fabric_op_completed_sync_ctx_create shall fail and return NULL. ]*/
            LogError("malloc failed");
        }
        else
        {
            /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_003: [ fabric_op_completed_sync_ctx_create shall increment the reference count for callback and store it. ]*/
            (void)callback->lpVtbl->AddRef(callback);
            result->callback = callback;

            goto all_ok;
        }
    }

    result = NULL;

all_ok:
    return result;
}

void fabric_op_completed_sync_ctx_destroy(FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx)
{
    if (fabric_op_completed_sync_ctx == NULL)
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_005: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_destroy shall return. ]*/
        LogError("Invalid arguments: FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = %p", fabric_op_completed_sync_ctx);
    }
    else
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_006: [ Otherwise, fabric_op_completed_sync_ctx_destroy shall decrement the reference count for the callback passed to fabric_op_completed_sync_ctx_create. ]*/
        (void)fabric_op_completed_sync_ctx->callback->lpVtbl->Release(fabric_op_completed_sync_ctx->callback);
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_007: [ fabric_op_completed_sync_ctx_destroy shall free the memory associated with fabric_op_completed_sync_ctx. ]*/
        free(fabric_op_completed_sync_ctx);
    }
}

BOOLEAN fabric_op_completed_sync_ctx_IsCompleted(FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx)
{
    BOOLEAN result;

    if (fabric_op_completed_sync_ctx == NULL)
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_008: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_IsCompleted shall return FALSE. ]*/
        LogError("Invalid arguments: FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = %p", fabric_op_completed_sync_ctx);
        result = FALSE;
    }
    else
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_009: [ Otherwise fabric_op_completed_sync_ctx_IsCompleted shall always return TRUE. ]*/
        result = TRUE;
    }

    return result;
}

BOOLEAN fabric_op_completed_sync_ctx_CompletedSynchronously(FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx)
{
    BOOLEAN result;

    if (fabric_op_completed_sync_ctx == NULL)
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_010: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_CompletedSynchronously shall return FALSE. ]*/
        LogError("Invalid arguments: FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = %p", fabric_op_completed_sync_ctx);
        result = FALSE;
    }
    else
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_011: [ Otherwise fabric_op_completed_sync_ctx_CompletedSynchronously shall return TRUE. ]*/
        result = TRUE;
    }

    return result;
}

HRESULT fabric_op_completed_sync_ctx_get_Callback(FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx, IFabricAsyncOperationCallback** callback)
{
    HRESULT result;

    if (fabric_op_completed_sync_ctx == NULL)
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_012: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_get_Callback shall return E_FAIL. ]*/
        LogError("Invalid arguments: FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = %p", fabric_op_completed_sync_ctx);
        result = E_FAIL;
    }
    else if (callback == NULL)
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_013: [ If callback is NULL, fabric_op_completed_sync_ctx_get_Callback shall return E_POINTER. ]*/
        LogError("Invalid arguments: FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = %p", fabric_op_completed_sync_ctx);
        result = E_POINTER;
    }
    else
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_014: [ Otherwise fabric_op_completed_sync_ctx_get_Callback shall increment the ref count for the callback passed as argument to fabric_op_completed_sync_ctx_create. ]*/
        (void)fabric_op_completed_sync_ctx->callback->lpVtbl->AddRef(fabric_op_completed_sync_ctx->callback);

        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_015: [ fabric_op_completed_sync_ctx_get_Callback shall return in callback the callback passed as argument to fabric_op_completed_sync_ctx_create. ]*/
        *callback = fabric_op_completed_sync_ctx->callback;

        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_016: [ On success, fabric_op_completed_sync_ctx_get_Callback shall return S_OK. ]*/
        result = S_OK;
    }

    return result;
}

HRESULT fabric_op_completed_sync_ctx_Cancel(FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx)
{
    HRESULT result;

    if (fabric_op_completed_sync_ctx == NULL)
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_017: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_Cancel shall return E_FAIL. ]*/
        LogError("Invalid arguments: FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = %p", fabric_op_completed_sync_ctx);
        result = E_FAIL;
    }
    else
    {
        /* Codes_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_018: [ Otherwise fabric_op_completed_sync_ctx shall return E_FAIL as a completed operation cannot be cancelled. ]*/
        result = E_FAIL;
    }

    return result;
}
