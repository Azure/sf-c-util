// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_COMMON_ARGC_ARGV_H
#define REAL_COMMON_ARGC_ARGV_H

#include <stdint.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_COMMON_ARGC_ARGV_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        ARGC_ARGV_free, \
        ARGC_ARGV_KEYWORDS_LIST \
)

#include "sf_c_util/common_argc_argv.h"

void real_ARGC_ARGV_free(int argc, char** argv);

const char* real_ARGC_ARGV_KEYWORDS_LIST[sizeof(ARGC_ARGV_KEYWORDS_LIST) / sizeof(ARGC_ARGV_KEYWORDS_LIST[0])];

#endif //REAL_COMMON_ARGC_ARGV_H
