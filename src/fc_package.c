// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "windows.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_section_list_argc_argv.h"

#include "sf_c_util/fc_package.h"

struct FC_PACKAGE_TAG
{
    FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION fabric_configuration_package_description;
    FABRIC_CONFIGURATION_SETTINGS fabric_configuration_settings;
};

FC_PACKAGE_HANDLE fc_package_create(int argc, char** argv, int* argc_consumed)
{
    FC_PACKAGE_HANDLE result;
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
        /*consumed greedy until cannot parse anymore*/
        if (strcmp(argv[0], CONFIGURATION_PACKAGE_NAME) != 0)
        {
            LogVerbose("cannot parse because first argument is %s and it was expected to be " CONFIGURATION_PACKAGE_NAME "", argv[0]);
            *argc_consumed = 0;
            result = NULL;
        }
        else
        {
            result = malloc(sizeof(struct FC_PACKAGE_TAG));
            if (result == NULL)
            {
                LogError("Failure in malloc");
                /*return as is*/
            }
            else
            {
                result->fabric_configuration_package_description.Name = mbs_to_wcs(argv[1]);
                if (result->fabric_configuration_package_description.Name == NULL)
                {
                    LogError("failure in mbs_to_wcs(argv[1]=%s);", argv[1]);
                }
                else
                {
                    *argc_consumed = 2;
                    result->fabric_configuration_package_description.Reserved = NULL;
                    result->fabric_configuration_package_description.ServiceManifestName = FC_NOT_IMPLEMENTED_STRING;
                    result->fabric_configuration_package_description.ServiceManifestVersion = FC_NOT_IMPLEMENTED_STRING;
                    result->fabric_configuration_package_description.Version = FC_NOT_IMPLEMENTED_STRING;

                    result->fabric_configuration_settings.Reserved = NULL;
                    
                    result->fabric_configuration_settings.Sections = malloc(sizeof(FABRIC_CONFIGURATION_SECTION_LIST));
                    if (result->fabric_configuration_settings.Sections == NULL)
                    {
                        LogError("failure in malloc");
                    }
                    else
                    {
                        int c_argc;
                        ARGC_ARGV_DATA_RESULT r;
                        r = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc - *argc_consumed, argv + *argc_consumed, (FABRIC_CONFIGURATION_SECTION_LIST*)result->fabric_configuration_settings.Sections, &c_argc);

                        switch (r)
                        {
                            case ARGC_ARGV_DATA_OK:
                            {
                                *argc_consumed += c_argc;
                                goto allok;
                                break;
                            }
                            case ARGC_ARGV_DATA_INVALID:
                            case ARGC_ARGV_DATA_ERROR:
                            default:
                            {
                                LogError("error, FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV returned %" PRI_MU_ENUM "", MU_ENUM_VALUE(ARGC_ARGV_DATA_RESULT, r));
                                break;
                            }
                        }
                        free((void*)result->fabric_configuration_settings.Sections);
                    }
                    free((void*)result->fabric_configuration_package_description.Name);
                }
                free(result); 
                result = NULL;
            }
        }
    }
allok:;
    return result;
}

void fc_package_destroy(FC_PACKAGE_HANDLE fc_package_handle)
{
    FABRIC_CONFIGURATION_SECTION_LIST_free((void*)fc_package_handle->fabric_configuration_settings.Sections);
    free((void*)fc_package_handle->fabric_configuration_settings.Sections);
    free((void*)fc_package_handle->fabric_configuration_package_description.Name);
    free(fc_package_handle);
}

const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* IFabricConfigurationPackage_get_Description(FC_PACKAGE_HANDLE fc_package_handle)
{
    const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* result;
    if (fc_package_handle == NULL)
    {
        LogError("invalid arg");
        result = NULL;
    }
    else
    {
        result = &fc_package_handle->fabric_configuration_package_description;
    }
    return result;
}


LPCWSTR IFabricConfigurationPackage_get_Path(FC_PACKAGE_HANDLE fc_package_handle)
{
    if (fc_package_handle == NULL)
    {
        LogError("invalid argument");
    }
    else
    {
        LogError("NOT IMPLEMENTED");
    }

    return FC_NOT_IMPLEMENTED_STRING;
}

const FABRIC_CONFIGURATION_SETTINGS* IFabricConfigurationPackage_get_Settings(FC_PACKAGE_HANDLE fc_package_handle)
{
    if (fc_package_handle == NULL)
    {
        LogError("invalid argument");
        return NULL;
    }
    else
    {
        return &fc_package_handle->fabric_configuration_settings;
    }
}


HRESULT IFabricConfigurationPackage_GetSection(FC_PACKAGE_HANDLE fc_package_handle,
    LPCWSTR sectionName,
    const FABRIC_CONFIGURATION_SECTION** bufferedValue
    )
{
    HRESULT hr;
    if (
        (fc_package_handle == NULL) ||
        (sectionName == NULL) ||
        (bufferedValue == NULL)
        )
    {
        LogError("invalid argument");
        hr = E_INVALIDARG;
    }
    else
    {
        ULONG i;
        for (i = 0; i < fc_package_handle->fabric_configuration_settings.Sections->Count; i++)
        {
            if (wcscmp(sectionName, fc_package_handle->fabric_configuration_settings.Sections->Items[i].Name) == 0)
            {
                *bufferedValue = fc_package_handle->fabric_configuration_settings.Sections->Items + i;
                return S_OK;
            }
        }
        
        hr = E_NOT_SET; /*this is NOT_FOUND in HRESULT */

    }
    return hr;
}

HRESULT IFabricConfigurationPackage_GetValue(FC_PACKAGE_HANDLE fc_package_handle,
    /* [in] */ LPCWSTR sectionName,
    /* [in] */ LPCWSTR parameterName,
    /* [out] */ BOOLEAN* isEncrypted,
    /* [retval][out] */ LPCWSTR* bufferedValue)
{
    HRESULT hr;
    if (
        (fc_package_handle == NULL) ||
        (parameterName == NULL) ||
        (isEncrypted == NULL) ||
        (bufferedValue == NULL)
        )
    {
        LogError("invalid arg");
        hr = E_INVALIDARG;
    }
    else
    {
        ULONG i;
        for (i = 0; i < fc_package_handle->fabric_configuration_settings.Sections->Count; i++)
        {
            if (wcscmp(sectionName, fc_package_handle->fabric_configuration_settings.Sections->Items[i].Name) == 0)
            {
                const FABRIC_CONFIGURATION_SECTION* section = fc_package_handle->fabric_configuration_settings.Sections->Items + i;
                ULONG j;
                for (j = 0; j < section->Parameters->Count; j++)
                {
                    if (wcscmp(parameterName, section->Parameters->Items[j].Name) == 0)
                    {
                        *bufferedValue = section->Parameters->Items[j].Value;
                        return S_OK;
                    }
                }
            }
        }

        hr = E_NOT_SET;
    }
    return hr;

}

HRESULT IFabricConfigurationPackage_DecryptValue(FC_PACKAGE_HANDLE fc_package_handle,
    /* [in] */ LPCWSTR encryptedValue,
    /* [retval][out] */ IFabricStringResult** decryptedValue)
{
    HRESULT hr;
    if (
        (fc_package_handle == NULL) ||
        (encryptedValue == NULL) ||
        (decryptedValue == NULL)
        )
    {
        LogError("invalid argument");
        hr = E_INVALIDARG;
    }
    else
    {
        hr = E_NOTIMPL;
    }
    return hr;
}

int IFabricConfigurationPackage_to_ARGC_ARGV(IFabricConfigurationPackage* iFabricConfigurationPackage, int* argc, char*** argv)
{
    int result;
    if (
        (iFabricConfigurationPackage == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument IFabricConfigurationPackage* iFabricConfigurationPackage=%p, int* argc=%p, char*** argv=%p",
            iFabricConfigurationPackage, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        *argv = malloc_2(2, sizeof(char*));
        if (*argv == NULL)
        {
            LogError("failure in malloc_2");
            result = MU_FAILURE;
        }
        else
        {
            if (((*argv)[0] = sprintf_char(CONFIGURATION_PACKAGE_NAME)) == NULL)
            {
                LogError("failure in sprintf_char");
                result = MU_FAILURE;
            }
            else
            {
                const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* fabric_configuration_package_description = iFabricConfigurationPackage->lpVtbl->get_Description(iFabricConfigurationPackage);
                if (fabric_configuration_package_description == NULL)
                {
                    LogError("failure in get_Description");
                    result = MU_FAILURE;
                }
                else
                {
                    if (fabric_configuration_package_description->Name == NULL)
                    {
                        LogError("unexpected fabric_configuration_package_description->Name == NULL");
                        result = MU_FAILURE;
                    }
                    else
                    {
                        if (((*argv)[1] = sprintf_char("%ls", fabric_configuration_package_description->Name)) == NULL)
                        {
                            LogError("failure in sprintf_char");
                            result = MU_FAILURE;
                        }
                        else
                        {
                            *argc = 2;
                            const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = iFabricConfigurationPackage->lpVtbl->get_Settings(iFabricConfigurationPackage);
                            if (fabric_configuration_settings == NULL)
                            {
                                LogError("unexpected get_Settings returning NULL");
                                result = MU_FAILURE;
                            }
                            else
                            {
                                const FABRIC_CONFIGURATION_SECTION_LIST* fabric_configuration_section_list = fabric_configuration_settings->Sections;

                                int p_argc;
                                char** p_argv;
                                if (FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(fabric_configuration_section_list, &p_argc, &p_argv) != 0)
                                {
                                    LogError("failure in FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV");
                                    result = MU_FAILURE;
                                }
                                else
                                {
                                    if (ARGC_ARGV_concat(argc, argv, p_argc, p_argv) != 0)
                                    {
                                        ARGC_ARGV_free(p_argc, p_argv);
                                        LogError("failure in ARGC_ARGV_concat");
                                        result = MU_FAILURE;
                                    }
                                    else
                                    {
                                        result = 0;
                                        ARGC_ARGV_free(p_argc, p_argv);
                                        goto allok;
                                    }
                                }

                            }
                            free((*argv)[1]);
                        }
                    }
                }
                free((*argv)[0]);
            }
            free(*argv);
        }
    }
allok:;
    return result;
}
