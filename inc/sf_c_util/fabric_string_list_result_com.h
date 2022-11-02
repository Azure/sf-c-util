// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_STRING_LIST_RESULT_COM_H
#define FABRIC_STRING_LIST_RESULT_COM_H

#include <stddef.h>

#include "windows.h"
#include "unknwn.h"

#include "fabriccommon.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/fabric_string_list_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FABRIC_STRING_LIST_RESULT_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricStringListResult, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, fabric_string_list_result_GetStrings, ULONG*, nstrings, const LPCWSTR **, strings) \
    )

    DECLARE_COM_WRAPPER_OBJECT(FABRIC_STRING_LIST_RESULT_HANDLE, FABRIC_STRING_LIST_RESULT_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_STRING_LIST_RESULT_COM_H */
