// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/*note: "fc_erdl_argc_argv" comes from "fabric configuration endpoint resource description list" and was shortened because of the build system who is unhappy with such a long filename*/

#include <errno.h>
#include <string.h>

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_erd_argc_argv.h"

#include "sf_c_util/fc_erdl_argc_argv.h"


/* FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST => argc/argv is a list of --serviceEndpointResource "string" --Protocol "string" --Type "string" --Port "string" --CertificateName "string"*/
int FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV(const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list, int* argc, char*** argv)
{
    int result;
    if (
        (fabric_endpoint_resource_description_list == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument const const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list=%p, int* argc=%p, char*** argv=%p",
            fabric_endpoint_resource_description_list, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        result = 0;
    }
    return result;
}

/* argc/argv => FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* */
ARGC_ARGV_DATA_RESULT FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(int argc, char** argv, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list, int* argc_consumed)
{
    ARGC_ARGV_DATA_RESULT result;
    if (
        (argv == NULL) ||
        (fabric_endpoint_resource_description_list == NULL) ||
        (argc_consumed == NULL)
        )
    {
        LogError("invalid argument int argc=%d, char** argv=%p, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list=%p, int* argc_consumed=%p",
            argc, argv, fabric_endpoint_resource_description_list, argc_consumed);
        result = ARGC_ARGV_DATA_INVALID;
    }
    else
    {
        result = ARGC_ARGV_DATA_ERROR;
    }
    return result;
}

/* freeing a previously produced FABRIC_CONFIGURATION_PARAMETER_LIST* */
void FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list)
{
    if (fabric_endpoint_resource_description_list == NULL)
    {
        LogError("invalid argument FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list=%p", fabric_endpoint_resource_description_list);
    }
    else
    {

    }
}
