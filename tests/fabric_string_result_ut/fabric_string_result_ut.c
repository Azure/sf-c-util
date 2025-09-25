// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "fabric_string_result_ut_pch.h"


MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error));

    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(malloc, NULL);
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
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();
}

/* fabric_string_result_create */

/* Tests_SRS_FABRIC_STRING_RESULT_01_008: [ If string_result is NULL, fabric_string_result_create shall fail and return NULL. ]*/
TEST_FUNCTION(fabric_string_result_create_with_NULL_string_result_fails)
{
    // arrange
    FABRIC_STRING_RESULT_HANDLE fabric_string_result;

    // act
    fabric_string_result = fabric_string_result_create(NULL);

    // assert
    ASSERT_IS_NULL(fabric_string_result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_STRING_RESULT_01_001: [ fabric_string_result_create shall allocate a new fabric string result instance and on success return a non-NULL pointer to it. ]*/
TEST_FUNCTION(fabric_string_result_create_succeeds)
{
    // arrange
    FABRIC_STRING_RESULT_HANDLE fabric_string_result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));

    // act
    fabric_string_result = fabric_string_result_create(L"hagauaga");

    // assert
    ASSERT_IS_NOT_NULL(fabric_string_result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_string_result_destroy(fabric_string_result);
}

/* Tests_SRS_FABRIC_STRING_RESULT_01_003: [ If any error occurs, fabric_string_result_create shall fail and return NULL. ]*/
TEST_FUNCTION(when_malloc_fails_fabric_string_result_create_also_fails)
{
    // arrange
    FABRIC_STRING_RESULT_HANDLE fabric_string_result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .SetReturn(NULL);

    // act
    fabric_string_result = fabric_string_result_create(L"hagauaga");

    // assert
    ASSERT_IS_NULL(fabric_string_result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* fabric_string_result_destroy */

/* Tests_SRS_FABRIC_STRING_RESULT_01_004: [ If fabric_string_result is NULL, fabric_string_result_destroy shall return. ]*/
TEST_FUNCTION(fabric_string_result_destroy_with_NULL_fabric_string_result_returns)
{
    // arrange

    // act
    fabric_string_result_destroy(NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_STRING_RESULT_01_005: [ Otherwise, fabric_string_result_destroy shall free the memory associated with the fabric string result instance. ]*/
TEST_FUNCTION(fabric_string_result_destroy_frees_resources)
{
    // arrange
    FABRIC_STRING_RESULT_HANDLE fabric_string_result = fabric_string_result_create(L"hagauaga");
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    // act
    fabric_string_result_destroy(fabric_string_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* fabric_string_result_get_String */

/* Tests_SRS_FABRIC_STRING_RESULT_01_006: [ If fabric_string_result is NULL, fabric_string_result_get_String shall fail and return NULL. ]*/
TEST_FUNCTION(fabric_string_result_get_String_with_NULL_fabric_string_result_fails)
{
    // arrange
    LPCWSTR result;

    // act
    result = fabric_string_result_get_String(NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_IS_NULL(result);
}

/* Tests_SRS_FABRIC_STRING_RESULT_01_007: [ Otherwise, fabric_string_result_get_String shall return a pointer to the string copied in fabric_string_result_create. ]*/
/* Tests_SRS_FABRIC_STRING_RESULT_01_002: [ fabric_string_result_create shall copy the string_result string so it can be returned by get_String. ]*/
TEST_FUNCTION(fabric_string_result_get_String_returns_the_string_passed_to_create)
{
    // arrange
    FABRIC_STRING_RESULT_HANDLE fabric_string_result = fabric_string_result_create(L"hagauaga");
    LPCWSTR result;
    umock_c_reset_all_calls();

    // act
    result = fabric_string_result_get_String(fabric_string_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(wchar_ptr, L"hagauaga", result);

    // cleanup
    fabric_string_result_destroy(fabric_string_result);
}

/* Tests_SRS_FABRIC_STRING_RESULT_01_007: [ Otherwise, fabric_string_result_get_String shall return a pointer to the string copied in fabric_string_result_create. ]*/
/* Tests_SRS_FABRIC_STRING_RESULT_01_002: [ fabric_string_result_create shall copy the string_result string so it can be returned by get_String. ]*/
TEST_FUNCTION(fabric_string_result_get_String_returns_empty_string)
{
    // arrange
    FABRIC_STRING_RESULT_HANDLE fabric_string_result = fabric_string_result_create(L"");
    LPCWSTR result;
    umock_c_reset_all_calls();

    // act
    result = fabric_string_result_get_String(fabric_string_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(wchar_ptr, L"", result);

    // cleanup
    fabric_string_result_destroy(fabric_string_result);
}

/* Tests_SRS_FABRIC_STRING_RESULT_01_007: [ Otherwise, fabric_string_result_get_String shall return a pointer to the string copied in fabric_string_result_create. ]*/
/* Tests_SRS_FABRIC_STRING_RESULT_01_002: [ fabric_string_result_create shall copy the string_result string so it can be returned by get_String. ]*/
TEST_FUNCTION(fabric_string_result_get_String_returns_the_string_twice)
{
    // arrange
    FABRIC_STRING_RESULT_HANDLE fabric_string_result = fabric_string_result_create(L"x");
    LPCWSTR result_1;
    LPCWSTR result_2;
    umock_c_reset_all_calls();

    // act
    result_1 = fabric_string_result_get_String(fabric_string_result);
    result_2 = fabric_string_result_get_String(fabric_string_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(wchar_ptr, L"x", result_1);
    ASSERT_ARE_EQUAL(wchar_ptr, L"x", result_2);

    // cleanup
    fabric_string_result_destroy(fabric_string_result);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
