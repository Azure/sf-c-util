// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/fc_section_argc_argv.h"

#include "sf_c_util/fc_section_list_argc_argv.h"

/* FABRIC_CONFIGURATION_SECTION_LIST => argc/argv */
int FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(const FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list, int* argc, char*** argv)
{
    int result;
    if (
        (fabric_configuration_section_list == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument const FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list=%p, int* argc=%p, char*** argv=%p",
            fabric_configuration_section_list, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        *argv = NULL;
        *argc = 0;

        bool wasError = false;

        for (ULONG i = 0; !wasError  && i <  fabric_configuration_section_list->Count; i++)
        {
            int g_argc;
            char** g_argv;
            if (FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV(fabric_configuration_section_list->Items + i, &g_argc, &g_argv) != 0)
            {
                LogError("failure in FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV");
            }
            else
            {
                if (ARGC_ARGV_concat(argc, argv, g_argc, g_argv) != 0)
                {
                    LogError("failure in ARGC_ARGV_concat");
                    wasError = true;
                }
                ARGC_ARGV_free(g_argc, g_argv);
            }
        }

        if (wasError)
        {
            result = MU_FAILURE;
        }
        else
        {
            result = 0;
        }
    }

    return result;
}

ARGC_ARGV_DATA_RESULT FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(int argc, char** argv, FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list, int* argc_consumed)
{
    ARGC_ARGV_DATA_RESULT result;
    if (
        (fabric_configuration_section_list == NULL) ||
        (argc_consumed == NULL)
        )
    {
        LogError("invalid arguments int argc=%d, char** argv=%p, FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list=%p, int* argc_consumed=%p",
            argc, argv, fabric_configuration_section_list, argc_consumed);
        result = ARGC_ARGV_DATA_ERROR;
    }
    else
    {
        *argc_consumed = 0;
        fabric_configuration_section_list->Count = 0;
        fabric_configuration_section_list->Items = NULL;

        bool done = false;
        bool wasError = false;

        while(!done && !wasError)
        {
            FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
            int c_argc;
            ARGC_ARGV_DATA_RESULT r = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc - *argc_consumed, argv + *argc_consumed, &fabric_configuration_section, &c_argc);
            switch (r)
            {
                case ARGC_ARGV_DATA_OK:
                {
                    if (c_argc == 0)
                    {
                        /*valid data, but nothing was consumed, we are done*/
                        done = true;
                    }
                    else
                    {
                        fabric_configuration_section_list->Count++;
                        FABRIC_CONFIGURATION_SECTION* new_sections = realloc_2((void*)fabric_configuration_section_list->Items, fabric_configuration_section_list->Count, sizeof(FABRIC_CONFIGURATION_SECTION));
                        if (new_sections == NULL)
                        {
                            fabric_configuration_section_list->Count--;
                            LogError("failure in realloc_2");

                        }
                        else
                        {
                            fabric_configuration_section_list->Items = new_sections;

                            /*cast the const away*/
                            *(FABRIC_CONFIGURATION_SECTION*)& fabric_configuration_section_list->Items[fabric_configuration_section_list->Count - 1] = fabric_configuration_section;
                        }
                        *argc_consumed += c_argc;
                    }
                    break;
                }
                case ARGC_ARGV_DATA_INVALID:
                {
                    done = true;
                    break;
                }
                default:
                case ARGC_ARGV_DATA_ERROR:
                {
                    wasError = true;
                    break;
                }
            }
        }

        if (wasError)
        {
            result = ARGC_ARGV_DATA_ERROR;
        }
        else
        {
            result = ARGC_ARGV_DATA_OK;
        }
    }
    return result;
}

/* freeing a previously filled FABRIC_CONFIGURATION_SECTION_LIST's data */
void FABRIC_CONFIGURATION_SECTION_LIST_free(FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list)
{
    for (ULONG u = 0; u < fabric_configuration_section_list->Count; u++)
    {
        FABRIC_CONFIGURATION_SECTION_free((void*)(fabric_configuration_section_list->Items + u));
    }
    free((void*)fabric_configuration_section_list->Items);
}

