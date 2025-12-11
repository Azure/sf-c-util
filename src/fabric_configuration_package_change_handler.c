// Copyright (C) Microsoft Corporation. All rights reserved.

#include <stdlib.h>

#include "windows.h"

#include "fabricruntime.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fabric_configuration_package_change_handler.h"

typedef struct FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG
{
    ON_CONFIGURATION_CHANGED on_configuration_changed;
    void* context;
} FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER;

FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE fabric_configuration_package_change_handler_create(
    ON_CONFIGURATION_CHANGED on_configuration_changed,
    void* context)
{
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_002: [ context is allowed to be NULL. ]*/

    if (
        /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_001: [ If on_configuration_changed is NULL, fabric_configuration_package_change_handler_create shall fail and return NULL. ]*/
        (on_configuration_changed == NULL)
        )
    {
        LogError("Invalid arguments: ON_CONFIGURATION_CHANGED on_configuration_changed=%p, void* context=%p",
            on_configuration_changed, context);
        result = NULL;
    }
    else
    {
        /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_003: [ fabric_configuration_package_change_handler_create shall allocate memory for the handler structure. ]*/
        result = malloc(sizeof(FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER));
        if (result == NULL)
        {
            /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_004: [ If memory allocation fails, fabric_configuration_package_change_handler_create shall fail and return NULL. ]*/
            LogError("malloc failed");
        }
        else
        {
            /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [ fabric_configuration_package_change_handler_create shall store the on_configuration_changed callback and context. ]*/
            result->on_configuration_changed = on_configuration_changed;
            result->context = context;

            /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [ fabric_configuration_package_change_handler_create shall succeed and return the handler. ]*/
            goto all_ok;
        }
    }

    result = NULL;

all_ok:
    return result;
}

void fabric_configuration_package_change_handler_destroy(FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle)
{
    if (handle == NULL)
    {
        /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [ If handle is NULL, fabric_configuration_package_change_handler_destroy shall return. ]*/
        LogError("Invalid arguments: FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle=%p", handle);
    }
    else
    {
        /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [ fabric_configuration_package_change_handler_destroy shall free the handler structure. ]*/
        free(handle);
    }
}

void fabric_configuration_package_change_handler_on_package_added(
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* configPackage)
{
    /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [ fabric_configuration_package_change_handler_on_package_added shall do nothing. ]*/
    (void)handle;
    (void)source;
    (void)configPackage;
}

void fabric_configuration_package_change_handler_on_package_removed(
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* configPackage)
{
    /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [ fabric_configuration_package_change_handler_on_package_removed shall do nothing. ]*/
    (void)handle;
    (void)source;
    (void)configPackage;
}

void fabric_configuration_package_change_handler_on_package_modified(
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previousConfigPackage,
    IFabricConfigurationPackage* configPackage)
{
    if (handle == NULL)
    {
        /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If handle is NULL, fabric_configuration_package_change_handler_on_package_modified shall return. ]*/
        LogError("Invalid arguments: FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle=%p, IFabricCodePackageActivationContext* source=%p, IFabricConfigurationPackage* previousConfigPackage=%p, IFabricConfigurationPackage* configPackage=%p",
            handle, source, previousConfigPackage, configPackage);
    }
    else
    {
        /*Codes_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [ fabric_configuration_package_change_handler_on_package_modified shall call the on_configuration_changed callback with previous_config_package set to previousConfigPackage and new_config_package set to configPackage. ]*/
        handle->on_configuration_changed(handle->context, source, previousConfigPackage, configPackage);
    }
}
