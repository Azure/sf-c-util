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

#include "sf_c_util/fc_activation_context_com.h"
#include "sf_c_util/fc_activation_context.h"

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

/*all tests are actually going against IFabricCodePackageActivationContext rather than FC_ACTIVATION_CONTEXT_HANDLE. COM_WRAPPER ROCKS!!!*/
/*including getting the produced IFabricCodePackageActivationContext -> argc/argv*/

TEST_FUNCTION(fc_activation_context_create_succeeds_1) /*in this case, it contains nothing, because argvs don't start with CONFIGURATION_PACKAGE_NAME*/
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

TEST_FUNCTION(fc_activation_context_create_succeeds_3) /*in this case, it contains only the name of the configuration package ("A")*/
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

TEST_FUNCTION(fc_activation_context_create_succeeds_4) /*in this case, it contains only the name of the configuration package ("A")*/
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

TEST_FUNCTION(fc_activation_context_create_succeeds_5) /*in this case, it contains 2 configuration packages, "A" and "B"*/
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

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
