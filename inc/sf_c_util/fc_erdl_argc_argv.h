// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FC_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_H
#define FC_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_H

/*note: "fc_erdl_argc_argv" comes from "fabric configuration endpoint resource description list" and was shortened because of the build system who is unhappy with such a long filename*/

#include "windows.h"
#include "fabriccommon.h"
#include "fabricruntime.h"
#include "fabrictypes.h"

#include "common_argc_argv.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
    extern "C" {
#endif

        /*a FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST is serialized to argc/argv as a list of --serviceEndpointResource "string" --Protocol "string" --Type "string" --Port "string" --CertificateName "string"*/

        /* FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST => argc/argv */
        MOCKABLE_FUNCTION(, int, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV, const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST*, fabric_endpoint_resource_description_list, int*, argc, char***, argv);

        /* argc/argv => FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* */
        MOCKABLE_FUNCTION(, ARGC_ARGV_DATA_RESULT, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV, int, argc, char**, argv, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST*, fabric_endpoint_resource_description_list, int*, argc_consumed);

        /* freeing a previously produced FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* */
        MOCKABLE_FUNCTION(, void, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST*, fabric_endpoint_resource_description_list);

#ifdef __cplusplus
    }
#endif

#endif /* FC_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_H */
