// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_STRING_LIST_RESULT_H
#define FABRIC_STRING_LIST_RESULT_H

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#include "windows.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct FABRIC_STRING_LIST_RESULT_TAG* FABRIC_STRING_LIST_RESULT_HANDLE;

    MOCKABLE_FUNCTION(, FABRIC_STRING_LIST_RESULT_HANDLE, fabric_string_list_result_create, ULONG, nStrings, const wchar_t**, strings);
    MOCKABLE_FUNCTION(, void, fabric_string_list_result_destroy, FABRIC_STRING_LIST_RESULT_HANDLE, fabric_string_list_result);
    MOCKABLE_FUNCTION(, HRESULT, fabric_string_result_GetStrings, FABRIC_STRING_LIST_RESULT_HANDLE, fabric_string_list_result, ULONG*, nStrings, wchar_t***, strings);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_STRING_LIST_RESULT_H */
