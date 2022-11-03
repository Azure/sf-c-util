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
        *argc = 0;
        *argv = NULL;
        bool wasError = false;
        for (ULONG i = 0; !wasError && (i < fabric_endpoint_resource_description_list->Count); i++)
        {
            int p_argc;
            char** p_argv;
            if (FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_to_ARGC_ARGV(fabric_endpoint_resource_description_list->Items + i, &p_argc, &p_argv) != 0)
            {
                LogError("fairlure in FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_to_ARGC_ARGV");
                wasError = true;
            }
            else
            {
                if (ARGC_ARGV_concat(argc, argv, p_argc, p_argv) != 0)
                {
                    LogError("failure in ARGC_ARGV_concat");
                }
                else
                {
                    /*keep going...*/
                }

                ARGC_ARGV_free(p_argc, p_argv);
            }
        }

        if (wasError)
        {
            ARGC_ARGV_free(*argc, *argv);
            /*clean*/
            result = MU_FAILURE;
        }
        else
        {
            result = 0;
        }
    }
    return result;
}

/* argc/argv => FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* */
ARGC_ARGV_DATA_RESULT FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(int argc, char** argv, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list, int* argc_consumed)
{
    ARGC_ARGV_DATA_RESULT result;
    if (
        ((argv == NULL) &&(argc!=0)) ||
        (fabric_endpoint_resource_description_list == NULL) ||
        (argc_consumed == NULL)
        )
    {
        LogError("invalid argument int argc=%d, char** argv=%p, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* fabric_endpoint_resource_description_list=%p, int* argc_consumed=%p",
            argc, argv, fabric_endpoint_resource_description_list, argc_consumed);
        result = ARGC_ARGV_DATA_ERROR;
    }
    else
    {
        *argc_consumed = 0;
        fabric_endpoint_resource_description_list->Count = 0;
        fabric_endpoint_resource_description_list->Items = NULL;

        bool done = false;
        bool wasError = false;


        while(!wasError && !done)
        {
            FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
            int c_argc;
            ARGC_ARGV_DATA_RESULT r = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc - *argc_consumed, argv + *argc_consumed, &d, &c_argc);
            switch (r)
            {
                case ARGC_ARGV_DATA_OK:
                {
                    fabric_endpoint_resource_description_list->Count++;
                    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* temp = realloc_2((void*)fabric_endpoint_resource_description_list->Items, fabric_endpoint_resource_description_list->Count, sizeof(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION));
                    if (temp == NULL)
                    {
                        fabric_endpoint_resource_description_list->Count--;
                        LogError("Failure in realloc_2");
                        wasError = true;
                    }
                    else
                    {
                        fabric_endpoint_resource_description_list->Items = temp;
                        /*cast the const away*/
                        *(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION*)(fabric_endpoint_resource_description_list->Items+fabric_endpoint_resource_description_list->Count - 1) = d;
                        /*advance to the next arguments*/
                        *argc_consumed += c_argc;
                    }
                    break;
                }
                case ARGC_ARGV_DATA_INVALID:
                {
                    /*not really an error, we are done scanning*/
                    done = true;
                    break;
                }
                case ARGC_ARGV_DATA_ERROR:
                default:
                {
                    LogError("failure in FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV, %" PRI_MU_ENUM "", MU_ENUM_VALUE(ARGC_ARGV_DATA_RESULT, r));
                    wasError = true;
                    break;
                }
            } /*switch*/
        } /*while*/

        if (wasError)
        {
            /*undo all changes*/
            for (ULONG i = 0; i < fabric_endpoint_resource_description_list->Count; i++)
            {
                FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free((FABRIC_ENDPOINT_RESOURCE_DESCRIPTION*)(fabric_endpoint_resource_description_list->Items + i));
            }
            free((void*)fabric_endpoint_resource_description_list->Items);
            result = ARGC_ARGV_DATA_ERROR;
        }
        else
        {
            result = ARGC_ARGV_DATA_OK; /*note: this can result in a 0 element for FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST, still totally valid*/
        }
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
        for (ULONG i = 0; i < fabric_endpoint_resource_description_list->Count; i++)
        {
            FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free((FABRIC_ENDPOINT_RESOURCE_DESCRIPTION*)(fabric_endpoint_resource_description_list->Items + i));
        }
        free((void*)fabric_endpoint_resource_description_list->Items);
    }
}
