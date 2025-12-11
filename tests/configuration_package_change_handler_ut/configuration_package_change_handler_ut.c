// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "configuration_package_change_handler_ut_pch.h"

// use fake objects as we do not expect any acting on those objects by this layer
static IFabricCodePackageActivationContext* test_activation_context = (IFabricCodePackageActivationContext*)0x4244;
static IFabricConfigurationPackage* test_config_package = (IFabricConfigurationPackage*)0x4246;
static IFabricConfigurationPackage* test_previous_config_package = (IFabricConfigurationPackage*)0x4248;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

// Mock callback
static void* g_on_configuration_changed_context;
static IFabricCodePackageActivationContext* g_on_configuration_changed_source;
static IFabricConfigurationPackage* g_on_configuration_changed_previous;
static IFabricConfigurationPackage* g_on_configuration_changed_new;
static int g_on_configuration_changed_call_count;

MOCK_FUNCTION_WITH_CODE(, void, test_on_configuration_changed, void*, context, IFabricCodePackageActivationContext*, source, IFabricConfigurationPackage*, previous_config_package, IFabricConfigurationPackage*, new_config_package)
    g_on_configuration_changed_context = context;
    g_on_configuration_changed_source = source;
    g_on_configuration_changed_previous = previous_config_package;
    g_on_configuration_changed_new = new_config_package;
    g_on_configuration_changed_call_count++;
MOCK_FUNCTION_END()

// Mock vtable functions for IFabricCodePackageActivationContext
static ULONG test_activation_context_AddRef_result;
MOCK_FUNCTION_WITH_CODE(, ULONG, test_activation_context_AddRef, IFabricCodePackageActivationContext*, This)
MOCK_FUNCTION_END(test_activation_context_AddRef_result)

static ULONG test_activation_context_Release_result;
MOCK_FUNCTION_WITH_CODE(, ULONG, test_activation_context_Release, IFabricCodePackageActivationContext*, This)
MOCK_FUNCTION_END(test_activation_context_Release_result)

static HRESULT test_RegisterConfigurationPackageChangeHandler_result;
static LONGLONG test_RegisterConfigurationPackageChangeHandler_callback_handle;
MOCK_FUNCTION_WITH_CODE(, HRESULT, test_RegisterConfigurationPackageChangeHandler, IFabricCodePackageActivationContext*, This, IFabricConfigurationPackageChangeHandler*, callback, LONGLONG*, callbackHandle)
    if (callbackHandle != NULL)
    {
        *callbackHandle = test_RegisterConfigurationPackageChangeHandler_callback_handle;
    }
MOCK_FUNCTION_END(test_RegisterConfigurationPackageChangeHandler_result)

static HRESULT test_UnregisterConfigurationPackageChangeHandler_result;
MOCK_FUNCTION_WITH_CODE(, HRESULT, test_UnregisterConfigurationPackageChangeHandler, IFabricCodePackageActivationContext*, This, LONGLONG, callbackHandle)
MOCK_FUNCTION_END(test_UnregisterConfigurationPackageChangeHandler_result)

// Mock vtable for IFabricConfigurationPackageChangeHandler
static ULONG test_com_handler_Release_result;
MOCK_FUNCTION_WITH_CODE(, ULONG, test_com_handler_Release, IFabricConfigurationPackageChangeHandler*, This)
MOCK_FUNCTION_END(test_com_handler_Release_result)

// Test vtable
static IFabricCodePackageActivationContextVtbl test_activation_context_vtbl = {
    .AddRef = test_activation_context_AddRef,
    .Release = test_activation_context_Release,
    .RegisterConfigurationPackageChangeHandler = test_RegisterConfigurationPackageChangeHandler,
    .UnregisterConfigurationPackageChangeHandler = test_UnregisterConfigurationPackageChangeHandler
};

static IFabricCodePackageActivationContext test_activation_context_instance = {
    .lpVtbl = &test_activation_context_vtbl
};

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error), "umock_c_init");

    ASSERT_ARE_EQUAL(int, 0, umocktypes_windows_register_types());

    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    umock_c_reset_all_calls();
    umock_c_negative_tests_init();

    test_activation_context_AddRef_result = 1;
    test_activation_context_Release_result = 0;
    test_RegisterConfigurationPackageChangeHandler_result = S_OK;
    test_RegisterConfigurationPackageChangeHandler_callback_handle = 0x1234;
    test_UnregisterConfigurationPackageChangeHandler_result = S_OK;
    test_com_handler_Release_result = 0;

    g_on_configuration_changed_context = NULL;
    g_on_configuration_changed_source = NULL;
    g_on_configuration_changed_previous = NULL;
    g_on_configuration_changed_new = NULL;
    g_on_configuration_changed_call_count = 0;
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();
}

/* configuration_package_change_handler_create */

/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_001: [ If activation_context is NULL, configuration_package_change_handler_create shall fail and return NULL. ]*/
TEST_FUNCTION(configuration_package_change_handler_create_with_NULL_activation_context_fails)
{
    // arrange
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    // act
    result = configuration_package_change_handler_create(NULL, test_on_configuration_changed, (void*)0x4242);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_002: [ If on_configuration_changed is NULL, configuration_package_change_handler_create shall fail and return NULL. ]*/
TEST_FUNCTION(configuration_package_change_handler_create_with_NULL_callback_fails)
{
    // arrange
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    // act
    result = configuration_package_change_handler_create(&test_activation_context_instance, NULL, (void*)0x4242);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_004: [ configuration_package_change_handler_create shall allocate memory for the handler structure. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [ configuration_package_change_handler_create shall call fabric_configuration_package_change_handler_create with the on_configuration_changed callback and context to create the core handler. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [ configuration_package_change_handler_create shall create a COM wrapper for IFabricConfigurationPackageChangeHandler using COM_WRAPPER_CREATE with the FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [ configuration_package_change_handler_create shall call AddRef on activation_context to take a reference. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [ configuration_package_change_handler_create shall register the COM handler with Service Fabric by calling activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [ configuration_package_change_handler_create shall store the callback handle returned by RegisterConfigurationPackageChangeHandler. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [ configuration_package_change_handler_create shall succeed and return the handler. ]*/
TEST_FUNCTION(configuration_package_change_handler_create_succeeds_with_NULL_context)
{
    // arrange
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .IgnoreAllCalls();
    STRICT_EXPECTED_CALL(test_activation_context_AddRef(&test_activation_context_instance));
    STRICT_EXPECTED_CALL(test_RegisterConfigurationPackageChangeHandler(&test_activation_context_instance, IGNORED_ARG, IGNORED_ARG));

    // act
    result = configuration_package_change_handler_create(&test_activation_context_instance, test_on_configuration_changed, NULL);

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    configuration_package_change_handler_destroy(result);
}

/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
TEST_FUNCTION(configuration_package_change_handler_create_fails_when_malloc_fails)
{
    // arrange
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .SetReturn(NULL);

    // act
    result = configuration_package_change_handler_create(&test_activation_context_instance, test_on_configuration_changed, (void*)0x4242);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If any error occurs, configuration_package_change_handler_create shall fail and return NULL. ]*/
TEST_FUNCTION(configuration_package_change_handler_create_fails_when_register_fails)
{
    // arrange
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;
    test_RegisterConfigurationPackageChangeHandler_result = E_FAIL;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .IgnoreAllCalls();
    STRICT_EXPECTED_CALL(free(IGNORED_ARG))
        .IgnoreAllCalls();
    STRICT_EXPECTED_CALL(test_activation_context_AddRef(&test_activation_context_instance));
    STRICT_EXPECTED_CALL(test_RegisterConfigurationPackageChangeHandler(&test_activation_context_instance, IGNORED_ARG, IGNORED_ARG));
    // Note: COM handler Release is handled internally by the COM wrapper, not through test mock
    STRICT_EXPECTED_CALL(test_activation_context_Release(&test_activation_context_instance));

    // act
    result = configuration_package_change_handler_create(&test_activation_context_instance, test_on_configuration_changed, (void*)0x4242);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* configuration_package_change_handler_destroy */

/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [ If handle is NULL, configuration_package_change_handler_destroy shall return. ]*/
TEST_FUNCTION(configuration_package_change_handler_destroy_with_NULL_handle_returns)
{
    // arrange

    // act
    configuration_package_change_handler_destroy(NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_013: [ configuration_package_change_handler_destroy shall unregister from Service Fabric by calling activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler with the stored callback handle. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_014: [ configuration_package_change_handler_destroy shall call Release on the COM wrapper (which will trigger destruction of the underlying fabric handler). ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_015: [ configuration_package_change_handler_destroy shall call Release on the stored activation_context. ]*/
/* Tests_SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_016: [ configuration_package_change_handler_destroy shall free the handler structure. ]*/
TEST_FUNCTION(configuration_package_change_handler_destroy_succeeds)
{
    // arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .IgnoreAllCalls();
    STRICT_EXPECTED_CALL(test_activation_context_AddRef(&test_activation_context_instance));
    STRICT_EXPECTED_CALL(test_RegisterConfigurationPackageChangeHandler(&test_activation_context_instance, IGNORED_ARG, IGNORED_ARG));
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle = configuration_package_change_handler_create(&test_activation_context_instance, test_on_configuration_changed, (void*)0x4242);
    ASSERT_IS_NOT_NULL(handle);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(test_UnregisterConfigurationPackageChangeHandler(&test_activation_context_instance, test_RegisterConfigurationPackageChangeHandler_callback_handle));
    // Note: COM handler Release is handled internally by the COM wrapper, not through test mock
    STRICT_EXPECTED_CALL(free(IGNORED_ARG))
        .IgnoreAllCalls();
    STRICT_EXPECTED_CALL(test_activation_context_Release(&test_activation_context_instance));

    // act
    configuration_package_change_handler_destroy(handle);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
