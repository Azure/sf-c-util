// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stddef.h>
#include <wchar.h>
#include <inttypes.h>
#include <stdbool.h>

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/string_utils.h"
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fabric_string_list_result.h"

typedef struct FABRIC_STRING_LIST_RESULT_TAG
{
    ULONG nstrings;
    wchar_t* strings[];
} FABRIC_STRING_LIST_RESULT;

FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result_create(ULONG nStrings, const wchar_t** strings)
{
    FABRIC_STRING_LIST_RESULT_HANDLE result;

    if (strings == NULL)
    {
        LogError("invalid argument ULONG nStrings=%" PRIu32 ", const wchar_t** strings=%p",
            nStrings, strings);
        result = NULL;
    }
    else
    {
        result = malloc_flex(sizeof(FABRIC_STRING_LIST_RESULT), nStrings, sizeof(wchar_t*));
        if (result == NULL)
        {
            LogError("failure in malloc_flex");
        }
        else
        {
            ULONG i;
            bool wasError = false;
            for (i = 0; !wasError && (i < nStrings); i++)
            {
                result->strings[i] = sprintf_wchar(L"%ls", strings[i]); /*can be made missing with enough smartnesses*/
                if (result->strings[i] == NULL)
                {
                    LogError("failire in sprintf_wchar");
                    wasError = true;
                }
                else
                {
                    /*keep going*/
                }
            }

            if (wasError)
            {
                ULONG j;
                for (j = 0; j < i; j++)
                {
                    free(result->strings[j]);
                }
            }
            else
            {
                goto allok;
            }



            free(result);
            result = NULL;
        }
    }
allok:;
    return result;
}

void fabric_string_list_result_destroy(FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result)
{
    if (fabric_string_list_result == NULL)
    {
        LogError("Invalid arguments: FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result=%p", fabric_string_list_result);
    }
    else
    {
        for (ULONG i = 0; i < fabric_string_list_result->nstrings; i++)
        {
            free(fabric_string_list_result->strings[i]);
        }
        free(fabric_string_list_result);
    }
}

HRESULT fabric_string_result_GetStrings(FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result, ULONG* nStrings, wchar_t*** strings)
{
    HRESULT result;

    if (
        (fabric_string_list_result == NULL)||
        (nStrings == NULL) ||
        (strings == NULL)
        )
    {
        LogError("Invalid arguments FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result=%p, ULONG* nStrings=%p, wchar_t*** strings=%p",
            fabric_string_list_result, nStrings, strings);
        result = E_INVALIDARG;
    }
    else
    {
        *nStrings = fabric_string_list_result->nstrings;
        *strings = fabric_string_list_result->strings;
        result = S_OK;
    }

    return result;
}
