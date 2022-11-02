// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_FABRIC_STRING_LIST_RESULT_H
#define REAL_FABRIC_STRING_LIST_RESULT_H

#include <stdint.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_FABRIC_STRING_LIST_RESULT_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        fabric_string_list_result_create, \
        fabric_string_list_result_destroy,   \
        fabric_string_list_result_GetStrings \
)

#include "sf_c_util/fabric_string_list_result.h"


FABRIC_STRING_LIST_RESULT_HANDLE real_fabric_string_list_result_create(ULONG nStrings, const wchar_t** strings);
void real_fabric_string_list_result_destroy(FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result);
HRESULT real_fabric_string_list_result_GetStrings(FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result, ULONG* nStrings, const LPCWSTR** strings);

#endif //REAL_FABRIC_STRING_LIST_RESULT_H
