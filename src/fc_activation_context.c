// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/hresult_to_string.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_package_com.h"

#include "sf_c_util/fc_activation_context.h"

struct FC_ACTIVATION_CONTEXT_TAG
{
    uint32_t nFabricConfigurationPackages;
    IFabricConfigurationPackage** iFabricConfigurationPackages; /*an array of nFabricConfigurationPackages*/
};

FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_create(int argc, char** argv, int* argc_consumed)
{
    FC_ACTIVATION_CONTEXT_HANDLE result;
    if (
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
        int c_argc;

        result = malloc(sizeof(struct FC_ACTIVATION_CONTEXT_TAG));
        if (result == NULL)
        {
            LogError("failure in malloc");
        }
        else
        {
            result->nFabricConfigurationPackages = 0;
            result->iFabricConfigurationPackages = NULL;

            FC_PACKAGE_HANDLE fc_package;
            bool done = false;
            bool waserror = false;

            while(!done && !waserror)
            {
                fc_package = fc_package_create(argc - *argc_consumed, argv + *argc_consumed, &c_argc);
                if (fc_package == NULL)
                {
                    /*not an error, just indicates we are done */
                    /*wrong, it is an error. Task 16098536: fc_package_create should differentiate betweene ERROR and "does not start with..." will address it*/
                    /*errr... will assume NULL means "we are done"*/
                    done = true;
                }
                else
                {
                    *argc_consumed += c_argc;

                    bool wasMoved = false;
                    IFabricConfigurationPackage* temp_IFabricConfigurationPackage = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
                    if (temp_IFabricConfigurationPackage == NULL)
                    {
                        LogError("failure in COM_WRAPPER_CREATE");
                        waserror = true;
                    }
                    else
                    {
                        wasMoved = true;


                        IFabricConfigurationPackage** temp = realloc_2(result->iFabricConfigurationPackages, (result->nFabricConfigurationPackages + 1), sizeof(IFabricConfigurationPackage*));
                        if (temp == NULL)
                        {
                            LogError("failure in realloc_2");
                            waserror = true;
                        }
                        else
                        {
                            result->iFabricConfigurationPackages = temp;
                            result->iFabricConfigurationPackages[result->nFabricConfigurationPackages] = temp_IFabricConfigurationPackage;
                            result->nFabricConfigurationPackages++;
                            continue;
                        }

                        temp_IFabricConfigurationPackage->lpVtbl->Release(temp_IFabricConfigurationPackage);
                    }

                    wasMoved ? (void)0: fc_package_destroy(fc_package);
                    
                }
            } /*can only exit when waserrror = true or when done==true*/

            if (waserror)
            {
                free(result);
                result = NULL;
            }
            else
            {
                /*all fine*/
            }
        }
    }
     
    return result;
}

void fc_activation_context_destroy(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle)
{
    if (fc_activation_context_handle == NULL)
    {
        LogError("invalid argument FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle=%p", fc_activation_context_handle);
    }
    else
    {
        for (uint32_t i = 0; i < fc_activation_context_handle->nFabricConfigurationPackages; i++)
        {
            fc_activation_context_handle->iFabricConfigurationPackages[i]->lpVtbl->Release(fc_activation_context_handle->iFabricConfigurationPackages[i]);
        }
        free(fc_activation_context_handle->iFabricConfigurationPackages);
        free(fc_activation_context_handle);
    }
    
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
    HRESULT result;
    if (
        (fc_activation_context_handle == NULL) ||
        (names == NULL)
        )
    {
        LogError("invalid argument (FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle=%p, IFabricStringListResult * *names=%p",
            fc_activation_context_handle, names);
        result = E_INVALIDARG;
    }
    else
    {

        result = E_FAIL;
    }
    return result;
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
    HRESULT result;
    if (
        (fc_activation_context_handle == NULL) ||
        (configPackageName == NULL) ||
        (configPackage == NULL)
        )
    {
        LogError("invalid argument FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle=%p, LPCWSTR configPackageName=%ls, IFabricConfigurationPackage * *configPackage=%p",
            fc_activation_context_handle,
            MU_WP_OR_NULL(configPackageName),
            configPackage);
        result = E_INVALIDARG;
    }
    else
    {
        uint32_t i;
        IFabricConfigurationPackage* fc_package = NULL;
        for (i = 0; i < fc_activation_context_handle->nFabricConfigurationPackages; i++)
        {
            fc_package = fc_activation_context_handle->iFabricConfigurationPackages[i];
            if (wcscmp(fc_package->lpVtbl->get_Description(fc_package)->Name, configPackageName) == 0)
            {
                break;
            }
        }

        if (i == fc_activation_context_handle->nFabricConfigurationPackages)
        {
            LogError("could not find the package named %ls", configPackageName);
            result = E_NOT_SET;
        }
        else
        {
            fc_package->lpVtbl->AddRef(fc_package);
            *configPackage = fc_package;

            result = S_OK;
        }
    }
    return result;
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
        HRESULT hr;

        *argc = 0;
        *argv = NULL;
        IFabricStringListResult* fabricStringListResult;

        hr = iFabricCodePackageActivationContext->lpVtbl->GetConfigurationPackageNames(iFabricCodePackageActivationContext, &fabricStringListResult);
        if (FAILED(hr))
        {
            LogHRESULTError(hr, "failure in GetConfigurationPackageNames");
            result = MU_FAILURE;
        }
        else
        {
            ULONG nStrings;
            const wchar_t** strings;

            hr = fabricStringListResult->lpVtbl->GetStrings(fabricStringListResult, &nStrings, &strings);
            if (FAILED(hr))
            {
                LogHRESULTError(hr, "failure in GetStrings");
                result = MU_FAILURE;
            }
            else
            {
                ULONG i;
                bool wasError = false;
                for (i = 0; !wasError && i < nStrings; i++)
                {
                    IFabricConfigurationPackage* configPackage;
                    hr = iFabricCodePackageActivationContext->lpVtbl->GetConfigurationPackage(iFabricCodePackageActivationContext, strings[i], &configPackage);
                    if (FAILED(hr))
                    {
                        wasError = true;
                        LogHRESULTError(hr, "failure in GetConfigurationPackage");
                    }
                    else
                    {
                        int p_argc;
                        char** p_argv;
                        if (IFabricConfigurationPackage_to_ARGC_ARGV(configPackage, &p_argc, &p_argv) != 0)
                        {
                            wasError = true;
                            LogError("failure in IFabricConfigurationPackage_to_ARGC_ARGV");
                        }
                        else
                        {
                            if (ARGC_ARGV_concat(argc, argv, p_argc, p_argv) != 0)
                            {
                                wasError = true;
                                LogError("Failure in ARGC_ARGV_concat");
                            }
                            else
                            {
                                /*all nice keep going*/
                            }

                            ARGC_ARGV_free(p_argc, p_argv);
                        }

                        configPackage->lpVtbl->Release(configPackage);
                    }
                }
                if (wasError)
                {
                    LogError("failing because of previous logged error");
                    result = MU_FAILURE;
                }
                else
                {
                    result = 0;
                }
            }

            fabricStringListResult->lpVtbl->Release(fabricStringListResult);
        }

    }
    return result;
}
