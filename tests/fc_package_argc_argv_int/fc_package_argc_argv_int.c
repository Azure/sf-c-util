// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdbool.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_package_com.h"
#include "sf_c_util/fc_package.h"

static bool argc_argv_are_equal(int argc_left, char** argv_left, int argc_right, char** argv_right)
{
    bool result;
    if (argc_left != argc_right)
    {
        result = false;
    }
    else
    {
        int i;
        for (i = 0; i < argc_left; i++)
        {
            if (strcmp(argv_left[i], argv_right[i]) != 0)
            {
                break;
            }
        }
        result = (i == argc_left);
    }
    return result;
}

TEST_DEFINE_ENUM_TYPE(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
}

TEST_FUNCTION_INITIALIZE(method_init)
{
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
}

/*all tests are actually going against IFabricConfigurationPackage rather than FC_PACKAGE_HANDLE. COM_WRAPPER ROCKS!!!*/
/*including getting the produced IFabricConfigurationPackage -> argc/argv*/

TEST_FUNCTION(FC_PACKAGE_create_with_0_sections_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clean
    fc_package_destroy(fc_package);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_to_IFabricConfigurationPackage_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///act
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert
    ASSERT_IS_NOT_NULL(obj);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_DecryptValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    IFabricStringResult* decrypted;

    HRESULT hr;

    ///act
    hr = obj->lpVtbl->DecryptValue(obj, L"", &decrypted);

    ///assert
    ASSERT_IS_TRUE(FAILED(hr));

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_GetSection_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    FABRIC_CONFIGURATION_SECTION* section;
    HRESULT hr;

    /*GetSection API*/
    ///act
    hr = obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section);

    ///assert
    ASSERT_IS_TRUE(FAILED(hr));

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_GetValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    BOOLEAN is_encrypted;
    wchar_t* value;
    HRESULT hr;

    /*GetValue*/
    ///act
    hr = obj->lpVtbl->GetValue(obj, L"DOES_NOT_EXIST", L"doesnothaveparametersanyway", &is_encrypted, &value);
    
    ///assert
    ASSERT_IS_TRUE(E_NOT_SET == hr);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_get_Description_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* fabric_configuration_package_description;

    ///act
    fabric_configuration_package_description = obj->lpVtbl->get_Description(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_package_description);
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", fabric_configuration_package_description->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->Version);
    ASSERT_IS_NULL(fabric_configuration_package_description->Reserved);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_get_Path_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const wchar_t* path;

    ///act
    path = obj->lpVtbl->get_Path(obj);

    ///assert
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, path);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_get_Settings_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings;

    ///act
    fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_settings->Sections->Count); /*no sections, no parameters, no nothing*/

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_to_argc_argv_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
}


TEST_FUNCTION(FC_PACKAGE_with_0_sections_half_params_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        "half_parameter"

    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clean
    fc_package_destroy(fc_package);
}

TEST_FUNCTION(FC_PACKAGE_with_0_sections_SERVICE_ENDPOINT_RESOURCE_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SERVICE_ENDPOINT_RESOURCE

    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clean
    fc_package_destroy(fc_package);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE, 
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);

    ///clean
    fc_package_destroy(fc_package);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_to_COM_object_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);

    ///act
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_DecryptValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    IFabricStringResult* decrypted;
    HRESULT hr;
    
    ///act
    hr = obj->lpVtbl->DecryptValue(obj, L"", &decrypted);

    ///assert
    ASSERT_IS_TRUE(FAILED(hr));
    
    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_GetSection_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    FABRIC_CONFIGURATION_SECTION* section;
    HRESULT hr;

    /*GetSection API*/
    ///act(1) - for a non-existing section
    hr = obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section);

    ///assert(1) - for a non-existing section
    ASSERT_IS_TRUE(FAILED(hr));

    ///act(2) - for an existing section
    hr = obj->lpVtbl->GetSection(obj, L"S1", &section);

    ///assert(2) - for an existing section
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetSection(obj, L"S1", &section)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
    ASSERT_ARE_EQUAL(int, 0, section->Parameters->Count);
    ASSERT_IS_NULL(section->Parameters->Items);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_GetValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    BOOLEAN is_encrypted;
    wchar_t* value;
    HRESULT hr;
    
    ///act
    hr = obj->lpVtbl->GetValue(obj, L"S1", L"doesnothaveparametersanyway", &is_encrypted, &value);

    ///assert
    ASSERT_IS_TRUE(E_NOT_SET == hr);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_get_Description_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* fabric_configuration_package_description;

    ///act
    fabric_configuration_package_description = obj->lpVtbl->get_Description(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_package_description);
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", fabric_configuration_package_description->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    ///clean
    obj->lpVtbl->Release(obj);
}


TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_get_Path_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const wchar_t* path;

    ///act
    path = obj->lpVtbl->get_Path(obj);

    ///assert
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, path);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters_get_Settings_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings;

    ///act
    fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_settings->Sections->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_settings->Sections->Items[0].Name);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_settings->Sections->Items[0].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_settings->Sections->Items[0].Parameters->Items);
    }
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_0_parameters__to_argc_argv_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    
    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ///clean
    fc_package_destroy(fc_package);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_to_COM_object_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ///act
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert
    ASSERT_IS_NOT_NULL(obj);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_DecryptValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    IFabricStringResult* decrypted;
    HRESULT hr;

    ///act
    hr = obj->lpVtbl->DecryptValue(obj, L"", &decrypted);
    
    ///assert
    ASSERT_IS_TRUE(FAILED(hr));

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_GetSection_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);


    const FABRIC_CONFIGURATION_SECTION* section;
    HRESULT hr;

    ///act(1) - a non-existing section
    hr = obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section);

    ///assert(1) - a non-existing section
    ASSERT_IS_TRUE(FAILED(hr));

    ///act(2) - an existing section
    hr = obj->lpVtbl->GetSection(obj, L"S1", &section);
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_IS_NOT_NULL(section);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
        ASSERT_ARE_EQUAL(int, 1, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", section->Parameters->Items[0].Value);
    }

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_GetValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    BOOLEAN is_encrypted;
    wchar_t* value;
    HRESULT hr;

    ///act(1) - for a non-existing value
    hr = obj->lpVtbl->GetValue(obj, L"S1", L"404 not found", &is_encrypted, &value);

    ///assert(1) - for a non-existing value
    ASSERT_IS_TRUE(E_NOT_SET == hr);

    ///act(2) - for an existing value
    hr = obj->lpVtbl->GetValue(obj, L"S1", L"s1p1", &is_encrypted, &value);

    ///assert(2) - for an existing value
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", value);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_get_Description_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* fabric_configuration_package_description;

    ///act
    fabric_configuration_package_description = obj->lpVtbl->get_Description(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_package_description);

    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", fabric_configuration_package_description->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->Version);
    ASSERT_IS_NULL(fabric_configuration_package_description->Reserved);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_get_Path_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const wchar_t* path;

    ///act
    path = obj->lpVtbl->get_Path(obj);

    ///assert
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, path);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_get_Settings_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings;

    ///act
    fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_settings->Sections->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_settings->Sections->Items[0].Name);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_settings->Sections->Items[0].Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Value);
    }

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_1_parameters_to_argc_argv_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    
    ///clean
    fc_package_destroy(fc_package);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_to_COM_object_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);

    ///act
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_DecryptValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    IFabricStringResult* decrypted;
    HRESULT hr;

    ///act
    hr = obj->lpVtbl->DecryptValue(obj, L"", &decrypted);

    ///assert
    ASSERT_IS_TRUE(FAILED(hr));

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_GetSection_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    
    const FABRIC_CONFIGURATION_SECTION* section;
    HRESULT hr;

    ///act(1) - for a non-existing section
    hr = obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section);

    ///assert(1) - for a non-existing section
    ASSERT_IS_TRUE(FAILED(hr));

    ///act(2) - for an existing section
    hr = obj->lpVtbl->GetSection(obj, L"S1", &section);

    ///assert(2) - for an existing section
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
        ASSERT_ARE_EQUAL(int, 2, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", section->Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", section->Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", section->Parameters->Items[1].Value);
    }

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_GetValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    BOOLEAN is_encrypted;
    wchar_t* value;
    HRESULT hr;

    ///act(1) - first parameter
    hr = obj->lpVtbl->GetValue(obj, L"S1", L"s1p1", &is_encrypted, &value);

    ///assert(1) - first parameter
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", value);

    ///act(2) - second parameter
    hr = obj->lpVtbl->GetValue(obj, L"S1", L"s1p2", &is_encrypted, &value);

    ///assert(2) - second parameter
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", value);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_get_Description_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* fabric_configuration_package_description;

    ///act
    fabric_configuration_package_description = obj->lpVtbl->get_Description(obj);
    
    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_package_description);
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", fabric_configuration_package_description->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->Version);
    ASSERT_IS_NULL(fabric_configuration_package_description->Reserved);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_get_Path_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const wchar_t* path;

    ///act
    path = obj->lpVtbl->get_Path(obj);

    ///assert
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, path);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_get_Settings_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings;

    ///act
    fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_settings->Sections->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_settings->Sections->Items[0].Name);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_settings->Sections->Items[0].Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_settings->Sections->Items[0].Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_settings->Sections->Items[0].Parameters->Items[1].Value);
    }

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_1_sections_2_parameters_to_argc_argc_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);

    ///clean
    fc_package_destroy(fc_package);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_to_CON_object_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);

    ///act
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_DecryptValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    IFabricStringResult* decrypted;
    HRESULT hr;

    ///act
    hr = obj->lpVtbl->DecryptValue(obj, L"", &decrypted);

    ///assert
    ASSERT_IS_TRUE(FAILED(hr));

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_GetSection_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_SECTION* section;
    HRESULT hr;
    
    ///act(1) - for a non-existing section
    hr = obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section);

    ///assert(1) - for a non-existing section
    ASSERT_IS_TRUE(FAILED(hr));

    ///act(2) - for the first section
    hr = obj->lpVtbl->GetSection(obj, L"S1", &section);

    ///assert(2) - for the first section
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
        ASSERT_ARE_EQUAL(int, 2, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", section->Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", section->Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", section->Parameters->Items[1].Value);
    }

    ///act(3) - for the second section
    hr = obj->lpVtbl->GetSection(obj, L"S2", &section);

    ///assert(3) - for the second section
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", section->Name);
        ASSERT_ARE_EQUAL(int, 2, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", section->Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", section->Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", section->Parameters->Items[1].Value);
    }

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_GetValue_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    BOOLEAN is_encrypted;
    const wchar_t* value;
    HRESULT hr;

    ///act(1) - first section - first value
    hr = obj->lpVtbl->GetValue(obj, L"S1", L"s1p1", &is_encrypted, &value);

    ///assert(1) - first section - first value
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", value);

    ///act(2) - first section - second value
    hr = obj->lpVtbl->GetValue(obj, L"S1", L"s1p2", &is_encrypted, &value);

    ///assert(2) - first section - second value
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", value);

    ///act(3) - second section - first value
    hr = obj->lpVtbl->GetValue(obj, L"S2", L"s2p1", &is_encrypted, &value);

    ///assert(3) - second section - first value
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", value);

    ///act(4) - second section - second value
    hr = obj->lpVtbl->GetValue(obj, L"S2", L"s2p2", &is_encrypted, &value);

    ///assert(4) - second section - second value
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", value);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_get_Description_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* fabric_configuration_package_description;

    ///act
    fabric_configuration_package_description = obj->lpVtbl->get_Description(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_package_description);

    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", fabric_configuration_package_description->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, fabric_configuration_package_description->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_get_Path_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    const wchar_t* path;

    ///act
    path = obj->lpVtbl->get_Path(obj);

    ///assert
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, path);

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_get_Settings_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    ///act
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);

    ///assert
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_settings->Sections->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_settings->Sections->Items[0].Name);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_settings->Sections->Items[0].Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_settings->Sections->Items[0].Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_settings->Sections->Items[0].Parameters->Items[1].Value);
    }
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_settings->Sections->Items[1].Name);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_settings->Sections->Items[1].Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_settings->Sections->Items[1].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_settings->Sections->Items[1].Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_settings->Sections->Items[1].Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_settings->Sections->Items[1].Parameters->Items[1].Value);
    }

    ///clean
    obj->lpVtbl->Release(obj);
}

TEST_FUNCTION(FC_PACKAGE_with_2_sections_each_with_2_parameters_to_argc_argv_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG",
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);
    ASSERT_IS_NOT_NULL(obj);

    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
