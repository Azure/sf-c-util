// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_STRING_RESULT_COM_H
#define FABRIC_STRING_RESULT_COM_H


#include "windows.h"
#include "fabriccommon.h"
#include "unknwn.h"
#include "com_wrapper/com_wrapper.h"
#include "fabric_string_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FABRIC_STRING_RESULT_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricStringResult, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, fabric_string_result_get_String) \
    )

    DECLARE_COM_WRAPPER_OBJECT(FABRIC_STRING_RESULT_HANDLE, FABRIC_STRING_RESULT_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_STRING_RESULT_COM_H */
