// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "fabric_configuration_package_change_handler_ut_pch.h"

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

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error), "umock_c_init");

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

/* fabric_configuration_package_change_handler_create */

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_001: [ If on_configuration_changed is NULL, fabric_configuration_package_change_handler_create shall fail and return NULL. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_create_with_NULL_callback_fails)
{
    // arrange
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    // act
    result = fabric_configuration_package_change_handler_create(NULL, (void*)0x4242);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_002: [ context is allowed to be NULL. ]*/
/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_003: [ fabric_configuration_package_change_handler_create shall allocate memory for the handler structure. ]*/
/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_004: [ If memory allocation fails, fabric_configuration_package_change_handler_create shall fail and return NULL.]*/
/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [ fabric_configuration_package_change_handler_create shall succeed and return the handler. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_create_succeeds_with_NULL_context)
{
    // arrange
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));

    // act
    result = fabric_configuration_package_change_handler_create(test_on_configuration_changed, NULL);

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_configuration_package_change_handler_destroy(result);
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_003: [ fabric_configuration_package_change_handler_create shall allocate memory for the handler structure. ]*/
/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_004: [ If memory allocation fails, fabric_configuration_package_change_handler_create shall fail and return NULL.]*/
/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [ fabric_configuration_package_change_handler_create shall succeed and return the handler. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_create_succeeds_with_context)
{
    // arrange
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;
    void* test_context = (void*)0x4242;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));

    // act
    result = fabric_configuration_package_change_handler_create(test_on_configuration_changed, test_context);

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_configuration_package_change_handler_destroy(result);
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [ fabric_configuration_package_change_handler_create shall store the on_configuration_changed callback and context.]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_create_fails_when_malloc_fails)
{
    // arrange
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .SetReturn(NULL);

    // act
    result = fabric_configuration_package_change_handler_create(test_on_configuration_changed, (void*)0x4242);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [ fabric_configuration_package_change_handler_create shall store the on_configuration_changed callback and context.]*/
TEST_FUNCTION(when_underlying_calls_fail_fabric_configuration_package_change_handler_create_also_fails)
{
    // arrange
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));

    umock_c_negative_tests_snapshot();

    for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            // act
            result = fabric_configuration_package_change_handler_create(test_on_configuration_changed, (void*)0x4242);

            // assert
            ASSERT_IS_NULL(result, "On failed call %zu", i);
        }
    }
}

/* fabric_configuration_package_change_handler_destroy */

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [ If handle is NULL, fabric_configuration_package_change_handler_destroy shall return. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_destroy_with_NULL_handle_returns)
{
    // arrange

    // act
    fabric_configuration_package_change_handler_destroy(NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [ fabric_configuration_package_change_handler_destroy shall free the handler structure. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_destroy_frees_the_memory)
{
    // arrange
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle = fabric_configuration_package_change_handler_create(test_on_configuration_changed, (void*)0x4242);
    ASSERT_IS_NOT_NULL(handle);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    // act
    fabric_configuration_package_change_handler_destroy(handle);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* fabric_configuration_package_change_handler_on_package_added */

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [ fabric_configuration_package_change_handler_on_package_added shall do nothing. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_on_package_added_does_nothing)
{
    // arrange
    void* test_context = (void*)0x4242;
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle = fabric_configuration_package_change_handler_create(test_on_configuration_changed, test_context);
    ASSERT_IS_NOT_NULL(handle);
    umock_c_reset_all_calls();

    // act
    fabric_configuration_package_change_handler_on_package_added(handle, test_activation_context, test_config_package);

    // assert
    ASSERT_ARE_EQUAL(int, 0, g_on_configuration_changed_call_count);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_configuration_package_change_handler_destroy(handle);
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [ fabric_configuration_package_change_handler_on_package_added shall do nothing. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_on_package_added_with_NULL_handle_does_nothing)
{
    // arrange

    // act
    fabric_configuration_package_change_handler_on_package_added(NULL, test_activation_context, test_config_package);

    // assert
    ASSERT_ARE_EQUAL(int, 0, g_on_configuration_changed_call_count);
}

/* fabric_configuration_package_change_handler_on_package_removed */

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [ fabric_configuration_package_change_handler_on_package_removed shall do nothing. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_on_package_removed_does_nothing)
{
    // arrange
    void* test_context = (void*)0x4242;
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle = fabric_configuration_package_change_handler_create(test_on_configuration_changed, test_context);
    ASSERT_IS_NOT_NULL(handle);
    umock_c_reset_all_calls();

    // act
    fabric_configuration_package_change_handler_on_package_removed(handle, test_activation_context, test_config_package);

    // assert
    ASSERT_ARE_EQUAL(int, 0, g_on_configuration_changed_call_count);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_configuration_package_change_handler_destroy(handle);
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [ fabric_configuration_package_change_handler_on_package_removed shall do nothing. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_on_package_removed_with_NULL_handle_does_nothing)
{
    // arrange

    // act
    fabric_configuration_package_change_handler_on_package_removed(NULL, test_activation_context, test_config_package);

    // assert
    ASSERT_ARE_EQUAL(int, 0, g_on_configuration_changed_call_count);
}

/* fabric_configuration_package_change_handler_on_package_modified */

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [ If handle is NULL, fabric_configuration_package_change_handler_on_package_modified shall return. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_on_package_modified_with_NULL_handle_returns)
{
    // arrange

    // act
    fabric_configuration_package_change_handler_on_package_modified(NULL, test_activation_context, test_previous_config_package, test_config_package);

    // assert
    ASSERT_ARE_EQUAL(int, 0, g_on_configuration_changed_call_count);
}

/* Tests_SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [ fabric_configuration_package_change_handler_on_package_modified shall call the on_configuration_changed callback with previous_config_package set to previousConfigPackage and new_config_package set to configPackage. ]*/
TEST_FUNCTION(fabric_configuration_package_change_handler_on_package_modified_calls_callback)
{
    // arrange
    void* test_context = (void*)0x4242;
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle = fabric_configuration_package_change_handler_create(test_on_configuration_changed, test_context);
    ASSERT_IS_NOT_NULL(handle);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(test_on_configuration_changed(test_context, test_activation_context, test_previous_config_package, test_config_package));

    // act
    fabric_configuration_package_change_handler_on_package_modified(handle, test_activation_context, test_previous_config_package, test_config_package);

    // assert
    ASSERT_ARE_EQUAL(int, 1, g_on_configuration_changed_call_count);
    ASSERT_ARE_EQUAL(void_ptr, test_context, g_on_configuration_changed_context);
    ASSERT_ARE_EQUAL(void_ptr, test_activation_context, g_on_configuration_changed_source);
    ASSERT_ARE_EQUAL(void_ptr, test_previous_config_package, g_on_configuration_changed_previous);
    ASSERT_ARE_EQUAL(void_ptr, test_config_package, g_on_configuration_changed_new);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_configuration_package_change_handler_destroy(handle);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
