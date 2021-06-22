// Copyright (C) Microsoft Corporation. All rights reserved.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "windows.h"
#include "psapi.h"

/*below 2 functions need to appear before malloc is redefined to be gballoc_malloc*/
void* same_as_malloc(size_t size)
{
    return malloc(size);
}

void same_as_free(void* ptr)
{
    free(ptr);
}

#include "macro_utils/macro_utils.h"
#include "c_logging/xlogging.h"

#include "servicefabric_enums_to_strings.h"

#include "hresult_to_string.h"

#define N_MAX_MODULES 100
#define N_MAX_CHARACTERS 1000

char* hresult_to_string(malloc_t the_malloc, free_t the_free, HRESULT hr)
{
    char* result;

    if (
        /*Codes_SRS_HRESULT_TO_STRING_02_018: [ If the_malloc is NULL then hresult_to_string shall fail and return NULL. ]*/
        (the_malloc == NULL) ||
        /*Codes_SRS_HRESULT_TO_STRING_02_017: [ If the_free is NULL then hresult_to_string shall fail and return NULL. ]*/
        (the_free == NULL)
        )
    {
        LogError("invalid arguments malloc_t the_malloc=%p, free_t the_free=%p, HRESULT hr=0x%x", the_malloc, the_free, hr);
        result = NULL;
    }
    else
    {
        /*allocate a buffer "large enough"*/
    /*Codes_SRS_HRESULT_TO_STRING_02_001: [ hresult_to_string shall allocate memory for a "big enough" string representation. ]*/
        result = the_malloc(N_MAX_CHARACTERS * sizeof(char));
        if (result == NULL)
        {
            /*Codes_SRS_HRESULT_TO_STRING_02_008: [ If there are any failures then hresult_to_string shall return NULL. ]*/
            LogError("failure in malloc");
            /*return as is*/
        }
        else
        {
            result[0] = '\0';
            /*Codes_SRS_HRESULT_TO_STRING_02_002: [ hresult_to_string shall call FormatMessageA with dwFlags set to FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS. ]*/
            /*see if the "system" can provide the code*/
            if (FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                hr,
                0, /*if you pass in zero, FormatMessage looks for a message for LANGIDs in the following order...*/
                (LPVOID)result, N_MAX_CHARACTERS, NULL) != 0)
            {
                /*Codes_SRS_HRESULT_TO_STRING_02_003: [ If FormatMessageA succeeds then hresult_to_string shall return the value as given by FormatMessageA. ]*/
                /*success, SYSTEM was able to find the message*/
                /*return as is*/
                goto allok;
            }
            else
            {
                /*Codes_SRS_HRESULT_TO_STRING_02_004: [ Otherwise, hresult_to_string shall look in all Service Fabric code for a match and return that match. ]*/
                /*look in ServiceFabric codes*/
                const char* maybeSFhasIt = MU_ENUM_TO_STRING(FABRIC_ERROR_CODE, hr);
                if (strcmp(maybeSFhasIt, "UNKNOWN") != 0)
                {
                    size_t length = strlen(maybeSFhasIt);
                    if ((length + 1) > N_MAX_CHARACTERS)
                    {
                        LogError("internal buffer would not fit the whole message");
                        goto clean;
                    }
                    else
                    {
                        (void)memcpy(result, maybeSFhasIt, (length + 1) * sizeof(char));
                        goto allok;
                    }
                }

                /*Codes_SRS_HRESULT_TO_STRING_02_005: [ If no Service Fabric codes match hresult then hresult_to_string shall look in all the loaded modules by the current process. ]*/
                /*then maaaaaybe one of the other modules provides it*/

                HANDLE currentProcess = GetCurrentProcess();
                /*apparently this cannot fail and returns somewhat of a "pseudo handle"*/

                HMODULE hModules[N_MAX_MODULES];
                DWORD enumModulesUsedBytes;
                if (EnumProcessModules(currentProcess, hModules, sizeof(hModules), &enumModulesUsedBytes) == 0)
                {
                    /*Codes_SRS_HRESULT_TO_STRING_02_008: [ If there are any failures then hresult_to_string shall return NULL. ]*/
                    LogLastError("failure in EnumProcessModules");
                }
                else
                {
                    size_t iModule;
                    for (iModule = 0; iModule < (enumModulesUsedBytes / sizeof(HMODULE)); iModule++)
                    {
                        char fileName[MAX_PATH];
                        if (GetModuleFileNameA(hModules[iModule], fileName, sizeof(fileName) / sizeof(fileName[0])) == 0)
                        {
                            /*Codes_SRS_HRESULT_TO_STRING_02_008: [ If there are any failures then hresult_to_string shall return NULL. ]*/
                            LogLastError("Failure in GetModuleFileNameA");
                            iModule = enumModulesUsedBytes / sizeof(HMODULE);
                            break;
                        }
                        else
                        {
                            /*see if this module */
                            if (FormatMessageA(
                                FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
                                hModules[iModule],
                                hr,
                                0,
                                (LPVOID)result, N_MAX_CHARACTERS, NULL) != 0)
                            {
                                /*Codes_SRS_HRESULT_TO_STRING_02_006: [ If a module can decode hresult then that value shall be returned. */
                                break;
                            }
                            else
                            {
                                /*this module does not have it...*/
                            }
                        }
                    }

                    /*Codes_SRS_HRESULT_TO_STRING_02_007: [ Otherwise NULL shall be returned. ]*/
                    if (iModule == (enumModulesUsedBytes / sizeof(HMODULE)))
                    {
                        LogError("unknown HRESULT 0x%x", hr);
                    }
                    else
                    {
                        goto allok;
                    }
                }
            }
        clean:
            the_free(result);
            result = NULL;
        allok:;
        }
    }
    return result;
}


wchar_t* hresult_to_wstring(HRESULT hr)
{
    wchar_t* result;

    /*allocate a buffer "large enough"*/
    /*Codes_SRS_HRESULT_TO_STRING_02_009: [ hresult_to_wstring shall allocate memory for a "big enough" string representation. ]*/
    result = malloc(N_MAX_CHARACTERS * sizeof(wchar_t));
    if (result == NULL)
    {
        /*Codes_SRS_HRESULT_TO_STRING_02_016: [ If there are any failures then hresult_to_wstring shall return NULL. ]*/
        LogError("failure in malloc");
        /*return as is*/
    }
    else
    {
        result[0] = L'\0';
        /*Codes_SRS_HRESULT_TO_STRING_02_010: [ hresult_to_wstring shall call FormatMessageW with dwFlags set to FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS. ]*/
        /*see if the "system" can provide the code*/
        if (FormatMessageW(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            hr,
            0, /*if you pass in zero, FormatMessage looks for a message for LANGIDs in the following order...*/
            (LPVOID)result, N_MAX_CHARACTERS, NULL) != 0)
        {
            /*Codes_SRS_HRESULT_TO_STRING_02_011: [ If FormatMessageW succeeds then hresult_to_wstring shall return the value as given by FormatMessageW. ]*/
            /*success, SYSTEM was able to find the message*/
            /*return as is*/
            goto allok;
        }
        else
        {
            /*Codes_SRS_HRESULT_TO_STRING_02_012: [ Otherwise, hresult_to_wstring shall look in all Service Fabric code for a match and return that match. ]*/
            /*look in ServiceFabric codes*/

            const char* maybeSFhasIt = MU_ENUM_TO_STRING(FABRIC_ERROR_CODE, hr);
            if (strcmp(maybeSFhasIt, "UNKNOWN") != 0)
            {
                size_t length = strlen(maybeSFhasIt);
                if ((length + 1) > N_MAX_CHARACTERS)
                {
                    LogError("internal buffer would not fit the whole message");
                    goto clean;
                }
                else
                {
                    (void)mbstowcs(result, maybeSFhasIt, (length + 1) * sizeof(char));
                    goto allok;
                }
            }

            /*Codes_SRS_HRESULT_TO_STRING_02_013: [ If no Service Fabric codes match hresult then hresult_to_wstring shall look in all the loaded modules by the current process. ]*/

            HANDLE currentProcess = GetCurrentProcess();
            /*apparently this cannot fail and returns somewhat of a "pseudo handle"*/

            HMODULE hModules[N_MAX_MODULES];
            DWORD enumModulesUsedBytes;
            if (EnumProcessModules(currentProcess, hModules, sizeof(hModules), &enumModulesUsedBytes) == 0)
            {
                /*Codes_SRS_HRESULT_TO_STRING_02_016: [ If there are any failures then hresult_to_wstring shall return NULL. ]*/
                LogLastError("failure in EnumProcessModules");
            }
            else
            {
                size_t iModule;
                for (iModule = 0; iModule < (enumModulesUsedBytes / sizeof(HMODULE)); iModule++)
                {
                    char fileName[MAX_PATH];
                    if (GetModuleFileNameA(hModules[iModule], fileName, sizeof(fileName) / sizeof(fileName[0])) == 0)
                    {
                        /*Codes_SRS_HRESULT_TO_STRING_02_016: [ If there are any failures then hresult_to_wstring shall return NULL. ]*/
                        LogLastError("Failure in GetModuleFileNameA");
                        iModule = enumModulesUsedBytes / sizeof(HMODULE);
                        break;
                    }
                    else
                    {
                        /*Codes_SRS_HRESULT_TO_STRING_02_014: [ If a module can decode hresult then that value shall be returned. ]*/
                        if (FormatMessageW(
                            FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
                            hModules[iModule],
                            hr,
                            0,
                            (LPVOID)result, N_MAX_CHARACTERS, NULL) != 0)
                        {
                            break;
                        }
                        else
                        {
                            /*this module does not have it...*/
                        }
                    }
                }

                /*Codes_SRS_HRESULT_TO_STRING_02_007: [ Otherwise NULL shall be returned. ]*/
                if (iModule == (enumModulesUsedBytes / sizeof(HMODULE)))
                {
                    LogError("unknown HRESULT 0x%x", hr);
                }
                else
                {
                    goto allok;
                }
            }
        }
clean:
        /*Codes_SRS_HRESULT_TO_STRING_02_015: [ Otherwise NULL shall be returned. ]*/
        free(result);
        result = NULL;
allok:;
    }
    return result;
}
