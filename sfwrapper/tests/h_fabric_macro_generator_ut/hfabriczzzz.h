// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef HFABRICZZZZ_H
#define HFABRICZZZZ_H

#include "windows.h"

#include "sf_macros.h"

#include "ifabriczzzz.h"
#include "ifabriczzzz_sync.h"
#include "h_fabric_macro_generator.h"

H_FABRIC_DEFINE_TYPEDEF(IFabricZZZZ);

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    H_FABRIC_DECLARE_CREATE(IFabricZZZZ);
    H_FABRIC_DECLARE_DESTROY(IFabricZZZZ);

    /*below are the same API as ifabriczzzz_sync exposes, which are the same as the methods of the IFabricQueryClient10 interface*/

    H_FABRIC_DECLARE_API(IFabricZZZZ, DoSomethingAwesome,
        IN_ARGS(
            ARG(const char*, queryDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricZZZZ, DoSomethingAwesomeNoSFTimeout,
        IN_ARGS(
            ARG(const char*, queryDescription)
        )
    )

    H_FABRIC_DECLARE_API(IFabricZZZZ, DoSomethingWithPossibleFailures,
        IN_ARGS(
            ARG(const char*, queryDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricZZZZ, DoSomethingWithPossibleFailuresNoSFTimeout,
        IN_ARGS(
            ARG(const char*, queryDescription)
        )
    )

#ifdef __cplusplus
}
#endif


#endif /*HFABRICZZZZ_H*/
