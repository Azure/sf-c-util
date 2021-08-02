// Copyright (c) Microsoft. All rights reserved.

#include "windows.h"

#include "ifabriczzzz.h"
#include "ifabriczzzz_sync.h"

#include "hfabriczzzz.h"

H_FABRIC_DEFINE_STRUCT_TYPE(IFabricZZZZ);

H_FABRIC_DEFINE_CREATE(IFabricZZZZ);
H_FABRIC_DEFINE_DESTROY(IFabricZZZZ);

H_FABRIC_DEFINE_API(IFabricZZZZ, DoSomethingAwesome,
    IN_ARGS(
        ARG(const char*, queryDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API_NO_SF_TIMEOUT(IFabricZZZZ, DoSomethingAwesomeNoSFTimeout,
    IN_ARGS(
        ARG(const char*, queryDescription)
    )
)

H_FABRIC_DEFINE_API_WITH_RESULTS(IFabricZZZZ, DoSomethingWithPossibleFailures,
    IN_ARGS(
        ARG(const char*, queryDescription),
        ARG(DWORD, timeoutMilliseconds)
    ),
    RESULTS(FABRIC_E_INVALID_ADDRESS, FABRIC_E_INVALID_NAME_URI)
)

H_FABRIC_DEFINE_API_NO_SF_TIMEOUT_WITH_RESULTS(IFabricZZZZ, DoSomethingWithPossibleFailuresNoSFTimeout,
    IN_ARGS(
        ARG(const char*, queryDescription)
    ),
    RESULTS(FABRIC_E_NAME_NOT_EMPTY)
)
