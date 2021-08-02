// Copyright (C) Microsoft Corporation. All rights reserved.

#include <stdint.h>

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/interlocked.h"

#include "mycallback.h"

typedef struct MyFabricAsyncOperationCallback_TAG {
    IFabricAsyncOperationCallbackVtbl *lpVtbl;
    volatile_atomic int32_t count;
    HANDLE ev;
} MyFabricAsyncOperationCallback;

static HRESULT STDMETHODCALLTYPE QueryInterface(IFabricAsyncOperationCallback *this,
    REFIID vTableGuid, void **ppv)
{
    if (IsEqualIID(vTableGuid, &IID_IFabricAsyncOperationCallback))
    {
        *ppv = 0;
        return E_NOINTERFACE;
    }

    *ppv = this;

    (void)this->lpVtbl->AddRef(this);

    return S_OK;
}

static ULONG STDMETHODCALLTYPE AddRef(IFabricAsyncOperationCallback *this)
{
    MyFabricAsyncOperationCallback* impl = (MyFabricAsyncOperationCallback*)this;
    ULONG result = interlocked_increment(&impl->count);
    return result;
}

static ULONG STDMETHODCALLTYPE Release(IFabricAsyncOperationCallback *this)
{
    MyFabricAsyncOperationCallback* impl = (MyFabricAsyncOperationCallback*)this;
    int32_t count = interlocked_decrement(&impl->count);
    if (count == 0)
    {
        free(this);
    }
    return count;
}

static void STDMETHODCALLTYPE Invoke(IFabricAsyncOperationCallback * This, IFabricAsyncOperationContext *context)
{
    
    MyFabricAsyncOperationCallback* obj = (MyFabricAsyncOperationCallback*)This;
    if (context->lpVtbl->CompletedSynchronously(context))
    {
        //spurious callback, result will be picked in the caller because it examines CompletedSynchronously too.
    }
    else
    {
        if (!SetEvent(obj->ev))
        {
            LogLastError("failure in SetEvent");
        }
    }
}

static IFabricAsyncOperationCallbackVtbl myIFabricAsyncOperationCallbackVtbl = 
{
    QueryInterface,
    AddRef,
    Release,
    Invoke
};

IFabricAsyncOperationCallback* IFabricAsyncOperationCallback_do_nothing_create(HANDLE ev)
{
    MyFabricAsyncOperationCallback* result;
    if (ev == NULL)
    {
        LogError("invalud argument HANDLE ev=%p", ev);
        result = NULL;
    }
    else
    {
        result = malloc(sizeof(MyFabricAsyncOperationCallback));
        if (result == NULL)
        {
            LogError("failure in malloc");
            /*return as is*/
        }
        else
        {
            result->lpVtbl = &myIFabricAsyncOperationCallbackVtbl;
            (void)interlocked_exchange(&result->count, 1);
            result->ev = ev;
        }
    }
    return (IFabricAsyncOperationCallback*)result;
}
