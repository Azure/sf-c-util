// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef FC_ACTIVATION_CONTEXT_H
#define FC_ACTIVATION_CONTEXT_H


#include "windows.h"
#include "fabriccommon.h"
#include "fabricruntime.h"
#include "fabrictypes.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct FC_ACTIVATION_CONTEXT_TAG* FC_ACTIVATION_CONTEXT_HANDLE;

    /*a FC_ACTIVATION_CONTEXT_HANDLE (aka IFabricCodePackageActivationContext) is serialized to argc/argv as list of IFabricConfigurationPackage + a list of ...*/

    MOCKABLE_INTERFACE(fc_activation_context,
        FUNCTION(, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_create, int, argc, char**, argv, int*, argc_consumed),
        FUNCTION(, void, fc_activation_context_destroy, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, LPCWSTR, get_ContextId, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, LPCWSTR, get_CodePackageName, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, LPCWSTR, get_CodePackageVersion, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, LPCWSTR, get_WorkDirectory, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, LPCWSTR, get_LogDirectory, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, LPCWSTR, get_TempDirectory, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, const FABRIC_SERVICE_TYPE_DESCRIPTION_LIST*, get_ServiceTypes, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, const FABRIC_SERVICE_GROUP_TYPE_DESCRIPTION_LIST*, get_ServiceGroupTypes, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, const FABRIC_APPLICATION_PRINCIPALS_DESCRIPTION*, get_ApplicationPrincipals, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST*, get_ServiceEndpointResources, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle),
        FUNCTION(, HRESULT, GetServiceEndpointResource, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ LPCWSTR, serviceEndpointResourceName,
            /* [retval][out] */ const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION**, bufferedValue), 
        FUNCTION(, HRESULT, GetCodePackageNames, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [retval][out] */ IFabricStringListResult**, names),
        FUNCTION(, HRESULT, GetConfigurationPackageNames, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [retval][out] */ IFabricStringListResult**, names), 
        FUNCTION(, HRESULT, GetDataPackageNames, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [retval][out] */ IFabricStringListResult**, names), 
        FUNCTION(, HRESULT, GetCodePackage, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ LPCWSTR, codePackageName,
            /* [retval][out] */ IFabricCodePackage**, codePackage), 
        FUNCTION(, HRESULT, GetConfigurationPackage, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ LPCWSTR, configPackageName,
            /* [retval][out] */ IFabricConfigurationPackage**, configPackage), 
        FUNCTION(, HRESULT, GetDataPackage, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ LPCWSTR, dataPackageName,
            /* [retval][out] */ IFabricDataPackage**, dataPackage), 
        FUNCTION(, HRESULT, RegisterCodePackageChangeHandler, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ IFabricCodePackageChangeHandler*, callback,
            /* [retval][out] */ LONGLONG*, callbackHandle), 
        FUNCTION(, HRESULT, UnregisterCodePackageChangeHandler, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ LONGLONG, callbackHandle), 
        FUNCTION(, HRESULT, RegisterConfigurationPackageChangeHandler, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ IFabricConfigurationPackageChangeHandler*, callback,
            /* [retval][out] */ LONGLONG*, callbackHandle),
        FUNCTION(, HRESULT, UnregisterConfigurationPackageChangeHandler, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ LONGLONG, callbackHandle),
        FUNCTION(, HRESULT, RegisterDataPackageChangeHandler, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ IFabricDataPackageChangeHandler*, callback,
            /* [retval][out] */ LONGLONG*, callbackHandle), 
        FUNCTION(, HRESULT, UnregisterDataPackageChangeHandler, FC_ACTIVATION_CONTEXT_HANDLE, fc_activation_context_handle,
            /* [in] */ LONGLONG, callbackHandle)
    )

    /* IFabricCodePackageActivationContext => argc/argv */
    MOCKABLE_FUNCTION(, int, IFabricCodePackageActivationContext_to_ARGC_ARGV, IFabricCodePackageActivationContext*, iFabricCodePackageActivationContext, int*, argc, char***, argv);

    /*argc/argv = > IFabricConfigurationPackage * sort of "factory" :). Handled by fc_create above in MOCKABLE_INTERFACE(fc_package,... */
    /*freeing a previously produced IFabricConfigurationPackage* => done by COM means, it ends up eventually calling fc_package_destroy */

#ifdef __cplusplus
}
#endif

#endif /* FC_ACTIVATION_CONTEXT_H */
