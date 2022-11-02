// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_endpoint_resource_description_argc_argv.h"


/* FABRIC_ENDPOINT_RESOURCE_DESCRIPTION => argc/argv */
int FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_to_ARGC_ARGV(const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description, int* argc, char*** argv)
{
    (void)fabric_endpoint_resource_description;
    (void)argc;
    (void)argv;
    return MU_FAILURE;
}

/* argc/argv => FABRIC_CONFIGURATION_PARAMETER_LIST* */
ARGC_ARGV_DATA_RESULT FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(int argc, char** argv, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description, int* argc_consumed)
{
    (void)argc;
    (void)argv;
    (void)fabric_endpoint_resource_description;
    (void)argc_consumed;
    return MU_FAILURE;
}

/* freeing a previously produced FABRIC_CONFIGURATION_PARAMETER_LIST* */
void FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description)
{
    (void)fabric_endpoint_resource_description;
}
