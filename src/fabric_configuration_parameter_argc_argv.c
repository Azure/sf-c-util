// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>
#include <stdbool.h>

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/fabric_configuration_parameter_argc_argv.h"

int FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV(const FABRIC_CONFIGURATION_PARAMETER* fabric_configuration_parameter, int* argc, char*** argv)
{
    int result;
    if (
        (fabric_configuration_parameter == NULL) ||
        (argc == NULL) ||
        (argv == NULL) ||
        (fabric_configuration_parameter->IsEncrypted) ||
        (fabric_configuration_parameter->MustOverride)
        )
    {
        LogError("invalid argument const FABRIC_CONFIGURATION_PARAMETER* fabric_configuration_parameter=%p, int* argc=%p, char*** argv=%p (or might be unsupported)",
            fabric_configuration_parameter, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        *argv = malloc_2(2, sizeof(char**));
        if (*argv == NULL)
        {
            LogError("failure in malloc_2(2, sizeof(char**)=%zu);", sizeof(char**));
            result = MU_FAILURE;
        }
        else
        {
            (*argv)[0] = sprintf_char("%ls", fabric_configuration_parameter->Name);
            if ((*argv)[0] == NULL)
            {
                LogError("failure in sprintf_char(\"%%ls\", fabric_configuration_parameter->Name=%ls);", fabric_configuration_parameter->Name);
                result = MU_FAILURE;
            }
            else
            {
                (*argv)[1] = sprintf_char("%ls", fabric_configuration_parameter->Value);
                if ((*argv)[1] == NULL)
                {
                    LogError("failure in sprintf_char(\"%%ls\", fabric_configuration_parameter->Value=%ls);", fabric_configuration_parameter->Value);
                    result = MU_FAILURE;
                }
                else
                {
                    *argc = 2;
                    result = 0;
                    goto allok;
                }
                free((*argv)[0]);
            }
            free(*argv);
        }
    }
allok:;
    return result;
}

ARGC_ARGV_DATA_RESULT FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV(int argc, char** argv, FABRIC_CONFIGURATION_PARAMETER** fabric_configuration_parameter, int* argc_consumed)
{
    ARGC_ARGV_DATA_RESULT result;
    if (
        (argc < 2) ||
        (argv == NULL) ||
        (argv[0] == NULL) ||
        (argv[1] == NULL) ||
        (fabric_configuration_parameter == NULL) ||
        (argc_consumed == NULL)
        )
    {
        LogError("invalid argument int argc=%d, char** argv=%p, FABRIC_CONFIGURATION_PARAMETER* fabric_configuration_parameter=%p, int* argc_consumed=%p",
            argc, argv, fabric_configuration_parameter, argc_consumed);
        result = ARGC_ARGV_DATA_INVALID;
    }
    else
    {
        for (uint32_t i = 0; i < sizeof(ARGC_ARGV_KEYWORDS_LIST) / sizeof(ARGC_ARGV_KEYWORDS_LIST[0]); i++)
        {
            if (strcmp(argv[0], ARGC_ARGV_KEYWORDS_LIST[i]) == 0)
            {
                LogVerbose("argv[0]=%s cannot be a parameter name because it is a reserved keyword", argv[0]);
                return  ARGC_ARGV_DATA_INVALID;
            }
        }
        
        *fabric_configuration_parameter = malloc(sizeof(FABRIC_CONFIGURATION_PARAMETER));
        if (*fabric_configuration_parameter == NULL)
        {
            LogError("failure in malloc(sizeof(FABRIC_CONFIGURATION_PARAMETER)=%zu);", sizeof(FABRIC_CONFIGURATION_PARAMETER));
            result = ARGC_ARGV_DATA_ERROR;
        }
        else
        {
            (*fabric_configuration_parameter)->IsEncrypted = false;
            (*fabric_configuration_parameter)->MustOverride = false;
            (*fabric_configuration_parameter)->Reserved = NULL;

            (*fabric_configuration_parameter)->Name = mbs_to_wcs(argv[0]);
            if ((*fabric_configuration_parameter)->Name == NULL)
            {
                LogError("failure in mbs_to_wcs(argv[0]=%s);", argv[0]);
                result = ARGC_ARGV_DATA_ERROR;
            }
            else
            {
                (*fabric_configuration_parameter)->Value = mbs_to_wcs(argv[1]);

                if ((*fabric_configuration_parameter)->Value == NULL)
                {
                    LogError("failure in mbs_to_wcs(argv[1]=%s);", argv[1]);
                    result = ARGC_ARGV_DATA_ERROR;
                }
                else
                {
                    result = ARGC_ARGV_DATA_OK;
                    *argc_consumed = 2;
                    goto allok;
                }

                free((void*)(*fabric_configuration_parameter)->Name);
            }
            free(*fabric_configuration_parameter);
        }
    }
allok:;
    return result;
}


void FABRIC_CONFIGURATION_PARAMETER_free(FABRIC_CONFIGURATION_PARAMETER* fabric_configuration_parameter)
{
    if (fabric_configuration_parameter == NULL)
    {
        LogError("invalid argument FABRIC_CONFIGURATION_PARAMETER* fabric_configuration_parameter=%p", fabric_configuration_parameter);
    }
    else
    {
        free((void*)fabric_configuration_parameter->Name);
        free((void*)fabric_configuration_parameter->Value);
        free(fabric_configuration_parameter);
    }
}
