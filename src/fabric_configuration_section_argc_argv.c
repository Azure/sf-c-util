// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/fabric_configuration_parameter_argc_argv.h"

#include "sf_c_util/fabric_configuration_section_argc_argv.h"

/* FABRIC_CONFIGURATION_SECTION => argc/argv */
int FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV(const FABRIC_CONFIGURATION_SECTION* fabric_configuration_section, int* argc, char*** argv)
{
    int result;
    if (
        (fabric_configuration_section == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument const FABRIC_CONFIGURATION_SECTION* fabric_configuration_section=%p, int* argc=%p, char*** argv=%p",
            fabric_configuration_section, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        *argv = malloc_2(2, sizeof(char*)); /*2 because --sectionName + "name", will realloc'd for every parameter*/
        if (*argv == NULL)
        {
            LogError("failure in 2 + fabric_configuration_section->Parameters->Count");
            result = MU_FAILURE;
        }
        else
        {
            *argc = 2;
            (*argv)[0] = sprintf_char(SECTION_NAME_DEFINE);
            if ((*argv)[0] == NULL)
            {
                LogError("failure in sprintf_char(SECTION_NAME_DEFINE=%s)", SECTION_NAME_DEFINE);
                result = MU_FAILURE;
            }
            else
            {
                (*argv)[1] = sprintf_char("%ls", fabric_configuration_section->Name);
                if ((*argv)[1] == NULL)
                {
                    LogError("failure in sprintf_char(\"%%ls\", fabric_configuration_section->Name=%ls);", fabric_configuration_section->Name);
                    result = MU_FAILURE;
                }
                else
                {
                    uint32_t i;
                    for (i = 0; i < fabric_configuration_section->Parameters->Count; i++)
                    {
                        int parameter_argc;
                        char** parameter_argv;
                        if (FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV(fabric_configuration_section->Parameters->Items + i, &parameter_argc, &parameter_argv) != 0)
                        {
                            LogError("failure in FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV(fabric_configuration_section->Parameters->Items + i=%" PRIu32 ", &parameter_argc=%p, &parameter_argv=%p)",
                                i, &argc, &argv);
                            result = MU_FAILURE;
                            break;
                        }
                        else
                        {
                            if (ARGC_ARGV_concat(argc, argv, parameter_argc, parameter_argv) != 0)
                            {
                                LogError("failure in ARGC_ARGV_concat");
                            }
                            else
                            {
                                /*keep going*/
                            }
                            ARGC_ARGV_free(parameter_argc, parameter_argv);
                        }
                    }

                    if (i != fabric_configuration_section->Parameters->Count)
                    {
                        result = MU_FAILURE;
                    }
                    else
                    {
                        result = 0;
                        goto allok;
                    }


                    free(argv[1]);
                }

                free(argv[0]);
            }

            free(*argv);
        }
    }

allok:;
    return result;
}

static ARGC_ARGV_DATA_RESULT fabric_configuration_parameter_list_from_argc_argv(int argc, char** argv, FABRIC_CONFIGURATION_PARAMETER_LIST* fabric_configuration_parameter_list, int* argc_consumed)
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
                    free((void*)fabric_configuration_parameter_list->Items);
                    fabric_configuration_parameter_list->Items = re;
                    /*cast the const away*/
                    *(FABRIC_CONFIGURATION_PARAMETER *) &(fabric_configuration_parameter_list->Items[fabric_configuration_parameter_list->Count - 1]) = temp;
                    argc -= consumed;
                    argv += consumed;
                    argc_consumed += consumed;
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
        for (unsigned int i = 0; i < fabric_configuration_parameter_list->Count; i++)
        {
            FABRIC_CONFIGURATION_PARAMETER_free((FABRIC_CONFIGURATION_PARAMETER*)fabric_configuration_parameter_list->Items + fabric_configuration_parameter_list->Count);
            free((void*)fabric_configuration_parameter_list->Items);
        }
        result = ARGC_ARGV_DATA_ERROR;
    }
    return result;

}

static void fabric_configuration_parameter_list_free(FABRIC_CONFIGURATION_PARAMETER_LIST* fabric_configuration_parameter_list)
{
    for (unsigned int i = 0; i < fabric_configuration_parameter_list->Count; i++)
    {
        FABRIC_CONFIGURATION_PARAMETER_free((FABRIC_CONFIGURATION_PARAMETER*)fabric_configuration_parameter_list->Items + fabric_configuration_parameter_list->Count);
    }
}

/* argc/argv => FABRIC_CONFIGURATION_SECTION* */
ARGC_ARGV_DATA_RESULT FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(int argc, char** argv, FABRIC_CONFIGURATION_SECTION* fabric_configuration_section, int* argc_consumed)
{
    ARGC_ARGV_DATA_RESULT result;
    if (
        (argc < 2) ||
        (argv == NULL) ||
        (fabric_configuration_section == NULL) ||
        (argc_consumed == NULL)
        )
    {
        LogError("invalid arguments int argc=%d, char** argv=%p, FABRIC_CONFIGURATION_SECTION** fabric_configuration_section=%p, int* argc_consumed=%p",
            argc, argv, fabric_configuration_section, argc_consumed);
        result = ARGC_ARGV_DATA_ERROR;
    }
    else
    {
        *argc_consumed = 0;
        if (strcmp(argv[0], SECTION_NAME_DEFINE) != 0)
        {
            LogVerbose("cannot parse as FABRIC_CONFIGURATION_SECTION because the first argument is %s, but it is expected to be " SECTION_NAME_DEFINE "", argv[0]);
            result = ARGC_ARGV_DATA_INVALID;
        }
        else
        {
            (*argc_consumed)++;
            fabric_configuration_section->Name = mbs_to_wcs(argv[1]);
            if (fabric_configuration_section->Name == NULL)
            {
                LogError("failure in mbs_to_wcs(argv[1]=%s);", argv[1]);
                result = ARGC_ARGV_DATA_ERROR;
            }
            else
            {
                (*argc_consumed)++;

                FABRIC_CONFIGURATION_PARAMETER_LIST* fabric_configuration_parameter_list;

                fabric_configuration_parameter_list = malloc(sizeof(FABRIC_CONFIGURATION_PARAMETER_LIST));
                if (fabric_configuration_parameter_list == NULL)
                {
                    LogError("failure in malloc");
                    result = ARGC_ARGV_DATA_ERROR;
                }
                else
                {
                    fabric_configuration_section->Parameters = fabric_configuration_parameter_list;

                    int consumed;
                    ARGC_ARGV_DATA_RESULT r = fabric_configuration_parameter_list_from_argc_argv(argc-(*argc_consumed), argv+(*argc_consumed), fabric_configuration_parameter_list, &consumed);
                    switch (r)
                    {
                        case ARGC_ARGV_DATA_OK:
                        {
                            *argc_consumed += consumed;
                            result = ARGC_ARGV_DATA_OK;
                            goto allok;
                            break;
                        }
                        case ARGC_ARGV_DATA_INVALID:
                        {
                            /*at this point invalid is unexpected because an empty list should always be parseable*/
                            LogError("unexpected %" PRI_MU_ENUM "", MU_ENUM_VALUE(ARGC_ARGV_DATA_RESULT, r));
                            result = ARGC_ARGV_DATA_INVALID;
                        }
                        default:
                        case ARGC_ARGV_DATA_ERROR:
                        {
                            LogError("failure %" PRI_MU_ENUM "", MU_ENUM_VALUE(ARGC_ARGV_DATA_RESULT, r));
                            result = ARGC_ARGV_DATA_INVALID;
                        }
                    }

                    free(fabric_configuration_parameter_list);
                }
            }
        }
    }
allok:;
    return result;
}

/* freeing a previously produced FABRIC_CONFIGURATION_SECTION* */
void FABRIC_CONFIGURATION_SECTION_free(FABRIC_CONFIGURATION_SECTION* fabric_configuration_section)
{
    free((void*)fabric_configuration_section->Name);
    for (unsigned int i = 0; i < fabric_configuration_section->Parameters->Count; i++)
    {
        FABRIC_CONFIGURATION_PARAMETER_free((void*)(fabric_configuration_section->Parameters->Items + i));
    }
    free((void*)(fabric_configuration_section->Parameters->Items));
    free((void*)fabric_configuration_section->Parameters);
}

