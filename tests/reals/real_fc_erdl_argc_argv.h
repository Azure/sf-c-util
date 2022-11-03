// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/*note: "fc_erdl_argc_argv" comes from "fabric configuration endpoint resource description list" and was shortened because of the build system who is unhappy with such a long filename*/

#ifndef REAL_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_ARGC_ARGV_H
#define REAL_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_ARGC_ARGV_H

#include <stdint.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_ARGC_ARGV_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV, \
        FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV,   \
        FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free \
)

#include "sf_c_util/fc_erd_argc_argv.h"


int real_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV(const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list, int* argc, char*** argv);
ARGC_ARGV_DATA_RESULT real_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(int argc, char** argv, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list, int* argc_consumed);
void real_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list);

#endif //REAL_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_ARGC_ARGV_H
