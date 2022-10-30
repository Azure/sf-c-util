// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/fc_package_argc_argv.h"

/* FABRIC_CONFIGURATION_PACKAGE => argc/argv */
int FABRIC_CONFIGURATION_PACKAGE_to_ARGC_ARGV(const IFabricConfigurationPackage* iFabricConfigurationPackage, int* argc, char*** argv)
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
        result = 0; /*TODO: continue from here*/
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
