// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_FABRIC_CONFIGURATION_SECTION_LIST_ARGC_ARGV_H
#define REAL_FABRIC_CONFIGURATION_SECTION_LIST_ARGC_ARGV_H

#include <stdint.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_FABRIC_CONFIGURATION_SECTION_LIST_ARGC_ARGV_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV, \
        FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV,   \
        FABRIC_CONFIGURATION_SECTION_LIST_free \
)

#include "sf_c_util/fc_section_argc_argv.h"


int real_FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(const FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list , int* argc, char*** argv);
ARGC_ARGV_DATA_RESULT real_FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(int argc, char** argv, FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list, int* argc_consumed);
void real_FABRIC_CONFIGURATION_SECTION_LIST_free(FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list);

#endif //REAL_FABRIC_CONFIGURATION_SECTION_LIST_ARGC_ARGV_H
