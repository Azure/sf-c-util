// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "macro_utils/macro_utils.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

MU_DEFINE_ENUM_STRINGS(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

const char* ARGC_ARGV_KEYWORDS_LIST[] = { ARGC_ARGV_KEYWORDS_LIST_DEFINE };

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

int ARGC_ARGV_concat(int* argc_dest, char*** argv_dest, int argc_source, char** argv_source)
{
    int result;
    if (
        (argc_dest == NULL) ||
        (argv_dest == NULL) ||
        ((argc_source==0) && (argv_source != NULL))
        )
    {
        LogError("invalid argument int* argc_dest=%p, char*** argv_dest=%p, int argc_source=%d, char** argv_source=%p", argc_dest, argv_dest, argc_source, argv_source);
        result = MU_FAILURE;
    }
    else
    {
        char** temp;
        temp = realloc(*argv_dest, ((* argc_dest) + argc_source) * sizeof(char**));
        if (temp == NULL)
        {
            LogError("failure in realloc(argv_dest, (*argc_dest + argc_dest) * sizeof(char**));");
            result = MU_FAILURE;
        }
        else
        {
            int i;
            for (i = 0; i < argc_source; i++)
            {
                temp[*argc_dest + i] = sprintf_char("%s", argv_source[i]);
                if (temp[*argc_dest + i] == NULL)
                {
                    LogError("failure in copyying");
                    break;
                }
                else
                {
                    /*all good*/
                }
            }
            /*TODO: clean if it fails*/

            if (i != argc_source)
            {
                result = MU_FAILURE;
            }
            else
            {
                *argc_dest += argc_source;
                *argv_dest = temp;
                result = 0;
            }
        }
    }
    return result;
}
