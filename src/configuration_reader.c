// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

#include "windows.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "sf_c_util/hresult_to_string.h"

#include "sf_c_util/configuration_reader.h"

#define TRUEString L"True"
#define FALSEString L"False"

static int get_string_value_from_package(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, IFabricConfigurationPackage** fabric_configuration_package, const wchar_t** wchar_value)
{
    int result;
    HRESULT hr;

    /*Codes_SRS_CONFIGURATION_READER_01_006: [ configuration_reader_get_uint8_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
    /*Codes_SRS_CONFIGURATION_READER_42_017: [ configuration_reader_get_uint32_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
    /*Codes_SRS_CONFIGURATION_READER_42_006: [ configuration_reader_get_uint64_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
    /*Codes_SRS_CONFIGURATION_READER_42_028: [ configuration_reader_get_char_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
    /*Codes_SRS_CONFIGURATION_READER_42_038: [ configuration_reader_get_wchar_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
    /*Codes_SRS_CONFIGURATION_READER_42_048: [ configuration_reader_get_thandle_rc_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
    hr = activation_context->lpVtbl->GetConfigurationPackage(activation_context, config_package_name, fabric_configuration_package);
    if (FAILED(hr))
    {
        /*Codes_SRS_CONFIGURATION_READER_01_010: [ If there are any other failures then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_021: [ If there are any other failures then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_010: [ If there are any other failures then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_031: [ If there are any other failures then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_041: [ If there are any other failures then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_052: [ If there are any other failures then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
        LogHRESULTError(hr, "GetConfigurationPackage failed (config_package_name:%ls)", config_package_name);
        result = MU_FAILURE;
    }
    else
    {
        BOOLEAN is_encrypted = FALSE;
        /*Codes_SRS_CONFIGURATION_READER_01_007: [ configuration_reader_get_uint8_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_018: [ configuration_reader_get_uint32_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_007: [ configuration_reader_get_uint64_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_029: [ configuration_reader_get_char_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_039: [ configuration_reader_get_wchar_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
        /*Codes_SRS_CONFIGURATION_READER_42_049: [ configuration_reader_get_thandle_rc_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
        hr = (*fabric_configuration_package)->lpVtbl->GetValue((*fabric_configuration_package), section_name, parameter_name, &is_encrypted, wchar_value);
        if (FAILED(hr))
        {
            /*Codes_SRS_CONFIGURATION_READER_01_010: [ If there are any other failures then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
            /*Codes_SRS_CONFIGURATION_READER_42_021: [ If there are any other failures then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
            /*Codes_SRS_CONFIGURATION_READER_42_010: [ If there are any other failures then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
            /*Codes_SRS_CONFIGURATION_READER_42_031: [ If there are any other failures then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
            /*Codes_SRS_CONFIGURATION_READER_42_041: [ If there are any other failures then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
            /*Codes_SRS_CONFIGURATION_READER_42_052: [ If there are any other failures then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
            LogHRESULTError(hr, "GetValue failed (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                config_package_name, section_name, parameter_name);
            result = MU_FAILURE;
        }
        else
        {
            result = 0;
            goto all_ok;
        }
        (void)(*fabric_configuration_package)->lpVtbl->Release(*fabric_configuration_package);
        *fabric_configuration_package = NULL;
    }
all_ok:
    return result;
}

int configuration_reader_get_uint8_t(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, uint8_t* value)
{
    int result;

    if (
        /*Codes_SRS_CONFIGURATION_READER_01_001: [ If activation_context is NULL then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
        activation_context == NULL ||
        /*Codes_SRS_CONFIGURATION_READER_01_002: [ If config_package_name is NULL or empty then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
        (config_package_name == NULL || config_package_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_01_003: [ If section_name is NULL or empty then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
        (section_name == NULL || section_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_01_004: [ If parameter_name is NULL or empty then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
        (parameter_name == NULL || parameter_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_01_005: [ If value is NULL then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
        (value == NULL)
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls, uint8_t* value = %p",
            activation_context, MU_WP_OR_NULL(config_package_name), MU_WP_OR_NULL(section_name), MU_WP_OR_NULL(parameter_name), value);
        result = MU_FAILURE;
    }
    else
    {
        IFabricConfigurationPackage* fabric_configuration_package;
        const wchar_t* wchar_value;
        if (get_string_value_from_package(activation_context, config_package_name, section_name, parameter_name, &fabric_configuration_package, &wchar_value) != 0)
        {
            // already logged error
            result = MU_FAILURE;
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_READER_01_008: [ configuration_reader_get_uint8_t shall convert the value to uint8_t and store it in value. ]*/
            wchar_t* end_ptr;
            uint64_t temp = wcstoull(wchar_value, &end_ptr, 10);
            if (end_ptr == wchar_value)
            {
                /*Codes_SRS_CONFIGURATION_READER_42_021: [ If there are any other failures then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
                LogError("failure in wcstoull(%ls): subject sequence is empty or does not have the expected form (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                    wchar_value, config_package_name, section_name, parameter_name);
                result = MU_FAILURE;
            }
            else
            {
                if ((temp == ULLONG_MAX) && (errno == ERANGE))
                {
                    /*Codes_SRS_CONFIGURATION_READER_01_009: [ If the value is outside the range of representable values then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
                    LogError("ULLONGMAX was returned for wcstoull(%ls), indicating the correct value is outside the range of representable values (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                        wchar_value, config_package_name, section_name, parameter_name);
                    result = MU_FAILURE;
                }
                else if (temp > UINT8_MAX)
                {
                    /*Codes_SRS_CONFIGURATION_READER_01_009: [ If the value is outside the range of representable values then configuration_reader_get_uint8_t shall fail and return a non-zero value. ]*/
                    LogError("The value %" PRIu64 " is too large for uint8_t (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                        temp, config_package_name, section_name, parameter_name);
                    result = MU_FAILURE;
                }
                else
                {
                    /*Codes_SRS_CONFIGURATION_READER_01_011: [ configuration_reader_get_uint8_t shall succeed and return 0. ]*/
                    *value = (uint8_t)temp;
                    result = 0;
                }
            }
            (void)fabric_configuration_package->lpVtbl->Release(fabric_configuration_package);
        }
    }

    return result;
}

int configuration_reader_get_uint32_t(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, uint32_t* value)
{
    int result;

    if (
        /*Codes_SRS_CONFIGURATION_READER_42_012: [ If activation_context is NULL then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
        activation_context == NULL ||
        /*Codes_SRS_CONFIGURATION_READER_42_013: [ If config_package_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
        (config_package_name == NULL || config_package_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_014: [ If section_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
        (section_name == NULL || section_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_015: [ If parameter_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
        (parameter_name == NULL || parameter_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_016: [ If value is NULL then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
        (value == NULL)
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls, uint32_t* value = %p",
            activation_context, MU_WP_OR_NULL(config_package_name), MU_WP_OR_NULL(section_name), MU_WP_OR_NULL(parameter_name), value);
        result = MU_FAILURE;
    }
    else
    {
        IFabricConfigurationPackage* fabric_configuration_package;
        const wchar_t* wchar_value;
        if (get_string_value_from_package(activation_context, config_package_name, section_name, parameter_name, &fabric_configuration_package, &wchar_value) != 0)
        {
            // already logged error
            result = MU_FAILURE;
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_READER_42_019: [ configuration_reader_get_uint32_t shall convert the value to uint32_t and store it in value. ]*/
            wchar_t* end_ptr;
            uint64_t temp = wcstoull(wchar_value, &end_ptr, 10);
            if (end_ptr == wchar_value)
            {
                /*Codes_SRS_CONFIGURATION_READER_42_021: [ If there are any other failures then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
                LogError("failure in wcstoull(%ls): subject sequence is empty or does not have the expected form (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                    wchar_value, config_package_name, section_name, parameter_name);
                result = MU_FAILURE;
            }
            else
            {
                if ((temp == ULLONG_MAX) && (errno == ERANGE))
                {
                    /*Codes_SRS_CONFIGURATION_READER_42_020: [ If the value is outside the range of representable values then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
                    LogError("ULLONGMAX was returned for wcstoull(%ls), indicating the correct value is outside the range of representable values (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                        wchar_value, config_package_name, section_name, parameter_name);
                    result = MU_FAILURE;
                }
                else if (temp > UINT32_MAX)
                {
                    /*Codes_SRS_CONFIGURATION_READER_42_020: [ If the value is outside the range of representable values then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
                    LogError("The value %" PRIu64 " is too large for uint32_t (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                        temp, config_package_name, section_name, parameter_name);
                    result = MU_FAILURE;
                }
                else
                {
                    /*Codes_SRS_CONFIGURATION_READER_42_022: [ configuration_reader_get_uint32_t shall succeed and return 0. ]*/
                    *value = (uint32_t)temp;
                    result = 0;
                }
            }
            (void)fabric_configuration_package->lpVtbl->Release(fabric_configuration_package);
        }
    }

    return result;
}

int configuration_reader_get_uint64_t(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, uint64_t* value)
{
    int result;

    if (
        /*Codes_SRS_CONFIGURATION_READER_42_001: [ If activation_context is NULL then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
        activation_context == NULL ||
        /*Codes_SRS_CONFIGURATION_READER_42_002: [ If config_package_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
        (config_package_name == NULL || config_package_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_003: [ If section_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
        (section_name == NULL || section_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_004: [ If parameter_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
        (parameter_name == NULL || parameter_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_005: [ If value is NULL then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
        (value == NULL)
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls, uint64_t* value = %p",
            activation_context, MU_WP_OR_NULL(config_package_name), MU_WP_OR_NULL(section_name), MU_WP_OR_NULL(parameter_name), value);
        result = MU_FAILURE;
    }
    else
    {
        IFabricConfigurationPackage* fabric_configuration_package;
        const wchar_t* wchar_value;
        if (get_string_value_from_package(activation_context, config_package_name, section_name, parameter_name, &fabric_configuration_package, &wchar_value) != 0)
        {
            // already logged error
            result = MU_FAILURE;
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_READER_42_008: [ configuration_reader_get_uint64_t shall convert the value to uint64_t and store it in value. ]*/
            wchar_t* end_ptr;
            uint64_t temp = wcstoull(wchar_value, &end_ptr, 10);
            if (end_ptr == wchar_value)
            {
                /*Codes_SRS_CONFIGURATION_READER_42_010: [ If there are any other failures then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
                LogError("failure in wcstoull(%ls): subject sequence is empty or does not have the expected form (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                    wchar_value, config_package_name, section_name, parameter_name);
                result = MU_FAILURE;
            }
            else
            {
                if ((temp == ULLONG_MAX) && (errno == ERANGE))
                {
                    /*Codes_SRS_CONFIGURATION_READER_42_009: [ If the value is outside the range of representable values then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
                    LogError("ULLONGMAX was returned for wcstoull(%ls), indicating the correct value is outside the range of representable values (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                        wchar_value, config_package_name, section_name, parameter_name);
                    result = MU_FAILURE;
                }
                else
                {
                    /*Codes_SRS_CONFIGURATION_READER_42_011: [ configuration_reader_get_uint64_t shall succeed and return 0. ]*/
                    *value = temp;
                    result = 0;
                }
            }
            (void)fabric_configuration_package->lpVtbl->Release(fabric_configuration_package);
        }
    }

    return result;
}

int configuration_reader_get_bool(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, bool* value)
{
    int result;

    if (
        /*Codes_SRS_CONFIGURATION_READER_03_001: [ If activation_context is NULL then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
        activation_context == NULL ||
        /*Codes_SRS_CONFIGURATION_READER_03_002: [ If config_package_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
        (config_package_name == NULL || config_package_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_03_003: [ If section_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
        (section_name == NULL || section_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_03_004: [ If parameter_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
        (parameter_name == NULL || parameter_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_03_005: [ If value is NULL then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
        (value == NULL)
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls, uint32_t* value = %p",
            activation_context, MU_WP_OR_NULL(config_package_name), MU_WP_OR_NULL(section_name), MU_WP_OR_NULL(parameter_name), value);
        result = MU_FAILURE;
    }
    else
    {
        IFabricConfigurationPackage* fabric_configuration_package;
        const wchar_t* wchar_value;
        /*Codes_SRS_CONFIGURATION_READER_03_006: [ configuration_reader_get_bool shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
        /*Codes_SRS_CONFIGURATION_READER_03_007: [ configuration_reader_get_bool shall call GetValue on the configuration package with section_name and parameter_name. ]*/
        if (get_string_value_from_package(activation_context, config_package_name, section_name, parameter_name, &fabric_configuration_package, &wchar_value) != 0)
        {
            // already logged error
            /*Codes_SRS_CONFIGURATION_READER_03_011: [ If there are any other failures then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
            result = MU_FAILURE;
        }
        else
        {
            // Codes_SRS_CONFIGURATION_READER_11_001: [ configuration_reader_get_bool shall do a case insensitive comparison of the string. ]
            /*Codes_SRS_CONFIGURATION_READER_03_009: [ If the string is False, configuration_reader_get_bool shall set value to false and return 0. ]*/
            /*Codes_SRS_CONFIGURATION_READER_03_012: [ configuration_reader_get_bool shall succeed and return 0. ]*/
            if (_wcsicmp(wchar_value, FALSEString) == 0)
            {
                *value = false;
                result = 0;
            }
            /*Codes_SRS_CONFIGURATION_READER_03_010: [ If the string is True, configuration_reader_get_bool shall set value to true and return 0. ]*/
            /*Codes_SRS_CONFIGURATION_READER_03_012: [ configuration_reader_get_bool shall succeed and return 0. ]*/
            else if (_wcsicmp(wchar_value, TRUEString) == 0)
            {
                *value = true;
                result = 0;
            }
            /*Codes_SRS_CONFIGURATION_READER_03_014: [ If the string is an empty string, configuration_reader_get_bool shall set value to false and return 0. ]*/
            else if (wcscmp(wchar_value, L"") == 0)
            {
                *value = false;
                result = 0;
            }
            else
            {
                /*Codes_SRS_CONFIGURATION_READER_03_013: [ If the string is anything other than the above, configuration_reader_get_bool shall fail and return a non-zero value. ]*/
                LogError("Invalid boolean value %ls for const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls",
                    wchar_value, config_package_name, section_name, parameter_name);
                result = MU_FAILURE;
            }
            (void)fabric_configuration_package->lpVtbl->Release(fabric_configuration_package);
        }
    }

    return result;
}

int configuration_reader_get_char_string(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, char** value)
{
    int result;

    if (
        /*Codes_SRS_CONFIGURATION_READER_42_023: [ If activation_context is NULL then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
        activation_context == NULL ||
        /*Codes_SRS_CONFIGURATION_READER_42_024: [ If config_package_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
        (config_package_name == NULL || config_package_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_025: [ If section_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
        (section_name == NULL || section_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_026: [ If parameter_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
        (parameter_name == NULL || parameter_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_027: [ If value is NULL then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
        (value == NULL)
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls, char** value = %p",
            activation_context, MU_WP_OR_NULL(config_package_name), MU_WP_OR_NULL(section_name), MU_WP_OR_NULL(parameter_name), value);
        result = MU_FAILURE;
    }
    else
    {
        IFabricConfigurationPackage* fabric_configuration_package;
        const wchar_t* wchar_value;
        if (get_string_value_from_package(activation_context, config_package_name, section_name, parameter_name, &fabric_configuration_package, &wchar_value) != 0)
        {
            // already logged error
            result = MU_FAILURE;
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_READER_42_030: [ configuration_reader_get_char_string shall convert the value from a wide-character string to narrow-character string and store it in value. ]*/
            char* temp = sprintf_char("%ls", wchar_value);

            if (temp == NULL)
            {
                /*Codes_SRS_CONFIGURATION_READER_42_031: [ If there are any other failures then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
                LogError("Failed to copy string %ls (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                    wchar_value, config_package_name, section_name, parameter_name);
                result = MU_FAILURE;
            }
            else
            {
                /*Codes_SRS_CONFIGURATION_READER_42_032: [ configuration_reader_get_char_string shall succeed and return 0. ]*/
                *value = temp;
                result = 0;
            }
            (void)fabric_configuration_package->lpVtbl->Release(fabric_configuration_package);
        }
    }

    return result;
}

int configuration_reader_get_thandle_rc_string(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, THANDLE(RC_STRING)* value)
{
    int result;

    if (
        /*Codes_SRS_CONFIGURATION_READER_42_043: [ If activation_context is NULL then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
        activation_context == NULL ||
        /*Codes_SRS_CONFIGURATION_READER_42_044: [ If config_package_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
        (config_package_name == NULL || config_package_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_045: [ If section_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
        (section_name == NULL || section_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_046: [ If parameter_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
        (parameter_name == NULL || parameter_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_047: [ If value is NULL then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
        (value == NULL)
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls, THANDLE(RC_STRING)* value = %p",
            activation_context, MU_WP_OR_NULL(config_package_name), MU_WP_OR_NULL(section_name), MU_WP_OR_NULL(parameter_name), value);
        result = MU_FAILURE;
    }
    else
    {
        IFabricConfigurationPackage* fabric_configuration_package;
        const wchar_t* wchar_value;
        if (get_string_value_from_package(activation_context, config_package_name, section_name, parameter_name, &fabric_configuration_package, &wchar_value) != 0)
        {
            // already logged error
            result = MU_FAILURE;
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_READER_42_050: [ configuration_reader_get_thandle_rc_string shall convert the value from a wide-character string to narrow-character string. ]*/
            char* temp = sprintf_char("%ls", wchar_value);

            if (temp == NULL)
            {
                /*Codes_SRS_CONFIGURATION_READER_42_052: [ If there are any other failures then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
                LogError("Failed to copy string %ls (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                    wchar_value, config_package_name, section_name, parameter_name);
                result = MU_FAILURE;
            }
            else
            {
                /*Codes_SRS_CONFIGURATION_READER_42_051: [ configuration_reader_get_thandle_rc_string shall store the converted string in a THANDLE(RC_STRING). ]*/
                THANDLE(RC_STRING) temp_rc = rc_string_create_with_move_memory(temp);

                if (temp_rc == NULL)
                {
                    /*Codes_SRS_CONFIGURATION_READER_42_052: [ If there are any other failures then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
                    LogError("Failed to wrap string %s in RC_STRING (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                        temp, config_package_name, section_name, parameter_name);
                    result = MU_FAILURE;
                    free(temp);
                }
                else
                {
                    THANDLE_INITIALIZE_MOVE(RC_STRING)(value, &temp_rc);

                    /*Codes_SRS_CONFIGURATION_READER_42_053: [ configuration_reader_get_thandle_rc_string shall succeed and return 0. ]*/
                    result = 0;
                }
            }
            (void)fabric_configuration_package->lpVtbl->Release(fabric_configuration_package);
        }
    }

    return result;
}

int configuration_reader_get_wchar_string(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, wchar_t** value)
{
    int result;

    if (
        /*Codes_SRS_CONFIGURATION_READER_42_033: [ If activation_context is NULL then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
        activation_context == NULL ||
        /*Codes_SRS_CONFIGURATION_READER_42_034: [ If config_package_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
        (config_package_name == NULL || config_package_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_035: [ If section_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
        (section_name == NULL || section_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_036: [ If parameter_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
        (parameter_name == NULL || parameter_name[0] == L'\0') ||
        /*Codes_SRS_CONFIGURATION_READER_42_037: [ If value is NULL then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
        (value == NULL)
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const wchar_t* config_package_name = %ls, const wchar_t* section_name = %ls, const wchar_t* parameter_name = %ls, wchar_t** value = %p",
            activation_context, MU_WP_OR_NULL(config_package_name), MU_WP_OR_NULL(section_name), MU_WP_OR_NULL(parameter_name), value);
        result = MU_FAILURE;
    }
    else
    {
        IFabricConfigurationPackage* fabric_configuration_package;
        const wchar_t* wchar_value;
        if (get_string_value_from_package(activation_context, config_package_name, section_name, parameter_name, &fabric_configuration_package, &wchar_value) != 0)
        {
            // already logged error
            result = MU_FAILURE;
        }
        else
        {
            /*Codes_SRS_CONFIGURATION_READER_42_040: [ configuration_reader_get_wchar_string shall copy the string and store it in value. ]*/
            *value = sprintf_wchar(L"%s", wchar_value);

            if (*value == NULL)
            {
                /*Codes_SRS_CONFIGURATION_READER_42_041: [ If there are any other failures then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
                LogError("Failed to copy string %ls (config_package_name:%ls, section_name:%ls, parameter_name:%ls)",
                    wchar_value, config_package_name, section_name, parameter_name);
                result = MU_FAILURE;
            }
            else
            {
                /*Codes_SRS_CONFIGURATION_READER_42_042: [ configuration_reader_get_wchar_string shall succeed and return 0. ]*/
                result = 0;
            }
            (void)fabric_configuration_package->lpVtbl->Release(fabric_configuration_package);
        }
    }

    return result;
}
