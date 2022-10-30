// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_CONFIGURATION_PACKAGE_ARGC_ARGV_H
#define FABRIC_CONFIGURATION_PACKAGE_ARGC_ARGV_H

#include "macro_utils/macro_utils.h"

#include "fabricruntime.h"

#include "sf_c_util/common_argc_argv.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    /*a FABRIC_CONFIGURATION_PACKAGE is serialized to argc/argv as --configurationPackageName "name" followed by 1x FABRIC_CONFIGURATION_SECTION_LIST*/

    /* FABRIC_CONFIGURATION_PACKAGE => argc/argv */
    MOCKABLE_FUNCTION(, int, FABRIC_CONFIGURATION_PACKAGE_to_ARGC_ARGV, IFabricConfigurationPackage*, iFabricConfigurationPackage, int*, argc, char***, argv);

    /* argc/argv => IFabricConfigurationPackage* sort of "factory" :)*/
    MOCKABLE_FUNCTION(, ARGC_ARGV_DATA_RESULT, FABRIC_CONFIGURATION_PACKAGE_from_ARGC_ARGV, int, argc, char**, argv, IFabricConfigurationPackage**, iFabricConfigurationPackage, int*, argc_consumed);

    /* freeing a previously produced IFabricConfigurationPackage* */
    MOCKABLE_FUNCTION(, void, FABRIC_CONFIGURATION_PACKAGE_free, IFabricConfigurationPackage*, iFabricConfigurationPackage);

#ifdef __cplusplus
}
#endif

#endif /*FABRIC_CONFIGURATION_PACKAGE_ARGC_ARGV_H*/
