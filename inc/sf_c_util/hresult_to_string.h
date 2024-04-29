// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef HRESULT_TO_STRING_H
#define HRESULT_TO_STRING_H

#ifdef __cplusplus
/*in C, wchar_t comes from stddef.h, in C++ it is a keyword*/
#else
#include <stddef.h>
#endif

#include "windows.h"

typedef void* (*malloc_t)(size_t);
typedef void (*free_t)(void* ptr);

#ifdef __cplusplus
extern "C" {
#endif
    void same_as_free(void* ptr);
    void* same_as_malloc(size_t size);
#ifdef __cplusplus
}
#endif

#include "macro_utils/macro_utils.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*will return a string that has some human understandable meaning
    will return NULL if it cannot determine the meaning (or it fails internally)
    in order will try to find the meaning of the HRESULT as follows:
    1) system (as defined in FORMAT_MESSAGE_FROM_SYSTEM)
    2) service fabric's code
    3) every other loaded module

    caller needs to dispose (call free()) of the string (note the missing "const" from the return value)
    */
    char* hresult_to_string(malloc_t the_malloc, free_t the_free, HRESULT hresult);
    wchar_t* hresult_to_wstring(HRESULT hresult);

/*the below macro is just like LogError, but it will print "HRESULT="some string" if the string exists*/
#define LogHRESULTWithFormat(log_macro, hr, FORMAT, ...)                                                                                                                                              \
    do                                                                                                                                                                                          \
    {                                                                                                                                                                                           \
        char* hresult_to_string_result = hresult_to_string(same_as_malloc, same_as_free, hr);                                                                                                   \
        if (hresult_to_string_result != NULL)                                                                                                                                                   \
        {                                                                                                                                                                                       \
            log_macro(FORMAT " HRESULT=\"%s\"", __VA_ARGS__ MU_IFCOMMALOGIC(MU_COUNT_ARG(__VA_ARGS__)) hresult_to_string_result);                                                                \
            same_as_free(hresult_to_string_result);                                                                                                                                             \
        }                                                                                                                                                                                       \
        else                                                                                                                                                                                    \
        {                                                                                                                                                                                       \
            log_macro(FORMAT " HRESULT=(error or no module can provide it Facility=0x%x Code=0x%x)" , __VA_ARGS__ MU_IFCOMMALOGIC(MU_COUNT_ARG(__VA_ARGS__)) HRESULT_FACILITY(hr),HRESULT_CODE(hr));   \
        }                                                                                                                                                                                       \
    }                                                                                                                                                                                           \
    while(0)

#define LogHRESULTWithoutFormat(log_macro, hr)                                                                                                                                    \
    do                                                                                                                                                                          \
    {                                                                                                                                                                           \
        char* hresult_to_string_result = hresult_to_string(same_as_malloc, same_as_free, hr);                                                                                   \
        if (hresult_to_string_result != NULL)                                                                                                                                   \
        {                                                                                                                                                                       \
            log_macro("HRESULT=\"%s\"", hresult_to_string_result);                                                                                                               \
            same_as_free(hresult_to_string_result);                                                                                                                                     \
        }                                                                                                                                                                       \
        else                                                                                                                                                                    \
        {                                                                                                                                                                       \
            log_macro("HRESULT=(error or no module can provide it Facility=0x%x Code=0x%x)", HRESULT_FACILITY(hr), HRESULT_CODE(hr));                                            \
        }                                                                                                                                                                       \
    }                                                                                                                                                                           \
    while(0)

#if _MSC_VER>=1920 /*VS 2019*/
    // This macro is a wrapper on top of other logging macros
    // It can be used like: LogHRESULT(LogInfo, hr, "foo")
#define LogHRESULT(log_macro, hr, ...) do{MU_IF(MU_COUNT_ARG(__VA_ARGS__), LogHRESULTWithFormat(log_macro, hr, __VA_ARGS__);,  LogHRESULTWithoutFormat (log_macro, hr);)}while(0)
#else /*VS 2017*/
    // This macro is a wrapper on top of other logging macros
    // It can be used like: LogHRESULT(LogInfo, hr, "foo")
#define LogHRESULT(log_macro, hr, ...) do{MU_IF(MU_COUNT_ARG(__VA_ARGS__), LogHRESULTWithFormat (log_macro, hr, __VA_ARGS__);,  LogHRESULTWithoutFormat (log_macro, hr);)}while(0)
#endif /*VS 2017/2019*/

#define LogHRESULTError(hr, ...)        LogHRESULT(LogError, hr, __VA_ARGS__)
#define LogHRESULTInfo(hr, ...)         LogHRESULT(LogInfo, hr, __VA_ARGS__)
#define LogHRESULTVerbose(hr, ...)      LogHRESULT(LogVerbose, hr, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
