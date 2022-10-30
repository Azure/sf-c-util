// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_CONFIGURATION_PARAMETER_ARGC_ARGV_H
#define FABRIC_CONFIGURATION_PARAMETER_ARGC_ARGV_H


#include "fabrictypes.h"

#include "common_argc_argv.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    /*a FABRIC_CONFIGURATION_PARAMETER is serialized to argc/argv as "name" and "value" string*/

    /* FABRIC_CONFIGURATION_PARAMETER => argc/argv */
    MOCKABLE_FUNCTION(, int, FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV, const FABRIC_CONFIGURATION_PARAMETER*, fabric_configuration_parameter, int*, argc, char***, argv);

    /* argc/argv => FABRIC_CONFIGURATION_PARAMETER* */
    MOCKABLE_FUNCTION(, ARGC_ARGV_DATA_RESULT, FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV, int, argc, char**, argv, FABRIC_CONFIGURATION_PARAMETER*, fabric_configuration_parameter, int*, argc_consumed);

    /* freeing a previously produced FABRIC_CONFIGURATION_PARAMETER* */
    MOCKABLE_FUNCTION(, void, FABRIC_CONFIGURATION_PARAMETER_free, FABRIC_CONFIGURATION_PARAMETER*, fabric_configuration_parameter);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_CONFIGURATION_PARAMETER_ARGC_ARGV_H */
