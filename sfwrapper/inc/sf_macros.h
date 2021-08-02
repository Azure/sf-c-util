// Copyright (C) Microsoft Corporation. All rights reserved.

/*this file defines several macros that are used to create IFabricSomething* objects and run SF APIs on those objects*/
/*the reason is: when an HRESULT indicates FABRIC_E_OBJECT_CLOSED - the object gets in weird state and it needs to be disposed and re-created */

#ifndef SF_MACROS_H
#define SF_MACROS_H

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/xlogging.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"

#define CREATE_IFABRICINSTANCE_NAME(IFabricType) MU_C2(CREATE_, IFabricType)

/*the below macro expands to something that is the implementation of this helper in a .c file*/
#define DEFINE_CREATE_IFABRICINSTANCE(IFabricType /*such as IFabricApplicationManagementClient10*/)                                                                                                 \
HRESULT CREATE_IFABRICINSTANCE_NAME(IFabricType)(IFabricType** fabricVariable)                                                                                                                                   \
{                                                                                                                                                                                                   \
    HRESULT hr;                                                                                                                                                                                     \
    if (fabricVariable == NULL)                                                                                                                                                                     \
    {                                                                                                                                                                                               \
        LogError("invalid argument fabricVariable=%p", fabricVariable);                                                                                                                             \
        hr = E_POINTER;                                                                                                                                                                             \
    }                                                                                                                                                                                               \
    else                                                                                                                                                                                            \
    {                                                                                                                                                                                               \
        /*clean it if needed*/                                                                                                                                                                      \
        if (*fabricVariable != NULL)                                                                                                                                                                \
        {                                                                                                                                                                                           \
            LogInfo("cleaning *fabricVariable=%p", *fabricVariable);                                                                                                                                \
            (void)((IFabricType*)*fabricVariable)->lpVtbl->Release((IFabricType*)*fabricVariable);                                                                                                  \
        }                                                                                                                                                                                           \
                                                                                                                                                                                                    \
        /*re-create in place*/                                                                                                                                                                      \
        hr = FabricCreateLocalClient(&MU_C2(IID_, IFabricType), fabricVariable); /*all IFabricWhateverSomethingClient are created with this call, passing a different interface IID*/               \
        if (FAILED(hr))                                                                                                                                                                             \
        {                                                                                                                                                                                           \
            LogHRESULTError(hr, "failure in FabricCreateLocalClient");                                                                                                                              \
            /*return as is*/                                                                                                                                                                        \
        }                                                                                                                                                                                           \
        else                                                                                                                                                                                        \
        {                                                                                                                                                                                           \
            /*well - it seems to have been created "in place", return hr "as is"*/                                                                                                                  \
        }                                                                                                                                                                                           \
    }                                                                                                                                                                                               \
    return hr;                                                                                                                                                                                      \
}                                                                                                                                                                                                   \


/*the below macro expands to something that is the definition of the helper in a .h file*/
#define DECLARE_CREATE_IFABRICINSTANCE(IFabricType)                                                                                                                                                 \
HRESULT CREATE_IFABRICINSTANCE_NAME(IFabricType)(IFabricType** fabricVariable);

/*the below macro expands to something that is usable in a .c file belonging to the user - that is - a function call*/
#define CREATE_IFABRICINSTANCE(IFabricType, fabricVariable)                                                                                                                                         \
CREATE_IFABRICINSTANCE_NAME(IFabricType)(fabricVariable)

#endif /*#define SF_MACROS_H*/
