// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

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
    int dummy;
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
        *argc_consumed = 0;
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
    LogError("Not implemented");
    return NULL;
}

LPCWSTR get_CodePackageName(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}

LPCWSTR get_CodePackageVersion(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}

LPCWSTR get_WorkDirectory(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}
LPCWSTR get_LogDirectory(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}
LPCWSTR get_TempDirectory(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}

const FABRIC_SERVICE_TYPE_DESCRIPTION_LIST* get_ServiceTypes(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}

const FABRIC_SERVICE_GROUP_TYPE_DESCRIPTION_LIST* get_ServiceGroupTypes(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}

const FABRIC_APPLICATION_PRINCIPALS_DESCRIPTION* get_ApplicationPrincipals(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented");
    return NULL;
}

const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* get_ServiceEndpointResources(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    (void)fc_activation_context_handle;
    LogError("Not implemented but will be.");
    return NULL;
}

HRESULT GetServiceEndpointResource(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR serviceEndpointResourceName,
    /* [retval][out] */ const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION** bufferedValue)
{
    (void)fc_activation_context_handle;
    (void)serviceEndpointResourceName;
    (void)bufferedValue;
    LogError("Not implemented but will be.");
    return E_NOTIMPL;
}

HRESULT GetCodePackageNames(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [retval][out] */ IFabricStringListResult** names)
{
    (void)fc_activation_context_handle;
    (void)names;
    LogError("Not implemented");
    return E_NOTIMPL;
}

HRESULT GetConfigurationPackageNames(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [retval][out] */ IFabricStringListResult** names)
{
    (void)fc_activation_context_handle;
    (void)names;
    LogError("Not implemented but will be.");
    return E_NOTIMPL;
}

HRESULT GetDataPackageNames(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [retval][out] */ IFabricStringListResult** names)
{
    (void)fc_activation_context_handle;
    (void)names;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

HRESULT GetCodePackage(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR codePackageName,
    /* [retval][out] */ IFabricCodePackage** codePackage)
{
    (void)fc_activation_context_handle;
    (void)codePackageName;
    (void)codePackage;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

HRESULT GetConfigurationPackage(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR configPackageName,
    /* [retval][out] */ IFabricConfigurationPackage** configPackage)
{
    (void)fc_activation_context_handle;
    (void)configPackageName;
    (void)configPackage;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

HRESULT GetDataPackage(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR dataPackageName,
    /* [retval][out] */ IFabricDataPackage** dataPackage)
{
    (void)fc_activation_context_handle;
    (void)dataPackageName;
    (void)dataPackage;
    LogError("Not implemented.");
    return E_NOTIMPL;
}


HRESULT RegisterCodePackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ IFabricCodePackageChangeHandler* callback,
    /* [retval][out] */ LONGLONG* callbackHandle)
{
    (void)fc_activation_context_handle;
    (void)callback;
    (void)callbackHandle;
    LogError("Not implemented.");
    return E_NOTIMPL;
}


HRESULT UnregisterCodePackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LONGLONG callbackHandle)
{
    (void)fc_activation_context_handle;
    (void)callbackHandle;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

HRESULT RegisterConfigurationPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ IFabricConfigurationPackageChangeHandler* callback,
    /* [retval][out] */ LONGLONG* callbackHandle)
{
    (void)fc_activation_context_handle;
    (void)callback;
    (void)callbackHandle;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

HRESULT UnregisterConfigurationPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LONGLONG callbackHandle)
{
    (void)fc_activation_context_handle;
    (void)callbackHandle;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

HRESULT RegisterDataPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ IFabricDataPackageChangeHandler* callback,
    /* [retval][out] */ LONGLONG* callbackHandle)
{
    (void)fc_activation_context_handle;
    (void)callback;
    (void)callbackHandle;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

HRESULT UnregisterDataPackageChangeHandler(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LONGLONG callbackHandle)
{
    (void)fc_activation_context_handle;
    (void)callbackHandle;
    LogError("Not implemented.");
    return E_NOTIMPL;
}

int IFabricCodePackageActivationContext_to_ARGC_ARGV(IFabricCodePackageActivationContext* iFabricCodePackageActivationContext, int* argc, char*** argv)
{
    int result;
    if (
        (iFabricCodePackageActivationContext == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument IFabricCodePackageActivationContext* iFabricCodePackageActivationContext=%p, int* argc=%p, char*** argv=%p",
            iFabricCodePackageActivationContext, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        result = MU_FAILURE;
    }
    return result;
}
