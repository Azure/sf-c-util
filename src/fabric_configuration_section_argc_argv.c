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

/* argc/argv => FABRIC_CONFIGURATION_SECTION* */
ARGC_ARGV_DATA_RESULT FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(int argc, char** argv, FABRIC_CONFIGURATION_SECTION** fabric_configuration_section, int* argc_consumed)
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
        if (strcmp(argv[0], SECTION_NAME_DEFINE) != 0)
        {
            LogVerbose("cannot parse as FABRIC_CONFIGURATION_SECTION because the first argument is %s, but it is expected to be " SECTION_NAME_DEFINE "", argv[0]);
            result = ARGC_ARGV_DATA_INVALID
        }
        else
        {
            *fabric_configuration_section = malloc(sizeof(FABRIC_CONFIGURATION_SECTION));
            if (*fabric_configuration_section == NULL)
            {
                LogError("failure in malloc(sizeof(FABRIC_CONFIGURATION_SECTION)=%zu);",
                    sizeof(FABRIC_CONFIGURATION_SECTION));
            }
            else
            {
                (*fabric_configuration_section)->Name = mbs_to_wcs(argv[1]);
                if ((*fabric_configuration_section)->Name == NULL)
                {
                    LogError("failure in mbs_to_wcs(argv[1]=%s);", argv[1]);
                }
                else
                {
                    /*while the parameters can still be parsed... parse them*/
                    int param_index = 2; /*parameters start at "2" index*/
                    int param_argc = argc - 2;
                    while (param_argc > 0)
                    {
                        FABRIC_CONFIGURATION_PARAMETER* fabric_configuration_parameter;
                        int argc_consumed;
                        ARGC_ARGV_DATA_RESULT param_result = FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV(param_argc, argv + param_index, &fabric_configuration_parameter, &argc_consumed);
                        switch (param_result)

                        {
                            /*AICI AM RAMAS*/
                        }
                    }
                    free((*fabric_configuration_section)->Name);
                }
                
                free(*fabric_configuration_section);
            }
        }
    }
    return result;
}

/* freeing a previously produced FABRIC_CONFIGURATION_SECTION* */
void FABRIC_CONFIGURATION_SECTION_free(FABRIC_CONFIGURATION_SECTION* fabric_configuration_section)
{
    (void) fabric_configuration_section;
}

