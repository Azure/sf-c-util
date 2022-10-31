// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdbool.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_package_com.h"
#include "sf_c_util/fc_package.h"

static bool argc_argv_are_equal(int argc_left, char** argv_left, int argc_right, char** argv_right)
{
    if (argc_left != argc_right)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < argc_left; i++)
        {
            if (strcmp(argv_left[i], argv_right[i]) != 0)
            {
                return false;
            }
        }
        return true;
    }
}

static TEST_MUTEX_HANDLE test_serialize_mutex;

TEST_DEFINE_ENUM_TYPE(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    TEST_MUTEX_DESTROY(test_serialize_mutex);
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    if (TEST_MUTEX_ACQUIRE(test_serialize_mutex))
    {
        ASSERT_FAIL("Could not acquire test serialization mutex.");
    }
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    TEST_MUTEX_RELEASE(test_serialize_mutex);
}

/*all tests are actually going against IFabricConfigurationPackage rather than FC_PACKAGE_HANDLE. COM_WRAPPER ROCKS!!!*/
/*including getting the produced IFabricConfigurationPackage -> argc/argv*/

TEST_FUNCTION(FC_PACKAGE_with_0_sections_succeeds)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "CONFIG"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    ///act(1)
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///act(2)
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    /*DecryptValue*/
    IFabricStringResult* decrypted;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->DecryptValue(obj, L"", &decrypted)));

    /*GetSection API*/
    FABRIC_CONFIGURATION_SECTION* section;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section)));

    /*GetValue*/
    BOOLEAN is_encrypted;
    wchar_t* value;
    ASSERT_IS_TRUE(E_NOT_SET == obj->lpVtbl->GetValue(obj, L"DOES_NOT_EXIST", L"doesnothaveparametersanyway", &is_encrypted, &value));

    /*get_Description*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    /*get_Path*/
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Path(obj));

    /*get_Settings API*/
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_settings->Sections->Count); /*no sections, no parameters, no nothing*/

    /*act(3)*/
    int p_argc;
    char** p_argv;
    ASSERT_ARE_EQUAL(int, 0, IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv));
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

    ///act(1)
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///act(2)
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    /*DecryptValue*/
    IFabricStringResult* decrypted;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->DecryptValue(obj, L"", &decrypted)));

    /*GetSection API*/
    FABRIC_CONFIGURATION_SECTION* section;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section)));

    /*GetValue*/
    BOOLEAN is_encrypted;
    wchar_t* value;
    ASSERT_IS_TRUE(E_NOT_SET == obj->lpVtbl->GetValue(obj, L"DOES_NOT_EXIST", L"doesnothaveparametersanyway", &is_encrypted, &value));

    /*get_Description*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    /*get_Path*/
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Path(obj));

    /*get_Settings*/
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_settings->Sections->Count); /*no sections, no parameters, no nothing*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);

    /*act(3)*/
    int p_argc;
    char** p_argv;
    ASSERT_ARE_EQUAL(int, 0, IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv));
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
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

    ///act(1)
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///act(2)
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    /*DecryptValue*/
    IFabricStringResult* decrypted;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->DecryptValue(obj, L"", &decrypted)));

    /*GetSection API*/
    FABRIC_CONFIGURATION_SECTION* section;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section)));

    /*GetValue*/
    BOOLEAN is_encrypted;
    wchar_t* value;
    ASSERT_IS_TRUE(E_NOT_SET == obj->lpVtbl->GetValue(obj, L"DOES_NOT_EXIST", L"doesnothaveparametersanyway", &is_encrypted, &value));

    /*get_Description*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    /*get_Path*/
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Path(obj));

    /*get_Settings API*/
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_settings->Sections->Count); /*no sections, no parameters, no nothing*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);

    /*act(3)*/
    int p_argc;
    char** p_argv;
    ASSERT_ARE_EQUAL(int, 0, IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv));
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
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

    ///act(1)
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);

    ///act(2)
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    /*DecryptValue*/
    IFabricStringResult* decrypted;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->DecryptValue(obj, L"", &decrypted)));

    /*GetSection API*/
    FABRIC_CONFIGURATION_SECTION* section;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section)));
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetSection(obj, L"S1", &section)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
    ASSERT_ARE_EQUAL(int, 0, section->Parameters->Count);
    ASSERT_IS_NULL(section->Parameters->Items);

    /*GetValue*/
    BOOLEAN is_encrypted;
    wchar_t* value;
    ASSERT_IS_TRUE(E_NOT_SET == obj->lpVtbl->GetValue(obj, L"S1", L"doesnothaveparametersanyway", &is_encrypted, &value));

    /*get_Description*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    /*get_Path*/
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Path(obj));

    /*get_Settings API*/
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_settings->Sections->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_settings->Sections->Items[0].Name);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_settings->Sections->Items[0].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_settings->Sections->Items[0].Parameters->Items);
    }
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    
    /*act(3)*/
    int p_argc;
    char** p_argv;
    ASSERT_ARE_EQUAL(int, 0, IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv));
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

    ///act(1)
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ///act(2)
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);
    
    /*DecryptValue*/
    IFabricStringResult* decrypted;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->DecryptValue(obj, L"", &decrypted)));

    /*GetSection API*/
    FABRIC_CONFIGURATION_SECTION* section;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section)));
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetSection(obj, L"S1", &section)));
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
        ASSERT_ARE_EQUAL(int, 1, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", section->Parameters->Items[0].Value);
    }

    /*GetValue*/
    BOOLEAN is_encrypted;
    wchar_t* value;
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetValue(obj, L"S1", L"s1p1", &is_encrypted, &value)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", value);

    /*get_Description*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    /*get_Path*/
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Path(obj));

    /*get_Settings API*/
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings);
    ASSERT_IS_NOT_NULL(fabric_configuration_settings->Sections);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_settings->Sections->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_settings->Sections->Items[0].Name);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_settings->Sections->Items[0].Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_settings->Sections->Items[0].Parameters->Items[0].Value);
    }
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    
    /*act(3)*/
    int p_argc;
    char** p_argv;
    ASSERT_ARE_EQUAL(int, 0, IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv));
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

    ///act(1)
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);

    ///act(2)
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    /*DecryptValue*/
    IFabricStringResult* decrypted;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->DecryptValue(obj, L"", &decrypted)));

    /*GetSection API*/
    FABRIC_CONFIGURATION_SECTION* section;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section)));
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetSection(obj, L"S1", &section)));
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
        ASSERT_ARE_EQUAL(int, 2, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", section->Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", section->Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", section->Parameters->Items[1].Value);
    }

    /*GetValue*/
    BOOLEAN is_encrypted;
    wchar_t* value;
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetValue(obj, L"S1", L"s1p1", &is_encrypted, &value)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", value);
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetValue(obj, L"S1", L"s1p2", &is_encrypted, &value)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", value);

    /*get_Description*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    /*get_Path*/
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Path(obj));

    /*get_Settings API*/
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);
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
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    
    /*act(3)*/
    int p_argc;
    char** p_argv;
    ASSERT_ARE_EQUAL(int, 0, IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv));
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

    ///act(1)
    FC_PACKAGE_HANDLE fc_package = fc_package_create(argc, argv, &argc_consumed);

    ///assert(1)
    ASSERT_IS_NOT_NULL(fc_package);
    ASSERT_ARE_EQUAL(int, 14, argc_consumed);

    ///act(2)
    IFabricConfigurationPackage* obj = COM_WRAPPER_CREATE(FC_PACKAGE_HANDLE, IFabricConfigurationPackage, fc_package, fc_package_destroy);

    ///assert(2)
    ASSERT_IS_NOT_NULL(obj);

    /*DecryptValue*/
    IFabricStringResult* decrypted;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->DecryptValue(obj, L"", &decrypted)));

    /*GetSection API*/
    FABRIC_CONFIGURATION_SECTION* section;
    ASSERT_IS_TRUE(FAILED(obj->lpVtbl->GetSection(obj, L"DOES_NOT_EXIST", &section)));
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetSection(obj, L"S1", &section)));
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", section->Name);
        ASSERT_ARE_EQUAL(int, 2, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", section->Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", section->Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", section->Parameters->Items[1].Value);
    }
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetSection(obj, L"S2", &section)));
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", section->Name);
        ASSERT_ARE_EQUAL(int, 2, section->Parameters->Count);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", section->Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", section->Parameters->Items[0].Value);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", section->Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", section->Parameters->Items[1].Value);
    }

    /*GetValue*/
    BOOLEAN is_encrypted;
    wchar_t* value;
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetValue(obj, L"S1", L"s1p1", &is_encrypted, &value)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", value);
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetValue(obj, L"S1", L"s1p2", &is_encrypted, &value)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", value);
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetValue(obj, L"S2", L"s2p1", &is_encrypted, &value)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", value);
    ASSERT_IS_TRUE(SUCCEEDED(obj->lpVtbl->GetValue(obj, L"S2", L"s2p2", &is_encrypted, &value)));
    ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", value);

    /*get_Description*/
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestName);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->ServiceManifestVersion);
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Description(obj)->Version);
    ASSERT_IS_NULL(obj->lpVtbl->get_Description(obj)->Reserved);

    /*get_Path*/
    ASSERT_ARE_EQUAL(wchar_ptr, FC_NOT_IMPLEMENTED_STRING, obj->lpVtbl->get_Path(obj));

    /*get_Settings API*/
    const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = obj->lpVtbl->get_Settings(obj);
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
    ASSERT_ARE_EQUAL(wchar_ptr, L"CONFIG", obj->lpVtbl->get_Description(obj)->Name);

    /*act(3)*/
    int p_argc;
    char** p_argv;
    ASSERT_ARE_EQUAL(int, 0, IFabricConfigurationPackage_to_ARGC_ARGV(obj, &p_argc, &p_argv));
    ASSERT_IS_TRUE(argc_argv_are_equal(argc_consumed, argv, p_argc, p_argv));

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    obj->lpVtbl->Release(obj);
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
