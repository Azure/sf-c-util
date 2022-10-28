// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef COMMON_ARGC_ARGV_H
#define COMMON_ARGC_ARGV_H

#include <stdbool.h>

#include "macro_utils/macro_utils.h"

#define ARGC_ARGV_DATA_RESULT_VALUES \
    ARGC_ARGV_DATA_OK, \
    ARGC_ARGV_DATA_INVALID, \
    ARGC_ARGV_DATA_ERROR

MU_DEFINE_ENUM(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

#define ARGC_ARGV_KEYWORDS_LIST_DEFINE "--configurationPackageName", "--sectionName"

extern const char* ARGC_ARGV_KEYWORDS_LIST[MU_COUNT_ARG(ARGC_ARGV_KEYWORDS_LIST_DEFINE)];

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    /* free previously produced argc/argv */
    MOCKABLE_FUNCTION(, void, ARGC_ARGV_free, int, argc, char**, argv);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_ARGC_ARGV_H */
