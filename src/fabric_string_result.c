// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stddef.h>
#include <wchar.h>

#include "windows.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fabric_string_result.h"

typedef struct FABRIC_STRING_RESULT_TAG
{
    wchar_t string_result[];
} FABRIC_STRING_RESULT;

FABRIC_STRING_RESULT_HANDLE fabric_string_result_create(const wchar_t* string_result)
{
    FABRIC_STRING_RESULT_HANDLE result;

    if (string_result == NULL)
    {
        /* Codes_SRS_FABRIC_STRING_RESULT_01_008: [ If string_result is NULL, fabric_string_result_create shall fail and return NULL. ]*/
        LogError("Invalid arguments: const wchar_t* string_result=%ls", MU_WP_OR_NULL(string_result));
    }
    else
    {
        size_t string_length = wcslen(string_result);
        if (string_length > (SIZE_MAX - sizeof(FABRIC_STRING_RESULT)) / sizeof(wchar_t))
        {
            /* Codes_SRS_FABRIC_STRING_RESULT_01_003: [ If any error occurs, fabric_string_result_create shall fail and return NULL. ]*/
            LogError("Overflow in needed memory computation");
        }
        else
        {
            // one for the zero terminator
            string_length++;

            size_t needed_memory = sizeof(FABRIC_STRING_RESULT) + (sizeof(wchar_t) * string_length);

            /* Codes_SRS_FABRIC_STRING_RESULT_01_001: [ fabric_string_result_create shall allocate a new fabric string result instance and on success return a non-NULL pointer to it. ]*/
            result = malloc(needed_memory);
            if (result == NULL)
            {
                /* Codes_SRS_FABRIC_STRING_RESULT_01_003: [ If any error occurs, fabric_string_result_create shall fail and return NULL. ]*/
                LogError("malloc(%zu) failed", needed_memory);
            }
            else
            {
                /* Codes_SRS_FABRIC_STRING_RESULT_01_002: [ fabric_string_result_create shall copy the string_result string so it can be returned by get_String. ]*/
                (void)memcpy(result->string_result, string_result, sizeof(wchar_t) * (string_length));
                goto all_ok;
            }
        }
    }

    result = NULL;

all_ok:
    return result;
}

void fabric_string_result_destroy(FABRIC_STRING_RESULT_HANDLE fabric_string_result)
{
    if (fabric_string_result == NULL)
    {
        /* Codes_SRS_FABRIC_STRING_RESULT_01_004: [ If fabric_string_result is NULL, fabric_string_result_destroy shall return. ]*/
        LogError("Invalid arguments: FABRIC_STRING_RESULT_HANDLE fabric_string_result=%p", fabric_string_result);
    }
    else
    {
        /* Codes_SRS_FABRIC_STRING_RESULT_01_005: [ Otherwise, fabric_string_result_destroy shall free the memory associated with the fabric string result instance. ]*/
        free(fabric_string_result);
    }
}

LPCWSTR fabric_string_result_get_String(FABRIC_STRING_RESULT_HANDLE fabric_string_result)
{
    LPCWSTR result;

    if (fabric_string_result == NULL)
    {
        /* Codes_SRS_FABRIC_STRING_RESULT_01_006: [ If fabric_string_result is NULL, fabric_string_result_get_String shall fail and return NULL. ]*/
        LogError("Invalid arguments: FABRIC_STRING_RESULT_HANDLE fabric_string_result=%p", fabric_string_result);
        result = NULL;
    }
    else
    {
        /* Codes_SRS_FABRIC_STRING_RESULT_01_007: [ Otherwise, fabric_string_result_get_String shall return a pointer to the string copied in fabric_string_result_create. ]*/
        result = fabric_string_result->string_result;
    }

    return result;
}
