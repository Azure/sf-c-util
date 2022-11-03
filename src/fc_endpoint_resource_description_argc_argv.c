// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_endpoint_resource_description_argc_argv.h"


/* FABRIC_ENDPOINT_RESOURCE_DESCRIPTION => argc/argv is --serviceEndpointResource "string" --Protocol "string" --Type "string" --Port "string" --CertificateName "string"*/
int FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_to_ARGC_ARGV(const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description, int* argc, char*** argv)
{
    int result;
    if (
        (fabric_endpoint_resource_description == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description=%p, int* argc=%p, char*** argv=%p",
            fabric_endpoint_resource_description, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        *argc = 6;
        *argv = malloc_2(*argc, sizeof(char*));
        if (*argv == NULL)
        {
            LogError("failure in malloc2");
            result = MU_FAILURE;
        }
        else
        {
            (* argv)[0] = sprintf_char("%s", SERVICE_ENDPOINT_RESOURCE);
            if ((*argv)[0] == NULL)
            {
                LogError("failure in sprintf_char");
                result = MU_FAILURE;
            }
            else
            {
                (*argv)[1] = sprintf_char("%ls", fabric_endpoint_resource_description->Name);
                if ((*argv)[1] == NULL)
                {
                    LogError("failure in sprintf_char");
                    result = MU_FAILURE;
                }
                else
                {
                    (*argv)[2] = sprintf_char("%ls", fabric_endpoint_resource_description->Protocol);
                    if ((*argv)[2] == NULL)
                    {
                        LogError("failure in sprintf_char");
                        result = MU_FAILURE;
                    }
                    else
                    {
                        (*argv)[3] = sprintf_char("%ls", fabric_endpoint_resource_description->Type);
                        if ((*argv)[3] == NULL)
                        {
                            LogError("failure in sprintf_char");
                            result = MU_FAILURE;
                        }
                        else
                        {
                            (*argv)[4] = sprintf_char("%" PRIu16 "", fabric_endpoint_resource_description->Port);
                            if ((*argv)[4] == NULL)
                            {
                                LogError("failure in sprintf_char");
                                result = MU_FAILURE;
                            }
                            else
                            {
                                (*argv)[5] = sprintf_char("%ls", fabric_endpoint_resource_description->CertificateName);
                                if ((*argv)[5] == NULL)
                                {
                                    LogError("failure in sprintf_char");
                                    result = MU_FAILURE;
                                }
                                else
                                {
                                    result = 0;
                                    goto allok;
                                    //free((*argv)[5]);
                                }
                                free((*argv)[4]);
                            }
                            free((*argv)[3]);
                        }
                        free((*argv)[2]);
                    }
                    free((*argv)[1]);
                }
                free((*argv)[0]);
            }
            free(*argv);
        }
    }
allok:;
    return result;
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
