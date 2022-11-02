// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_FABRIC_CONFIGURATION_ACTIVATION_CONTEXT_H
#define REAL_FABRIC_CONFIGURATION_ACTIVATION_CONTEXT_H

#include <stdint.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_FABRIC_CONFIGURATION_ACTIVATION_CONTEXT_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        fc_activation_context_create, \
        fc_activation_context_destroy,   \
        get_ContextId, \
        get_CodePackageName, \
        get_CodePackageVersion, \
        get_WorkDirectory, \
        get_LogDirectory, \
        get_TempDirectory, \
        get_ServiceTypes, \
        get_ServiceGroupTypes, \
        get_ApplicationPrincipals, \
        get_ServiceEndpointResources, \
        GetServiceEndpointResource, \
        GetCodePackageNames, \
        GetConfigurationPackageNames, \
        GetDataPackageNames, \
        GetCodePackage, \
        GetConfigurationPackage, \
        GetDataPackage, \
        RegisterCodePackageChangeHandler, \
        UnregisterCodePackageChangeHandler, \
        RegisterConfigurationPackageChangeHandler, \
        UnregisterConfigurationPackageChangeHandler, \
        RegisterDataPackageChangeHandler, \
        UnregisterDataPackageChangeHandler, \
        IFabricCodePackageActivationContext_to_ARGC_ARGV \
)

#include "sf_c_util/fc_activation_context.h"

FC_ACTIVATION_CONTEXT_HANDLE real_fc_activation_context_create(int argc, char** argv, int* argc_consumed);
void real_fc_activation_context_destroy(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
LPCWSTR real_get_ContextId(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
LPCWSTR real_get_CodePackageName(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
LPCWSTR real_get_CodePackageVersion(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
LPCWSTR real_get_WorkDirectory(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
LPCWSTR real_get_LogDirectory(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
LPCWSTR real_get_TempDirectory(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
const FABRIC_SERVICE_TYPE_DESCRIPTION_LIST* real_get_ServiceTypes(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
const FABRIC_SERVICE_GROUP_TYPE_DESCRIPTION_LIST* real_get_ServiceGroupTypes(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
const FABRIC_APPLICATION_PRINCIPALS_DESCRIPTION* real_get_ApplicationPrincipals(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* real_get_ServiceEndpointResources(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle);
HRESULT real_GetServiceEndpointResource(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR serviceEndpointResourceName,
    /* [retval][out] */ const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION** bufferedValue);
HRESULT real_GetCodePackageNames(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [retval][out] */ IFabricStringListResult** names);
HRESULT real_GetConfigurationPackageNames(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [retval][out] */ IFabricStringListResult** names);
HRESULT real_GetDataPackageNames(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [retval][out] */ IFabricStringListResult** names);
HRESULT real_GetCodePackage(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR codePackageName,
    /* [retval][out] */ IFabricCodePackage** codePackage);
HRESULT real_GetConfigurationPackage(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR configPackageName,
    /* [retval][out] */ IFabricConfigurationPackage** configPackage);
HRESULT real_GetDataPackage(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR dataPackageName,
    /* [retval][out] */ IFabricDataPackage** dataPackage);
HRESULT real_RegisterCodePackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ IFabricCodePackageChangeHandler* callback,
    /* [retval][out] */ LONGLONG* callbackHandle);
HRESULT real_UnregisterCodePackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LONGLONG callbackHandle);
HRESULT real_RegisterConfigurationPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ IFabricConfigurationPackageChangeHandler* callback,
    /* [retval][out] */ LONGLONG* callbackHandle);
HRESULT real_UnregisterConfigurationPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LONGLONG callbackHandle);
HRESULT real_RegisterDataPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ IFabricDataPackageChangeHandler* callback,
    /* [retval][out] */ LONGLONG* callbackHandle);
HRESULT real_UnregisterDataPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LONGLONG callbackHandle);

int real_IFabricCodePackageActivationContext_to_ARGC_ARGV(IFabricCodePackageActivationContext* iFabricCodePackageActivationContext, int* argc, char*** argv);

#endif //REAL_FABRIC_CONFIGURATION_ACTIVATION_CONTEXT_H
