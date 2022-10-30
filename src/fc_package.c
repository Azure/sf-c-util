// Copyright (C) Microsoft Corporation. All rights reserved.

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fc_package.h"


struct FC_PACKAGE_TAG
{
    FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION fabric_configuration_package_description;
    FABRIC_CONFIGURATION_SETTINGS fabric_configuration_settings;
};

FC_PACKAGE_HANDLE fc_create(int a) /*AICI AM RAMAS trebuie sa il construiesc din argc, argv*/
{
    return malloc(a + sizeof(struct FC_PACKAGE_TAG));
}

void fc_package_destroy(FC_PACKAGE_HANDLE fc_package_handle)
{
    free(fc_package_handle);
}

const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* get_Description(FC_PACKAGE_HANDLE fc_package_handle)
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


LPCWSTR get_Path(FC_PACKAGE_HANDLE fc_package_handle)
{
    if (fc_package_handle == NULL)
    {
        LogError("invalid argument");
    }
    else
    {
        LogError("NOT IMPLEMENTED");
    }

    return NULL;
}

const FABRIC_CONFIGURATION_SETTINGS* get_Settings(FC_PACKAGE_HANDLE fc_package_handle)
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


HRESULT GetSection(FC_PACKAGE_HANDLE fc_package_handle,
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

HRESULT GetValue(FC_PACKAGE_HANDLE fc_package_handle,
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

HRESULT DecryptValue(FC_PACKAGE_HANDLE fc_package_handle,
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
