// Copyright (C) Microsoft Corporation. All rights reserved.

#include <stdlib.h>

#include "windows.h"

#include "fabricruntime.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/hresult_to_string.h"

#include "sf_c_util/configuration_package_change_handler.h"
#include "sf_c_util/configuration_package_change_handler_com.h"

typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG
{
    IFabricCodePackageActivationContext* activation_context;
    LONGLONG callback_handle;
    ON_CONFIGURATION_CHANGED on_configuration_changed;
    void* context;
} CONFIGURATION_PACKAGE_CHANGE_HANDLER;

DEFINE_COM_WRAPPER_OBJECT(CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE_INTERFACES);

CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE configuration_package_change_handler_create(
    IFabricCodePackageActivationContext* activation_context,
    ON_CONFIGURATION_CHANGED on_configuration_changed,
    void* context)
{
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_003: [ context is allowed to be NULL. ]*/

    if (
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_001: [ If activation_context is NULL, configuration_package_change_handler_create shall fail and return NULL. ]*/
        (activation_context == NULL) ||
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_002: [ If on_configuration_changed is NULL, configuration_package_change_handler_create shall fail and return NULL. ]*/
        (on_configuration_changed == NULL)
        )
    {
        LogError("Invalid arguments: IFabricCodePackageActivationContext* activation_context=%p, ON_CONFIGURATION_CHANGED on_configuration_changed=%p, void* context=%p",
            activation_context, on_configuration_changed, context);
        result = NULL;
    }
    else
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_004: [ configuration_package_change_handler_create shall allocate memory for the handler structure. ]*/
        result = malloc(sizeof(CONFIGURATION_PACKAGE_CHANGE_HANDLER));
        if (result == NULL)
        {
            /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
            LogError("malloc failed");
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [ configuration_package_change_handler_create shall store the on_configuration_changed callback and context. ]*/
            result->on_configuration_changed = on_configuration_changed;
            result->context = context;
            result->callback_handle = 0;

            /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [ configuration_package_change_handler_create shall create a COM wrapper for IFabricConfigurationPackageChangeHandler using COM_WRAPPER_CREATE. ]*/
            IFabricConfigurationPackageChangeHandler* com_handler = COM_WRAPPER_CREATE(
                CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE,
                IFabricConfigurationPackageChangeHandler,
                result,
                configuration_package_change_handler_destroy);
            if (com_handler == NULL)
            {
                /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
                LogError("COM_WRAPPER_CREATE failed");
                free(result);
                result = NULL;
            }
            else
            {
                LONGLONG callback_handle;
                /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [ configuration_package_change_handler_create shall register the COM handler with Service Fabric by calling activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler. ]*/
                HRESULT hr = activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler(
                    activation_context,
                    com_handler,
                    &callback_handle);
                if (FAILED(hr))
                {
                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
                    LogHRESULTError(hr, "RegisterConfigurationPackageChangeHandler failed");
                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [ If RegisterConfigurationPackageChangeHandler fails, configuration_package_change_handler_create shall release the COM wrapper and free resources. ]*/
                    (void)com_handler->lpVtbl->Release(com_handler);
                    result = NULL;
                }
                else
                {
                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [ configuration_package_change_handler_create shall store the callback handle returned by RegisterConfigurationPackageChangeHandler. ]*/
                    result->callback_handle = callback_handle;
                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [ configuration_package_change_handler_create shall call AddRef on activation_context to take a reference. ]*/
                    (void)activation_context->lpVtbl->AddRef(activation_context);
                    result->activation_context = activation_context;

                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [ configuration_package_change_handler_create shall succeed and return the handler. ]*/
                    goto all_ok;
                }
            }
        }
    }

    result = NULL;

all_ok:
    return result;
}

void configuration_package_change_handler_destroy(CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle)
{
    if (handle == NULL)
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_013: [ If handle is NULL, configuration_package_change_handler_destroy shall return. ]*/
        LogError("Invalid arguments: CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle=%p", handle);
    }
    else
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_014: [ configuration_package_change_handler_destroy shall unregister from Service Fabric by calling activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler with the stored callback handle. ]*/
        HRESULT hr = handle->activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler(
            handle->activation_context,
            handle->callback_handle);
        if (FAILED(hr))
        {
            LogHRESULTError(hr, "UnregisterConfigurationPackageChangeHandler failed");
        }

        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_015: [ configuration_package_change_handler_destroy shall call Release on the stored activation_context. ]*/
        (void)handle->activation_context->lpVtbl->Release(handle->activation_context);

        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_016: [ configuration_package_change_handler_destroy shall free the handler structure. ]*/
        free(handle);
    }
}

void configuration_package_change_handler_on_package_added(
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* configPackage)
{
    if (handle == NULL)
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_017: [ If handle is NULL, configuration_package_change_handler_on_package_added shall return. ]*/
        LogError("Invalid arguments: CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle=%p, IFabricCodePackageActivationContext* source=%p, IFabricConfigurationPackage* configPackage=%p",
            handle, source, configPackage);
    }
    else
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_018: [ configuration_package_change_handler_on_package_added shall call the on_configuration_changed callback with previous_config_package set to NULL and new_config_package set to configPackage. ]*/
        handle->on_configuration_changed(handle->context, source, NULL, configPackage);
    }
}

void configuration_package_change_handler_on_package_removed(
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* configPackage)
{
    if (handle == NULL)
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_019: [ If handle is NULL, configuration_package_change_handler_on_package_removed shall return. ]*/
        LogError("Invalid arguments: CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle=%p, IFabricCodePackageActivationContext* source=%p, IFabricConfigurationPackage* configPackage=%p",
            handle, source, configPackage);
    }
    else
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_020: [ configuration_package_change_handler_on_package_removed shall call the on_configuration_changed callback with previous_config_package set to configPackage and new_config_package set to NULL. ]*/
        handle->on_configuration_changed(handle->context, source, configPackage, NULL);
    }
}

void configuration_package_change_handler_on_package_modified(
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previousConfigPackage,
    IFabricConfigurationPackage* configPackage)
{
    if (handle == NULL)
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_021: [ If handle is NULL, configuration_package_change_handler_on_package_modified shall return. ]*/
        LogError("Invalid arguments: CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle=%p, IFabricCodePackageActivationContext* source=%p, IFabricConfigurationPackage* previousConfigPackage=%p, IFabricConfigurationPackage* configPackage=%p",
            handle, source, previousConfigPackage, configPackage);
    }
    else
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_022: [ configuration_package_change_handler_on_package_modified shall call the on_configuration_changed callback with previous_config_package set to previousConfigPackage and new_config_package set to configPackage. ]*/
        handle->on_configuration_changed(handle->context, source, previousConfigPackage, configPackage);
    }
}
