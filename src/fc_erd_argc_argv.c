// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/*note: "fc_erd_argc_argv" comes from "fabric configuration endpoint resource description" and was shortened because of the build system who is unhappy with such a long filename*/

#include <errno.h>
#include <string.h>

#include "windows.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_erd_argc_argv.h"


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
                                if (
                                    (fabric_endpoint_resource_description->CertificateName == NULL) ||
                                    (fabric_endpoint_resource_description->CertificateName[0] == L'\0')
                                    )
                                {
                                    /*if CertificateName is L"" or NULL and that is passed as an argv to a command line - it gets eaten and later FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV expects 6 arguments but only gets 5*/
                                    (*argv)[5] = sprintf_char("%ls", L"CERTIFICATE_NAME_WAS_EMPTY_IN_FABRIC_ENDPOINT_RESOURCE_DESCRIPTION");
                                }
                                else
                                {
                                    (*argv)[5] = sprintf_char("%ls", fabric_endpoint_resource_description->CertificateName);
                                }

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
    ARGC_ARGV_DATA_RESULT result;
    if (
        (argc < 6) ||
        (argv == NULL) ||
        (fabric_endpoint_resource_description == NULL) ||
        (argc_consumed == NULL)
        )
    {
        LogError("invalid argument int argc=%d, char** argv=%p, FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description=%p, int* argc_consumed=%p",
            argc, argv, fabric_endpoint_resource_description, argc_consumed);
        result = ARGC_ARGV_DATA_INVALID;
    }
    else
    {
        if (strcmp(argv[0], SERVICE_ENDPOINT_RESOURCE) != 0)
        {
            LogError("cannot parse %s as FABRIC_ENDPOINT_RESOURCE_DESCRIPTION (it was expected to be " SERVICE_ENDPOINT_RESOURCE "", argv[0]);
            result = ARGC_ARGV_DATA_INVALID;
        }
        else
        {
            char* stop;
            errno = 0;
            uint64_t port = strtoull(argv[4], &stop, 10);
            if (
                (port > UINT16_MAX) || /*note: overflow always returns ULLONG_MAX which is clearly greater than UINT16_MAX*/
                (stop[0]!='\0')
                )
            {
                LogError("scanning of Port=%s failed. Returned value was=%" PRIu64 ", scanning stopped at \"%s\" after %zu characters, errno was=%d (%s)", argv[4], port, stop, stop - argv[4], errno, strerror(errno));
                result = ARGC_ARGV_DATA_INVALID;
            }
            else
            {
                fabric_endpoint_resource_description->Port = (uint16_t)port; /*weird how endpoint description in SF uses "unsigned long" for port... which not even IPV6 changes...*/

                fabric_endpoint_resource_description->Name = mbs_to_wcs(argv[1]);
                if (fabric_endpoint_resource_description->Name == NULL)
                {
                    LogError("failure in mbs_to_wcs");
                    result = ARGC_ARGV_DATA_ERROR;
                }
                else
                {
                    fabric_endpoint_resource_description->Protocol = mbs_to_wcs(argv[2]);
                    if (fabric_endpoint_resource_description->Protocol == NULL)
                    {
                        LogError("failure in mbs_to_wcs");
                        result = ARGC_ARGV_DATA_ERROR;
                    }
                    else
                    {
                        fabric_endpoint_resource_description->Type = mbs_to_wcs(argv[3]);
                        if (fabric_endpoint_resource_description->Type == NULL)
                        {
                            LogError("failure in mbs_to_wcs");
                            result = ARGC_ARGV_DATA_ERROR;
                        }
                        else
                        {
                            fabric_endpoint_resource_description->CertificateName = mbs_to_wcs(argv[5]);
                            if (fabric_endpoint_resource_description->CertificateName == NULL)
                            {
                                LogError("failure in mbs_to_wcs");
                                result = ARGC_ARGV_DATA_ERROR;
                            }
                            else
                            {
                                fabric_endpoint_resource_description->Reserved = NULL;
                                *argc_consumed = 6;
                                result = ARGC_ARGV_DATA_OK;
                                goto allok;
                                //free(fabric_endpoint_resource_description->CertificateName);
                            }
                            free((void*)fabric_endpoint_resource_description->Type);
                        }
                        free((void*)fabric_endpoint_resource_description->Protocol);
                    }
                    free((void*)fabric_endpoint_resource_description->Name);
                }
            }
        }
    }
allok:;
    return result;
}

/* freeing a previously produced FABRIC_CONFIGURATION_PARAMETER_LIST* */
void FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description)
{
    if (fabric_endpoint_resource_description == NULL)
    {
        LogError("invalid argument FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* fabric_endpoint_resource_description=%p", fabric_endpoint_resource_description);
    }
    else
    {
        free((void*)fabric_endpoint_resource_description->CertificateName);
        free((void*)fabric_endpoint_resource_description->Name);
        free((void*)fabric_endpoint_resource_description->Protocol);
        free((void*)fabric_endpoint_resource_description->Type);
    }
}
