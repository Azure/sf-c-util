// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_package.h"

#include "sf_c_util/fc_activation_context.h"

struct FC_ACTIVATION_CONTEXT_TAG
{
    FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION fabric_configuration_package_description;
    FABRIC_CONFIGURATION_SETTINGS fabric_configuration_settings;
};

FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_create(int argc, char** argv, int* argc_consumed)
{
    FC_ACTIVATION_CONTEXT_HANDLE result;
    if (
        (argc < 2) ||
        (argv == NULL) ||
        (argc_consumed == NULL)
        )
    {
        LogError("invalid argument int argc=%d, char** argv=%p, int* argc_consumed=%p", argc, argv, argc_consumed);
        result = NULL;
    }
    else
    {
        result = NULL;
    }
     
    return result;
}

void fc_activation_context_destroy(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
}

LPCWSTR get_ContextId(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    return NULL;
}
