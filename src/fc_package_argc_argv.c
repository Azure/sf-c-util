// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/fc_section_list_argc_argv.h"

#include "sf_c_util/fc_package_argc_argv.h"

/* FABRIC_CONFIGURATION_PACKAGE => argc/argv */
int FABRIC_CONFIGURATION_PACKAGE_to_ARGC_ARGV(IFabricConfigurationPackage* iFabricConfigurationPackage, int* argc, char*** argv)
{
    int result;
    if (
        (iFabricConfigurationPackage == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid arguments const IFabricConfigurationPackage* iFabricConfigurationPackage=%p, int* argc=%p, char*** argv=%p",
            iFabricConfigurationPackage, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        
        *argc = 2;
        (*argv)[0] = sprintf_char(CONFIGURATION_PACKAGE_NAME);
        if ((*argv)[0] == NULL)
        {
            LogError("failure in sprintf_char(CONFIGURATION_PACKAGE_NAME=%s)", CONFIGURATION_PACKAGE_NAME);
            result = MU_FAILURE;
        }
        else
        {
            const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* fabric_configuration_package_description;
            fabric_configuration_package_description = iFabricConfigurationPackage->lpVtbl->get_Description(iFabricConfigurationPackage);

            (*argv)[1] = sprintf_char("%ls", fabric_configuration_package_description->Name);
            if ((*argv)[1] == NULL)
            {
                LogError("failure in sprintf_char(\"%%ls\", fabric_configuration_package_description->Name=%ls);", fabric_configuration_package_description->Name);
                result = MU_FAILURE;
            }
            else
            {
                const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings= iFabricConfigurationPackage->lpVtbl->get_Settings(iFabricConfigurationPackage);
                int p_argc;
                char** p_argv;
                if (FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(fabric_configuration_settings->Sections, &p_argc, &p_argv) != 0)
                {
                    LogError("failure in FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV");
                    result = MU_FAILURE;
                }
                else
                {
                    if (ARGC_ARGV_concat(argc, argv, p_argc, p_argv) != 0)
                    {
                        ARGC_ARGV_free(p_argc, p_argv);
                        LogError("failure in ARGC_ARGV_concat");
                        /*TODO: likely more cleanup*/
                        result = MU_FAILURE;
                    }
                    else
                    {
                        ARGC_ARGV_free(p_argc, p_argv);
                        result = 0;
                        goto allok;
                    }
                }
                free((*argv)[1]);
            }
            free((*argv)[0]);
        }
allok:;
    }
    return result;
}

/* argc/argv => IFabricConfigurationPackage* */
ARGC_ARGV_DATA_RESULT FABRIC_CONFIGURATION_PACKAGE_from_ARGC_ARGV(int argc, char** argv, IFabricConfigurationPackage** iFabricConfigurationPackage, int* argc_consumed)
{
    (void)argc;
    (void)argv;
    (void)iFabricConfigurationPackage;
    (void)argc_consumed;
    return ARGC_ARGV_DATA_ERROR;
}

/* freeing a previously produced IFabricConfigurationPackage* */
void FABRIC_CONFIGURATION_PACKAGE_free(IFabricConfigurationPackage* iFabricConfigurationPackage)
{
    (void)iFabricConfigurationPackage;
}
