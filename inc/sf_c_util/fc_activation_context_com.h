// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FC_ACTIVATION_CONTEXT_COM_H
#define FC_ACTIVATION_CONTEXT_COM_H

#include "windows.h"
#include "fabriccommon.h"
#include "unknwn.h"

#include "com_wrapper/com_wrapper.h"

#include "fc_activation_context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FC_ACTIVATION_CONTEXT_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricCodePackageActivationContext, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, get_ContextId) ,\
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, get_CodePackageName), \
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, get_CodePackageVersion), \
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, get_WorkDirectory), \
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, get_LogDirectory), \
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, get_TempDirectory), \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_SERVICE_TYPE_DESCRIPTION_LIST*, get_ServiceTypes), \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_SERVICE_GROUP_TYPE_DESCRIPTION_LIST*, get_ServiceGroupTypes), \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_APPLICATION_PRINCIPALS_DESCRIPTION*, get_ApplicationPrincipals), \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST*, get_ServiceEndpointResources), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetServiceEndpointResource, \
            /* [in] */ LPCWSTR, serviceEndpointResourceName,                                    \
            /* [retval][out] */ const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION**, bufferedValue),   \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetCodePackageNames,  \
            /* [retval][out] */ IFabricStringListResult**, names),  \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetConfigurationPackageNames,  \
            /* [retval][out] */ IFabricStringListResult**, names), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetDataPackageNames, \
            /* [retval][out] */ IFabricStringListResult**, names), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetCodePackage, \
            /* [in] */ LPCWSTR, codePackageName,                    \
            /* [retval][out] */ IFabricCodePackage**, codePackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetConfigurationPackage, \
            /* [in] */ LPCWSTR, configPackageName,  \
            /* [retval][out] */ IFabricConfigurationPackage**, configPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetDataPackage,               \
            /* [in] */ LPCWSTR, dataPackageName,                        \
            /* [retval][out] */ IFabricDataPackage**, dataPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, RegisterCodePackageChangeHandler, \
            /* [in] */ IFabricCodePackageChangeHandler*, callback,  \
            /* [retval][out] */ LONGLONG*, callbackHandle), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, UnregisterCodePackageChangeHandler, \
            /* [in] */ LONGLONG, callbackHandle), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, RegisterConfigurationPackageChangeHandler, \
            /* [in] */ IFabricConfigurationPackageChangeHandler*, callback, \
            /* [retval][out] */ LONGLONG*, callbackHandle),  \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, UnregisterConfigurationPackageChangeHandler, \
            /* [in] */ LONGLONG, callbackHandle), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, RegisterDataPackageChangeHandler, \
            /* [in] */ IFabricDataPackageChangeHandler*, callback,  \
            /* [retval][out] */ LONGLONG*, callbackHandle), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, UnregisterDataPackageChangeHandler, \
            /* [in] */ LONGLONG, callbackHandle) \
    )

    DECLARE_COM_WRAPPER_OBJECT(FC_ACTIVATION_CONTEXT_HANDLE, FC_ACTIVATION_CONTEXT_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FC_ACTIVATION_CONTEXT_COM_H */
