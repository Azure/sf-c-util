// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef IFABRICZZZZSYNC_H
#define IFABRICZZZZSYNC_H

#include "windows.h"

#include "sf_macros.h"

#include "ifabriczzzz.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    MOCKABLE_FUNCTION(, HRESULT, DoSomethingAwesome,
        IFabricZZZZ*, This,
        /* [in] */ const char*, queryDescription,
        /* [in] */ DWORD, timeoutMilliseconds
        );

    MOCKABLE_FUNCTION(, HRESULT, DoSomethingAwesomeNoSFTimeout,
        IFabricZZZZ*, This,
        /* [in] */ const char*, queryDescription
    );

    MOCKABLE_FUNCTION(, HRESULT, DoSomethingWithPossibleFailures,
        IFabricZZZZ*, This,
        /* [in] */ const char*, queryDescription,
        /* [in] */ DWORD, timeoutMilliseconds
        );

    MOCKABLE_FUNCTION(, HRESULT, DoSomethingWithPossibleFailuresNoSFTimeout,
        IFabricZZZZ*, This,
        /* [in] */ const char*, queryDescription
        );

    MOCKABLE_FUNCTION(, HRESULT, CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ), IFabricZZZZ**, fabricVariable);

#ifdef __cplusplus
}
#endif


#endif /*IFABRICZZZZSYNC_H*/

