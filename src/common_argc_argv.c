// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "macro_utils/macro_utils.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/common_argc_argv.h"

MU_DEFINE_ENUM_STRINGS(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);


void ARGC_ARGV_free(int argc, char** argv)
{
    if (argv == NULL)
    {
        LogError("invalid argument int argc, char** argv=%p", argv);
    }
    else
    {
        for (int i = 0; i < argc; i++)
        {
            free(argv[i]);
        }
        free(argv);
    }
}
