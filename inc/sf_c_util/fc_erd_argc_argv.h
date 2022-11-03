// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FC_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_H
#define FC_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_H

/*note: "fc_erd_argc_argv" comes from "fabric configuration endpoint resource description" and was shortened because of the build system who is unhappy with such a long filename*/

#include "windows.h"
#include "fabriccommon.h"
#include "fabricruntime.h"
#include "fabrictypes.h"

#include "common_argc_argv.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
    extern "C" {
#endif

        /*a FABRIC_ENDPOINT_RESOURCE_DESCRIPTION is serialized to argc/argv as --serviceEndpointResource "string" --Protocol "string" --Type "string" --Port "string" --CertificateName "string"*/

        /* FABRIC_ENDPOINT_RESOURCE_DESCRIPTION => argc/argv */
        MOCKABLE_FUNCTION(, int, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_to_ARGC_ARGV, const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION*, fabric_endpoint_resource_description, int*, argc, char***, argv);

        /* argc/argv => FABRIC_CONFIGURATION_PARAMETER_LIST* */
        MOCKABLE_FUNCTION(, ARGC_ARGV_DATA_RESULT, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV, int, argc, char**, argv, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION*, fabric_endpoint_resource_description, int*, argc_consumed);

        /* freeing a previously produced FABRIC_CONFIGURATION_PARAMETER_LIST* */
        MOCKABLE_FUNCTION(, void, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION*, fabric_endpoint_resource_description);

#ifdef __cplusplus
    }
#endif

#endif /* FC_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_H */
