// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_CONFIGURATION_PARAMETER_LIST_ARGC_ARGV_H
#define FABRIC_CONFIGURATION_PARAMETER_LIST_ARGC_ARGV_H


#include "fabrictypes.h"

#include "common_argc_argv.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    /*a FABRIC_CONFIGURATION_PARAMETER_LIST is serialized to argc/argv as "name" and "value" string... (and on and on)*/

    /* FABRIC_CONFIGURATION_PARAMETER_LIST => argc/argv */
    MOCKABLE_FUNCTION(, int, FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV, const FABRIC_CONFIGURATION_PARAMETER_LIST*, fabric_configuration_parameter_list, int*, argc, char***, argv);

    /* argc/argv => FABRIC_CONFIGURATION_PARAMETER_LIST* */
    MOCKABLE_FUNCTION(, ARGC_ARGV_DATA_RESULT, FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV, int, argc, char**, argv, FABRIC_CONFIGURATION_PARAMETER_LIST*, fabric_configuration_parameter_list, int*, argc_consumed);

    /* freeing a previously produced FABRIC_CONFIGURATION_PARAMETER_LIST* */
    MOCKABLE_FUNCTION(, void, FABRIC_CONFIGURATION_PARAMETER_LIST_free, FABRIC_CONFIGURATION_PARAMETER_LIST*, fabric_configuration_parameter_list);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_CONFIGURATION_PARAMETER_LIST_ARGC_ARGV_H */
