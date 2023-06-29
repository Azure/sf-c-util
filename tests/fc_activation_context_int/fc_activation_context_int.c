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

#include "sf_c_util/fc_activation_context_com.h"
#include "sf_c_util/fc_activation_context.h"

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

/*all tests are actually going against IFabricCodePackageActivationContext rather than FC_ACTIVATION_CONTEXT_HANDLE. COM_WRAPPER ROCKS!!!*/
/*including getting the produced IFabricCodePackageActivationContext -> argc/argv*/

TEST_FUNCTION(fc_activation_context_create_succeeds_1) /*in this case, it contains nothing, because argvs don't start with CONFIGURATION_PACKAGE_NAME or with SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME "TOTALLY NOT"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 0, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_2) /*in this case, it contains nothing, because argv do not have the name of the CONFIGURATION_PACKAGE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME
        /*note: no name following CONFIGURATION_PACKAGE_NAME*/
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 0, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_3) /*in this case, it contains only the name of the configuration package ("A") and 0 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_4) /*in this case, it contains only the name of the configuration package ("A") and 0 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A", 
        "SOME UNEXPECTED STRING WHICH SHOULD BE IGNORED"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_5) /*in this case, it contains 2 configuration packages, "A" and "B" and 0 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_6) /*in this case, it contains 0 configuration packages and 1 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "name1", 
        "protocol1",
        "type1",
        "1",
        "certificate1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_7) /*in this case, it contains 0 configuration packages and 2 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_8) /*in this case, it contains 1 configuration packages and 1 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_9) /*in this case, it contains 1 configuration packages and 2 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2+6+6, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_10) /*in this case, it contains 2 configuration packages and 1 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B",
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2 + 2 + 6, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_succeeds_11) /*in this case, it contains 2 configuration packages and 2 SERVICE_ENDPOINT_RESOURCE*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B",
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;

    ///act
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);

    ///assert
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2 + 2 + 6+6, argc_consumed);

    ///clean
    fc_activation_context_destroy(activation_context);
}

TEST_FUNCTION(fc_activation_context_create_can_be_wrapped_in_COM_object)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    IFabricCodePackageActivationContext* fc_activation_context;

    ///act
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);

    ///assert
    ASSERT_IS_NOT_NULL(fc_activation_context);

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_GetConfigurationPackage_with_1_configuration_package)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    IFabricConfigurationPackage* configPackage;
    HRESULT hr;

    ///act
    hr = fc_activation_context->lpVtbl->GetConfigurationPackage(fc_activation_context, L"A", &configPackage);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_IS_NOT_NULL(configPackage);
    ASSERT_ARE_EQUAL(wchar_ptr, L"A", configPackage->lpVtbl->get_Description(configPackage)->Name);

    ///clean
    configPackage->lpVtbl->Release(configPackage);
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_GetConfigurationPackage_with_2_configuration_packages)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A", 
        CONFIGURATION_PACKAGE_NAME,
        "B"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    IFabricConfigurationPackage* configPackage;
    HRESULT hr;

    ///act(1)
    hr = fc_activation_context->lpVtbl->GetConfigurationPackage(fc_activation_context, L"A", &configPackage);

    ///assert(1)
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_IS_NOT_NULL(configPackage);
    ASSERT_ARE_EQUAL(wchar_ptr, L"A", configPackage->lpVtbl->get_Description(configPackage)->Name);

    ///clean(1)
    configPackage->lpVtbl->Release(configPackage);

    ///act(2)
    hr = fc_activation_context->lpVtbl->GetConfigurationPackage(fc_activation_context, L"B", &configPackage);

    ///assert(2)
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_IS_NOT_NULL(configPackage);
    ASSERT_ARE_EQUAL(wchar_ptr, L"B", configPackage->lpVtbl->get_Description(configPackage)->Name);

    ///clean(2)
    configPackage->lpVtbl->Release(configPackage);

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_GetConfigurationPackage_with_unexisting_package)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    IFabricConfigurationPackage* configPackage;
    HRESULT hr;

    ///act
    hr = fc_activation_context->lpVtbl->GetConfigurationPackage(fc_activation_context, L"DOE SNOT EXIST", &configPackage);

    ///assert
    ASSERT_IS_TRUE(hr==E_NOT_SET);

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_get_ServiceEndpointResources_with_1_service_endpoint_resource)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* list;

    ///act
    list = fc_activation_context->lpVtbl->get_ServiceEndpointResources(fc_activation_context);

    ///assert
    ASSERT_IS_NOT_NULL(list);
    ASSERT_ARE_EQUAL(uint64_t, 1, list->Count);
    ASSERT_ARE_EQUAL(wchar_ptr, L"name1", list->Items[0].Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"protocol1", list->Items[0].Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"type1", list->Items[0].Type);
    ASSERT_ARE_EQUAL(uint16_t, 1, list->Items[0].Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate1", list->Items[0].CertificateName);
    ASSERT_IS_NULL(list->Items[0].Reserved);

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_get_ServiceEndpointResources_with_2_service_endpoint_resource)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 6 + 6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* list;

    ///act
    list = fc_activation_context->lpVtbl->get_ServiceEndpointResources(fc_activation_context);

    ///assert
    ASSERT_IS_NOT_NULL(list);
    ASSERT_ARE_EQUAL(uint64_t, 2, list->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"name1", list->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"protocol1", list->Items[0].Protocol);
        ASSERT_ARE_EQUAL(wchar_ptr, L"type1", list->Items[0].Type);
        ASSERT_ARE_EQUAL(uint16_t, 1, list->Items[0].Port);
        ASSERT_ARE_EQUAL(wchar_ptr, L"certificate1", list->Items[0].CertificateName);
        ASSERT_IS_NULL(list->Items[0].Reserved);
    }
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"name2", list->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"protocol2", list->Items[1].Protocol);
        ASSERT_ARE_EQUAL(wchar_ptr, L"type2", list->Items[1].Type);
        ASSERT_ARE_EQUAL(uint16_t, 2, list->Items[1].Port);
        ASSERT_ARE_EQUAL(wchar_ptr, L"certificate2", list->Items[1].CertificateName);
        ASSERT_IS_NULL(list->Items[1].Reserved);
    }

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_GetConfigurationPackage_with_2_configuration_packages_and_2_service_endpoints)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B",
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2+2+6+6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    IFabricConfigurationPackage* configPackage;
    HRESULT hr;
    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST* list;

    ///act(1)
    hr = fc_activation_context->lpVtbl->GetConfigurationPackage(fc_activation_context, L"A", &configPackage);

    ///assert(1)
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_IS_NOT_NULL(configPackage);
    ASSERT_ARE_EQUAL(wchar_ptr, L"A", configPackage->lpVtbl->get_Description(configPackage)->Name);

    ///clean(1)
    configPackage->lpVtbl->Release(configPackage);

    ///act(2)
    hr = fc_activation_context->lpVtbl->GetConfigurationPackage(fc_activation_context, L"B", &configPackage);

    ///assert(2)
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_IS_NOT_NULL(configPackage);
    ASSERT_ARE_EQUAL(wchar_ptr, L"B", configPackage->lpVtbl->get_Description(configPackage)->Name);

    ///clean(2)
    configPackage->lpVtbl->Release(configPackage);

    ///act(3)
    list = fc_activation_context->lpVtbl->get_ServiceEndpointResources(fc_activation_context);

    ///assert(3)
    ASSERT_IS_NOT_NULL(list);
    ASSERT_ARE_EQUAL(uint64_t, 2, list->Count);
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"name1", list->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"protocol1", list->Items[0].Protocol);
        ASSERT_ARE_EQUAL(wchar_ptr, L"type1", list->Items[0].Type);
        ASSERT_ARE_EQUAL(uint16_t, 1, list->Items[0].Port);
        ASSERT_ARE_EQUAL(wchar_ptr, L"certificate1", list->Items[0].CertificateName);
        ASSERT_IS_NULL(list->Items[0].Reserved);
    }
    {
        ASSERT_ARE_EQUAL(wchar_ptr, L"name2", list->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"protocol2", list->Items[1].Protocol);
        ASSERT_ARE_EQUAL(wchar_ptr, L"type2", list->Items[1].Type);
        ASSERT_ARE_EQUAL(uint16_t, 2, list->Items[1].Port);
        ASSERT_ARE_EQUAL(wchar_ptr, L"certificate2", list->Items[1].CertificateName);
        ASSERT_IS_NULL(list->Items[1].Reserved);
    }

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_GetServiceEndpointResource_finds_1_out_of_1)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    HRESULT hr;
    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* desc;

    ///act
    hr = fc_activation_context->lpVtbl->GetServiceEndpointResource(fc_activation_context, L"name1", &desc);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"name1", desc->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"protocol1", desc->Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"type1", desc->Type);
    ASSERT_ARE_EQUAL(uint16_t, 1, desc->Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate1", desc->CertificateName);
    ASSERT_IS_NULL(desc->Reserved);

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_GetServiceEndpointResource_finds_2_out_of_2)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 6+6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    HRESULT hr;
    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* desc;

    ///act(1)
    hr = fc_activation_context->lpVtbl->GetServiceEndpointResource(fc_activation_context, L"name1", &desc);

    ///assert(1)
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"name1", desc->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"protocol1", desc->Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"type1", desc->Type);
    ASSERT_ARE_EQUAL(uint16_t, 1, desc->Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate1", desc->CertificateName);
    ASSERT_IS_NULL(desc->Reserved);

    ///act(2)
    hr = fc_activation_context->lpVtbl->GetServiceEndpointResource(fc_activation_context, L"name2", &desc);

    ///assert(2)
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(wchar_ptr, L"name2", desc->Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"protocol2", desc->Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"type2", desc->Type);
    ASSERT_ARE_EQUAL(uint16_t, 2, desc->Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate2", desc->CertificateName);
    ASSERT_IS_NULL(desc->Reserved);

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_GetServiceEndpointResource_does_not_find_unexisting_endpoint)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;

    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 6 + 6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    HRESULT hr;
    const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION* desc;

    ///act
    hr = fc_activation_context->lpVtbl->GetServiceEndpointResource(fc_activation_context, L"DOES NOT EXIST", &desc);

    ///assert
    ASSERT_IS_TRUE(hr == E_NOT_SET);

    ///clean
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}


TEST_FUNCTION(IFabricCodePackageActivationContext_to_ARGC_ARGV_succeeds_1) 
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B",
        SECTION_NAME_DEFINE,
        "S"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricCodePackageActivationContext_to_ARGC_ARGV(fc_activation_context, &p_argc, &p_argv);
    
    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 6, p_argc);
    ASSERT_ARE_EQUAL(char_ptr, CONFIGURATION_PACKAGE_NAME, p_argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "A", p_argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, CONFIGURATION_PACKAGE_NAME, p_argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "B", p_argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, p_argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "S", p_argv[5]);

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_to_ARGC_ARGV_succeeds_2)
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B",
        SECTION_NAME_DEFINE,
        "S",
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, 2+4+6, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricCodePackageActivationContext_to_ARGC_ARGV(fc_activation_context, &p_argc, &p_argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 12, p_argc);
    ASSERT_ARE_EQUAL(char_ptr, CONFIGURATION_PACKAGE_NAME, p_argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "A", p_argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, CONFIGURATION_PACKAGE_NAME, p_argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "B", p_argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, p_argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "S", p_argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, SERVICE_ENDPOINT_RESOURCE, p_argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "name1", p_argv[7]);
    ASSERT_ARE_EQUAL(char_ptr, "protocol1", p_argv[8]);
    ASSERT_ARE_EQUAL(char_ptr, "type1", p_argv[9]);
    ASSERT_ARE_EQUAL(char_ptr, "1", p_argv[10]);
    ASSERT_ARE_EQUAL(char_ptr, "certificate1", p_argv[11]);

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}

TEST_FUNCTION(IFabricCodePackageActivationContext_to_ARGC_ARGV_succeeds_3) /*has empty section, section with 1 param, section with 2 param and 2 service endpoints*/
{
    ///arrange
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        "A",
        CONFIGURATION_PACKAGE_NAME,
        "B",
        SECTION_NAME_DEFINE,
        "S1",
        SECTION_NAME_DEFINE,
        "S2",
        "p2", 
        "v2",
        SECTION_NAME_DEFINE,
        "S3",
        "p3",
        "v3",
        SERVICE_ENDPOINT_RESOURCE,
        "name1",
        "protocol1",
        "type1",
        "1",
        "certificate1",
        SERVICE_ENDPOINT_RESOURCE,
        "name2",
        "protocol2",
        "type2",
        "2",
        "certificate2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int argc_consumed;
    int i = 0;
    FC_ACTIVATION_CONTEXT_HANDLE activation_context;
    activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_ARE_EQUAL(int, argc, argc_consumed);
    IFabricCodePackageActivationContext* fc_activation_context;
    fc_activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(fc_activation_context);

    int p_argc;
    char** p_argv;
    int result;

    ///act
    result = IFabricCodePackageActivationContext_to_ARGC_ARGV(fc_activation_context, &p_argc, &p_argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, argc, p_argc);
    ASSERT_ARE_EQUAL(char_ptr, CONFIGURATION_PACKAGE_NAME, p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "A", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, CONFIGURATION_PACKAGE_NAME, p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "B", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "p2", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "v2", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "S3", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "p3", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "v3", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, SERVICE_ENDPOINT_RESOURCE, p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "name1", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "protocol1", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "type1", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "1", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "certificate1", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, SERVICE_ENDPOINT_RESOURCE, p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "name2", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "protocol2", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "type2", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "2", p_argv[i++]);
    ASSERT_ARE_EQUAL(char_ptr, "certificate2", p_argv[i++]);

    ///clean
    ARGC_ARGV_free(p_argc, p_argv);
    fc_activation_context->lpVtbl->Release(fc_activation_context);
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
