// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/fc_parameter_list_argc_argv.h"

#include "sf_c_util/fc_section_argc_argv.h"

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
                    int g_argc;
                    char** g_argv;
                    if (FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV(fabric_configuration_section->Parameters, &g_argc, &g_argv) != 0)
                    {
                        LogError("failure in FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV");
                        result = MU_FAILURE;
                    }
                    else
                    {
                        if (ARGC_ARGV_concat(argc, argv, g_argc, g_argv) != 0)
                        {
                            ARGC_ARGV_free(g_argc, g_argv);
                            LogError("failure in ARGC_ARGV_concat");
                            result = MU_FAILURE;
                        }
                        else
                        {
                            ARGC_ARGV_free(g_argc, g_argv);
                            result = 0;
                            goto allok;
                        }
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

/* argc/argv => FABRIC_CONFIGURATION_SECTION* */
ARGC_ARGV_DATA_RESULT FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(int argc, char** argv, FABRIC_CONFIGURATION_SECTION* fabric_configuration_section, int* argc_consumed)
{
    ARGC_ARGV_DATA_RESULT result;
    if (
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
        if (
            (argc < 2) ||
            (argv == NULL)
            )
        {
            LogError("not enough data to parse: int argc=%d, char** argv=%p, FABRIC_CONFIGURATION_SECTION** fabric_configuration_section=%p, int* argc_consumed=%p",
                argc, argv, fabric_configuration_section, argc_consumed);
            result = ARGC_ARGV_DATA_INVALID;
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

                    fabric_configuration_section->Parameters = malloc(sizeof(FABRIC_CONFIGURATION_PARAMETER_LIST));
                    if (fabric_configuration_section->Parameters == NULL)
                    {
                        LogError("failure in malloc");
                        result = ARGC_ARGV_DATA_ERROR;
                    }
                    else
                    {
                        int consumed;
                        ARGC_ARGV_DATA_RESULT r = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc - (*argc_consumed), argv + (*argc_consumed), (FABRIC_CONFIGURATION_PARAMETER_LIST*)fabric_configuration_section->Parameters, &consumed);
                        switch (r)
                        {
                            case ARGC_ARGV_DATA_OK:
                            {
                                *argc_consumed += consumed;
                                result = ARGC_ARGV_DATA_OK;
                                goto allok;
                                break; /*interesting the compiler ain't complaining about unreachable code here. Bug??*/
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

                        free((void*)fabric_configuration_section->Parameters);
                    }
                }
            }
        }
    }
allok:;
    return result;
}

/* freeing a previously filled FABRIC_CONFIGURATION_SECTION's data */
void FABRIC_CONFIGURATION_SECTION_free(FABRIC_CONFIGURATION_SECTION* fabric_configuration_section)
{
    free((void*)fabric_configuration_section->Name);
    FABRIC_CONFIGURATION_PARAMETER_LIST_free((FABRIC_CONFIGURATION_PARAMETER_LIST*)fabric_configuration_section->Parameters);
    free((void*)fabric_configuration_section->Parameters);
}
