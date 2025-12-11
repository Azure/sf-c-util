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

#include "sf_c_util/fabric_configuration_package_change_handler.h"
#include "sf_c_util/fabric_configuration_package_change_handler_com.h"
#include "sf_c_util/configuration_package_change_handler.h"

typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG
{
    IFabricCodePackageActivationContext* activation_context;
    LONGLONG callback_handle;
    IFabricConfigurationPackageChangeHandler* com_handler;
} CONFIGURATION_PACKAGE_CHANGE_HANDLER;

CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE configuration_package_change_handler_create(
    IFabricCodePackageActivationContext* activation_context,
    ON_CONFIGURATION_CHANGED on_configuration_changed,
    void* context)
{
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

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
            LogError("malloc(sizeof(CONFIGURATION_PACKAGE_CHANGE_HANDLER)=%zu) failed", sizeof(CONFIGURATION_PACKAGE_CHANGE_HANDLER));
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [ configuration_package_change_handler_create shall call fabric_configuration_package_change_handler_create with the on_configuration_changed callback and context to create the core handler. ]*/
            FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE fabric_handler = fabric_configuration_package_change_handler_create(
                on_configuration_changed,
                context);
            if (fabric_handler == NULL)
            {
                /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
                LogError("fabric_configuration_package_change_handler_create failed");
                free(result);
                result = NULL;
            }
            else
            {
                /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [ configuration_package_change_handler_create shall create a COM wrapper for IFabricConfigurationPackageChangeHandler using COM_WRAPPER_CREATE with the FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE. ]*/
                IFabricConfigurationPackageChangeHandler* com_handler = COM_WRAPPER_CREATE(
                    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE,
                    IFabricConfigurationPackageChangeHandler,
                    fabric_handler,
                    fabric_configuration_package_change_handler_destroy);
                if (com_handler == NULL)
                {
                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
                    LogError("COM_WRAPPER_CREATE failed");
                    fabric_configuration_package_change_handler_destroy(fabric_handler);
                    free(result);
                    result = NULL;
                }
                else
                {
                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [ configuration_package_change_handler_create shall call AddRef on activation_context to take a reference. ]*/
                    (void)activation_context->lpVtbl->AddRef(activation_context);
                    result->activation_context = activation_context;
                    result->com_handler = com_handler;

                    LONGLONG callback_handle;
                    /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [ configuration_package_change_handler_create shall register the COM handler with Service Fabric by calling activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler. ]*/
                    HRESULT hr = activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler(
                        activation_context,
                        com_handler,
                        &callback_handle);
                    if (FAILED(hr))
                    {
                        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
                        LogHRESULTError(hr, "RegisterConfigurationPackageChangeHandler failed");
                        (void)com_handler->lpVtbl->Release(com_handler);
                        (void)activation_context->lpVtbl->Release(activation_context);
                        free(result);
                        result = NULL;
                    }
                    else
                    {
                        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [ configuration_package_change_handler_create shall store the callback handle returned by RegisterConfigurationPackageChangeHandler. ]*/
                        result->callback_handle = callback_handle;

                        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [ configuration_package_change_handler_create shall succeed and return the handler. ]*/
                        goto all_ok;
                    }
                }
            }
        }
    }

all_ok:
    return result;
}

void configuration_package_change_handler_destroy(CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle)
{
    if (handle == NULL)
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [ If handle is NULL, configuration_package_change_handler_destroy shall return. ]*/
        LogError("Invalid arguments: CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle=%p", handle);
    }
    else
    {
        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_013: [ configuration_package_change_handler_destroy shall unregister from Service Fabric by calling activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler with the stored callback handle. ]*/
        HRESULT hr = handle->activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler(
            handle->activation_context,
            handle->callback_handle);
        if (FAILED(hr))
        {
            LogHRESULTError(hr, "UnregisterConfigurationPackageChangeHandler failed");
        }

        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_014: [ configuration_package_change_handler_destroy shall call Release on the COM wrapper (which will trigger destruction of the underlying fabric handler). ]*/
        (void)handle->com_handler->lpVtbl->Release(handle->com_handler);

        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_015: [ configuration_package_change_handler_destroy shall call Release on the stored activation_context. ]*/
        (void)handle->activation_context->lpVtbl->Release(handle->activation_context);

        /*Codes_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_016: [ configuration_package_change_handler_destroy shall free the handler structure. ]*/
        free(handle);
    }
}

