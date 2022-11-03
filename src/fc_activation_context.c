// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "windows.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/hresult_to_string.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_package.h"
#include "sf_c_util/fc_package_com.h"
#include "sf_c_util/fabric_string_list_result.h"
#include "sf_c_util/fabric_string_list_result_com.h"
#include "sf_c_util/fc_erdl_argc_argv.h"

#include "sf_c_util/fc_activation_context.h"

struct FC_ACTIVATION_CONTEXT_TAG
{
    uint32_t nFabricConfigurationPackages;
    IFabricConfigurationPackage** iFabricConfigurationPackages; /*an array of nFabricConfigurationPackages*/
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST fabric_endpoint_resource_description_list;
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
                /*see if there are endpoint resources here...*/
                ARGC_ARGV_DATA_RESULT r;
                r = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(argc - *argc_consumed, argv + *argc_consumed, &result->fabric_endpoint_resource_description_list, &c_argc);
                switch (r)
                {
                    case ARGC_ARGV_DATA_OK:
                    {
                        *argc_consumed += c_argc;
                        /*all fine*/
                        break;
                    }
                    case ARGC_ARGV_DATA_INVALID:
                    {
                        /*all fine*/
                        result->fabric_endpoint_resource_description_list.Count = 0;
                        result->fabric_endpoint_resource_description_list.Items = NULL;
                        break;
                    }
                    default:
                    case ARGC_ARGV_DATA_ERROR:
                    {
                        LogError("failure in FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV, it returned %" PRI_MU_ENUM "", MU_ENUM_VALUE(ARGC_ARGV_DATA_RESULT, r));
                        free(result); /*very likely MORE handling is needed here for cleanup*/
                        result = NULL; /*return it*/
                        break;
                    }

                }
                
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
    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* result;
    if (fc_activation_context_handle == NULL)
    {
        LogError("invalid argument FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle=%p", fc_activation_context_handle);
        result = NULL;
    }
    else
    {
        result = &fc_activation_context_handle->fabric_endpoint_resource_description_list;
    }
    return result;
}

HRESULT GetServiceEndpointResource(FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle,
    /* [in] */ LPCWSTR serviceEndpointResourceName,
    /* [retval][out] */ const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION** bufferedValue)
{
    HRESULT result;
    if (
        (fc_activation_context_handle == NULL) ||
        (serviceEndpointResourceName == NULL) ||
        (bufferedValue == NULL)
        )
    {
        LogError("invalid argument  FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context_handle=%p, LPCWSTR serviceEndpointResourceName=%ls, const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION * *bufferedValue=%p",
            fc_activation_context_handle,
            MU_WP_OR_NULL(serviceEndpointResourceName),
            bufferedValue);
        result = E_INVALIDARG;
    }
    else
    {
        ULONG i;
        for (i = 0; i < fc_activation_context_handle->fabric_endpoint_resource_description_list.Count; i++)
        {
            if (wcscmp(fc_activation_context_handle->fabric_endpoint_resource_description_list.Items[i].Name, serviceEndpointResourceName) == 0)
            {
                *bufferedValue = fc_activation_context_handle->fabric_endpoint_resource_description_list.Items + i;
                break;
            }
        }

        if (i == fc_activation_context_handle->fabric_endpoint_resource_description_list.Count)
        {
            result = E_NOT_SET;
        }
        else
        {
            result = S_OK;
        }

    }
    return result;
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

        const wchar_t** allnames = malloc_2(fc_activation_context_handle->nFabricConfigurationPackages, sizeof(wchar_t*));
        if (allnames == NULL)
        {
            LogError("failure in amlloc_2");
            result = E_FAIL;
        }
        else
        {
            for (ULONG i = 0; i < fc_activation_context_handle->nFabricConfigurationPackages; i++)
            {
                IFabricConfigurationPackage* fabricConfigurationPackage = fc_activation_context_handle->iFabricConfigurationPackages[i]; /*shortcut*/
                allnames[i] = fabricConfigurationPackage->lpVtbl->get_Description(fabricConfigurationPackage)->Name; /*fabric_string_list_result_create copies, we don't have to*/
            }

            FABRIC_STRING_LIST_RESULT_HANDLE fabric_string_list_result = fabric_string_list_result_create(fc_activation_context_handle->nFabricConfigurationPackages, allnames);
            if (fabric_string_list_result == NULL)
            {
                LogError("failure in fabric_string_list_result_create");
                result = E_FAIL;
            }
            else
            {
                *names = COM_WRAPPER_CREATE(FABRIC_STRING_LIST_RESULT_HANDLE, IFabricStringListResult, fabric_string_list_result, fabric_string_list_result_destroy);
                if (*names == NULL)
                {
                    LogError("failure in COM_WRAPPER_CREATE");
                    result = E_FAIL;
                }
                else
                {
                    result = S_OK;
                }

            }
            free(allnames);
        }
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
                    ARGC_ARGV_free(*argc, *argv);
                    LogError("failing because of previous logged error");
                    result = MU_FAILURE;
                }
                else
                {
                    /*add the rest of the serviceEndpointDescriptions*/
                    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* list = iFabricCodePackageActivationContext->lpVtbl->get_ServiceEndpointResources(iFabricCodePackageActivationContext);
                    if (list == NULL)
                    {
                        LogError("failure in get_ServiceEndpointResources");
                        result = MU_FAILURE;
                    }
                    else
                    {
                        int p_argc;
                        char** p_argv;
                        if (FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV(list, &p_argc, &p_argv) != 0)
                        {
                            LogError("failure in FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV");
                            result = MU_FAILURE;
                        }
                        else
                        {
                            if (ARGC_ARGV_concat(argc, argv, p_argc, p_argv) != 0)
                            {
                                LogError("failure in ARGC_ARGV_concat");
                                ARGC_ARGV_free(p_argc, p_argv);
                                result = MU_FAILURE;
                            }
                            else
                            {
                                ARGC_ARGV_free(p_argc, p_argv);
                                result = 0;
                            }
                        }
                    }
                    result != 0 ? ARGC_ARGV_free(*argc, *argv) : (void)0;
                }
            }
            fabricStringListResult->lpVtbl->Release(fabricStringListResult);
        }
    }
    return result;
}
