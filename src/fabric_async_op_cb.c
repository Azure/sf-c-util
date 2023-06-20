// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "windows.h"

#include "fabriccommon.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fabric_async_op_cb.h"

typedef struct FABRIC_ASYNC_OP_CB_TAG
{
    USER_INVOKE_CB user_invoke_cb;
    void* user_invoke_cb_context;
} FABRIC_ASYNC_OP_CB;

IMPLEMENT_MOCKABLE_FUNCTION(, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb_create, USER_INVOKE_CB, user_invoke_cb, void*, user_invoke_cb_context)
{
    FABRIC_ASYNC_OP_CB_HANDLE result;

    /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_002: [ user_invoke_cb_context shall be allowed to be NULL. ]*/

    if (user_invoke_cb == NULL)
    {
        /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_001: [ If user_invoke_cb is NULL, fabric_async_op_cb_create shall fail and return NULL. ]*/
        LogError("Invalid arguments: USER_INVOKE_CB user_invoke_cb=%p, void* user_invoke_cb_context=%p",
            user_invoke_cb, user_invoke_cb_context);
    }
    else
    {
        /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_003: [ Otherwise, fabric_async_op_cb_create shall allocate a new callback instance and on success return a non-NULL pointer to it. ]*/
        result = malloc(sizeof(FABRIC_ASYNC_OP_CB));
        if (result == NULL)
        {
            /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_004: [ If any error occurs, fabric_async_op_cb_create shall fail and return NULL. ]*/
            LogError("malloc failed");
        }
        else
        {
            result->user_invoke_cb = user_invoke_cb;
            result->user_invoke_cb_context = user_invoke_cb_context;

            goto all_ok;
        }
    }

    result = NULL;

all_ok:
    return result;
}

IMPLEMENT_MOCKABLE_FUNCTION(, void, fabric_async_op_cb_destroy, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb)
{
    if (fabric_async_op_cb == NULL)
    {
        /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_005: [ If fabric_async_op_cb is NULL, fabric_async_op_cb_destroy shall return. ]*/
        LogError("Invalid arguments: FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb=%p", fabric_async_op_cb);
    }
    else
    {
        /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_006: [ Otherwise, fabric_async_op_cb_destroy shall free the memory allocated in fabric_async_op_cb_create. ]*/
        free(fabric_async_op_cb);
    }
}

IMPLEMENT_MOCKABLE_FUNCTION(, void, fabric_async_op_cb_Invoke, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb, IFabricAsyncOperationContext*, context)
{
    if (fabric_async_op_cb == NULL)
    {
        /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_007: [ If fabric_async_op_cb is NULL, fabric_async_op_cb_Invoke shall return. ]*/
        LogError("Invalid arguments: FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb=%p, IFabricAsyncOperationContext* context=%p",
            fabric_async_op_cb, context);
    }
    else
    {
        /* Codes_SRS_FABRIC_ASYNC_OP_CB_01_008: [ Otherwise fabric_async_op_cb_Invoke shall call the callback user_invoke_cb and pass as arguments user_invoke_cb_context and context. ]*/
        fabric_async_op_cb->user_invoke_cb(fabric_async_op_cb->user_invoke_cb_context, context);
    }
}
