// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>
#include <stdbool.h>

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/fabric_configuration_parameter_argc_argv.h"

int FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV(const FABRIC_CONFIGURATION_PARAMETER_LIST* fabric_configuration_parameter_list , int* argc, char*** argv)
{
    int result;
    if (
        (fabric_configuration_parameter_list == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument const FABRIC_CONFIGURATION_PARAMETER_LIST* fabric_configuration_parameter_list=%p, int* argc=%p, char*** argv=%p",
            fabric_configuration_parameter_list, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        if (fabric_configuration_parameter_list->Count == 0)
        {
            /*special case because malloc(0) miiight return "something" or "NULL"*/
            *argc = 0;
            *argv = NULL;
            result = 0;
        }
        else
        {
            *argc = 0;
            *argv = NULL;
            bool was_error = false;
            for (ULONG u = 0; !was_error && u < fabric_configuration_parameter_list->Count; u++)
            {
                int g_argc; /*g_ comes from "generated"*/
                char** g_argv;
                if (FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV(fabric_configuration_parameter_list->Items + u, &g_argc, &g_argv) != 0)
                {
                    LogError("failure in FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV");
                    was_error = true;
                }
                else
                {
                    if (ARGC_ARGV_concat(argc, argv, g_argc, g_argv) != 0)
                    {
                        LogError("failure in ARGC_ARGV_concat");
                        was_error = true;
                    }
                    
                    ARGC_ARGV_free(g_argc, g_argv);
                }
            }

            if (was_error)
            {
                /*TODO: cleanup*/
                result = MU_FAILURE;
            }
            else
            {
                result = 0;
            }
        }
    }
    return result;
   
}

ARGC_ARGV_DATA_RESULT FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(int argc, char** argv, FABRIC_CONFIGURATION_PARAMETER_LIST* fabric_configuration_parameter_list, int* argc_consumed)
{
    ARGC_ARGV_DATA_RESULT result;
    /*scan parameteres until either: argc is consumed, or parameter scan fails*/
    fabric_configuration_parameter_list->Count = 0;
    fabric_configuration_parameter_list->Items = NULL;

    bool error = false;
    bool done_scanning = false;
    *argc_consumed = 0;

    while (!error && !done_scanning && argc > 0)
    {
        FABRIC_CONFIGURATION_PARAMETER temp;
        int consumed = 0;
        ARGC_ARGV_DATA_RESULT r = FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV(argc, argv, &temp, &consumed);
        switch (r)
        {
            case ARGC_ARGV_DATA_OK:
            {
                fabric_configuration_parameter_list->Count++;
                FABRIC_CONFIGURATION_PARAMETER* re = realloc_2((void*)fabric_configuration_parameter_list->Items, fabric_configuration_parameter_list->Count, sizeof(FABRIC_CONFIGURATION_PARAMETER));
                if (re == NULL)
                {
                    LogError("failure in realloc_2");
                    fabric_configuration_parameter_list->Count--;
                    error = true;
                }
                else
                {
                    fabric_configuration_parameter_list->Items = re;
                    /*cast the const away*/
                    *(FABRIC_CONFIGURATION_PARAMETER*)&(fabric_configuration_parameter_list->Items[fabric_configuration_parameter_list->Count - 1]) = temp;
                    argc -= consumed;
                    argv += consumed;
                    *argc_consumed += consumed;
                }
                break;
            }
            case ARGC_ARGV_DATA_INVALID:
            {
                /*not an error, likely the section ended somehow*/
                LogVerbose("done scanning");
                done_scanning = true;
                break;
            }
            case ARGC_ARGV_DATA_ERROR:
            default:
            {
                LogError("failure in FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV");
                error = true;
                break;
            }
        }
    }

    if (done_scanning || argc == 0)
    {
        result = ARGC_ARGV_DATA_OK; /*(note: this function doesn't really return "invalid" because 0 size list is still valid*/
    }
    else
    {
        /*attempt cleanup*/
        for (unsigned int i = 0; i < fabric_configuration_parameter_list->Count; i++)
        {
            FABRIC_CONFIGURATION_PARAMETER_free((FABRIC_CONFIGURATION_PARAMETER*)fabric_configuration_parameter_list->Items + fabric_configuration_parameter_list->Count);
            free((void*)fabric_configuration_parameter_list->Items);
        }
        result = ARGC_ARGV_DATA_ERROR;
    }
    return result;
}

void FABRIC_CONFIGURATION_PARAMETER_LIST_free(FABRIC_CONFIGURATION_PARAMETER_LIST* fabric_configuration_parameter_list)
{
    for (unsigned int i = 0; i < fabric_configuration_parameter_list->Count; i++)
    {
        FABRIC_CONFIGURATION_PARAMETER_free((FABRIC_CONFIGURATION_PARAMETER*)fabric_configuration_parameter_list->Items + i);
    }
    free((void*)fabric_configuration_parameter_list->Items);
}
