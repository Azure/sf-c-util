// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_HRESULT_TO_STRING_H
#define REAL_HRESULT_TO_STRING_H

#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
#else
#include <stdint.h>
#include <stddef.h>
#endif

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_HRESULT_TO_STRING_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        hresult_to_string, \
        hresult_to_wstring,   \
        same_as_free,   \
        same_as_malloc \
)

#include "sf_c_util/hresult_to_string.h"

#ifdef __cplusplus
extern "C" {
#endif

    char* real_hresult_to_string(malloc_t the_malloc, free_t the_free, HRESULT hr);
    wchar_t* real_hresult_to_wstring(HRESULT hresult);
    void real_same_as_free(void* ptr);
    void* real_same_as_malloc(size_t size);
    

#ifdef __cplusplus
}
#endif

#endif //REAL_HRESULT_TO_STRING_H
