// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_FABRIC_STRING_RESULT_H
#define REAL_FABRIC_STRING_RESULT_H

#include <stdint.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_FABRIC_STRING_RESULT_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        fabric_string_result_create, \
        fabric_string_result_destroy,   \
        fabric_string_result_get_String \
)

#include "sf_c_util/fabric_string_result.h"


FABRIC_STRING_RESULT_HANDLE real_fabric_string_result_create(const wchar_t* result_string);
void real_fabric_string_result_destroy(FABRIC_STRING_RESULT_HANDLE fabric_string_result);
LPCWSTR real_fabric_string_result_get_String(FABRIC_STRING_RESULT_HANDLE fabric_string_result);

#endif //REAL_FABRIC_STRING_RESULT_H
