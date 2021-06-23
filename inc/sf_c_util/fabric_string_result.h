// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_STRING_RESULT_H
#define FABRIC_STRING_RESULT_H

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

    typedef struct FABRIC_STRING_RESULT_TAG* FABRIC_STRING_RESULT_HANDLE;

    MOCKABLE_FUNCTION(, FABRIC_STRING_RESULT_HANDLE, fabric_string_result_create, const wchar_t*, result_string);
    MOCKABLE_FUNCTION(, void, fabric_string_result_destroy, FABRIC_STRING_RESULT_HANDLE, fabric_string_result);
    MOCKABLE_FUNCTION(, LPCWSTR, fabric_string_result_get_String, FABRIC_STRING_RESULT_HANDLE, fabric_string_result);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_STRING_RESULT_H */
