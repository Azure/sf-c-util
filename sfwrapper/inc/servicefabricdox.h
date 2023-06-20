// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef SERVICEFABRICDOX_H
#define SERVICEFABRICDOX_H

#include "winerror.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"

#include "mycallback.h"
#include "sf_c_util/hresult_to_string.h"

template<class I, typename Begin, typename End, typename TResult, typename... TArgs>
HRESULT ServiceFabric_DoX(
    I* client,
    Begin begin,
    End end,
    DWORD timeoutMilliseconds,
    TResult output,
    TArgs... targs
    )
{
    HRESULT result;
    HANDLE ev;
    if ((ev = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
    {
        result = HRESULT_FROM_WIN32(GetLastError());
        LogLastError("failure in CreateEvent");
    }
    else
    {
        IFabricAsyncOperationCallback* callback = IFabricAsyncOperationCallback_do_nothing_create(ev);
        if (callback == NULL)
        {
            LogError("failure in IFabricAsyncOperationCallback_do_nothing_create");
            result = E_FAIL;
        }
        else
        {
            IFabricAsyncOperationContext* context;
            result = (client->*begin)(targs..., timeoutMilliseconds, callback, &context);
            if (FAILED(result))
            {
                LogHRESULTError(result, "failure in begin");
                /*return as is*/
            }
            else
            {
                if (context->CompletedSynchronously())
                {
                    result = (client->*end)(context, output);
                    if (FAILED(result))
                    {
                        LogHRESULTError(result, "failure in end");
                        /*return as is*/
                    }
                    else
                    {
                        result = S_OK;
                    }
                }
                else
                {
                    if (WaitForSingleObject(ev, INFINITE) != WAIT_OBJECT_0)
                    {
                        LogLastError("failure in WaitForSingleObject");
                        result = MU_FAILURE;
                    }
                    else
                    {
                        result = (client->*end)(context, output);
                        if (FAILED(result))
                        {
                            LogHRESULTError(result, "failure in end");
                            /*return as is*/
                        }
                        else
                        {
                            result = S_OK;
                        }
                    }
                }
                (void)context->Release();
            }
            (void)callback->Release();
        }
        (void)CloseHandle(ev);
    }
    return result;
}

template<class I, typename Begin, typename End, typename... TArgs>
HRESULT ServiceFabric_DoX_NoResult(
    I* client,
    Begin begin,
    End end,
    DWORD timeoutMilliseconds,
    TArgs... targs
)
{
    HRESULT result;
    HANDLE ev;
    if ((ev = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
    {
        result = HRESULT_FROM_WIN32(GetLastError());
        LogLastError("failure in CreateEvent");
    }
    else
    {
        IFabricAsyncOperationCallback* callback = IFabricAsyncOperationCallback_do_nothing_create(ev);
        if (callback == NULL)
        {
            LogError("failure in IFabricAsyncOperationCallback_do_nothing_create");
            result = E_FAIL;
        }
        else
        {
            IFabricAsyncOperationContext* context;
            result = (client->*begin)(targs..., timeoutMilliseconds, callback, &context);
            if (FAILED(result))
            {
                LogHRESULTError(result, "failure in begin");
                /*return as is*/
            }
            else
            {
                if (context->CompletedSynchronously())
                {
                    result = (client->*end)(context);
                    if (FAILED(result))
                    {
                        LogHRESULTError(result, "failure in end");
                        /*return as is*/
                    }
                    else
                    {
                        result = S_OK;
                    }
                }
                else
                {
                    if (WaitForSingleObject(ev, INFINITE) != WAIT_OBJECT_0)
                    {
                        LogLastError("failure in WaitForSingleObject");
                        result = MU_FAILURE;
                    }
                    else
                    {
                        result = (client->*end)(context);
                        if (FAILED(result))
                        {
                            LogHRESULTError(result, "failure in end");
                            /*return as is*/
                        }
                        else
                        {
                            result = S_OK;
                        }
                    }
                }
                (void)context->Release();
            }
            (void)callback->Release();
        }
        (void)CloseHandle(ev);
    }
    return result;
}

#endif /*SERVICEFABRICDOX_H*/
