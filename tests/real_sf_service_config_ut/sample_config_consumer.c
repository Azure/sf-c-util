// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>

#include "windows.h"
#include "fabricruntime.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/thandle.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/fc_activation_context.h"
#include "sf_c_util/fc_activation_context_com.h"
#include "sf_c_util/sf_service_config.h"

#include "sample_service_config.h"
#include "sample_config_consumer.h"

THANDLE(SF_SERVICE_CONFIG(sample_service)) sample_config_consumer_create(int argc, char** argv)
{
    THANDLE(SF_SERVICE_CONFIG(sample_service)) result = NULL;
    int argc_consumed;
    FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    if (fc_activation_context == NULL)
    {
        LogError("failure in fc_activation_context_create(argc=%d, argv=%p, &argc_consumed)", argc, argv);
    }
    else
    {
        IFabricCodePackageActivationContext* activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, fc_activation_context, fc_activation_context_destroy);
        if (activation_context == NULL)
        {
            LogError("failure in COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, ...)");
            fc_activation_context_destroy(fc_activation_context);
        }
        else
        {
            THANDLE(SF_SERVICE_CONFIG(sample_service)) sf_config = SF_SERVICE_CONFIG_CREATE(sample_service)(activation_context);
            if (sf_config == NULL)
            {
                LogError("failure in SF_SERVICE_CONFIG_CREATE(sample_service)(activation_context=%p)", activation_context);
            }
            else
            {
                THANDLE_INITIALIZE_MOVE(SF_SERVICE_CONFIG(sample_service))(&result, &sf_config);
            }
            (void)activation_context->lpVtbl->Release(activation_context);
        }
    }
    return result;
}
