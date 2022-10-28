// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_CONFIGURATION_SECTION_ARGC_ARGV_H
#define FABRIC_CONFIGURATION_SECTION_ARGC_ARGV_H

#include "macro_utils/macro_utils.h"

#include "fabrictypes.h"

#include "sf_c_util/common_argc_argv.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    /*a FABRIC_CONFIGURATION_SECTION is serialized to argc/argv as --sectionName "name" followed by a list of parameters*/

    /* FABRIC_CONFIGURATION_SECTION => argc/argv */
    MOCKABLE_FUNCTION(, int, FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV, const FABRIC_CONFIGURATION_SECTION*, fabric_configuration_section, int*, argc, char***, argv);

    /* argc/argv => FABRIC_CONFIGURATION_SECTION* */
    MOCKABLE_FUNCTION(, ARGC_ARGV_DATA_RESULT, FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV, int, argc, char**, argv, FABRIC_CONFIGURATION_SECTION**, fabric_configuration_section, int*, argc_consumed);

    /* freeing a previously produced FABRIC_CONFIGURATION_SECTION* */
    MOCKABLE_FUNCTION(, void, FABRIC_CONFIGURATION_SECTION_free, FABRIC_CONFIGURATION_SECTION*, fabric_configuration_section);

#ifdef __cplusplus
}
#endif

#endif /*FABRIC_CONFIGURATION_SECTION_ARGC_ARGV_H*/
