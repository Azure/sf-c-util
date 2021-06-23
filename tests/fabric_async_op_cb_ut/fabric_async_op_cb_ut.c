// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include "macro_utils/macro_utils.h"

#include "real_gballoc_ll.h"
static void* my_gballoc_malloc(size_t size)
{
    return real_gballoc_ll_malloc(size);
}

static void my_gballoc_free(void* ptr)
{
     real_gballoc_ll_free(ptr);
}

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#define ENABLE_MOCKS

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#undef ENABLE_MOCKS

#include "real_gballoc_hl.h"


#include "sf_c_util/fabric_async_op_cb.h"

static TEST_MUTEX_HANDLE test_serialize_mutex;
// use a fake object as we do not expect any acting on that object by this layer
static IFabricAsyncOperationContext* test_async_operation_context = (IFabricAsyncOperationContext*)0x4244;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

MOCK_FUNCTION_WITH_CODE(, void, test_user_callback, void*, user_context, IFabricAsyncOperationContext*, fabric_async_operation_context)
MOCK_FUNCTION_END()

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error), "umock_c_init");

    REGISTER_GLOBAL_MOCK_HOOK(malloc, my_gballoc_malloc);
    REGISTER_GLOBAL_MOCK_HOOK(free, my_gballoc_free);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    TEST_MUTEX_DESTROY(test_serialize_mutex);

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    if (TEST_MUTEX_ACQUIRE(test_serialize_mutex))
    {
        ASSERT_FAIL("Could not acquire test serialization mutex.");
    }

    umock_c_reset_all_calls();
    umock_c_negative_tests_init();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();
    TEST_MUTEX_RELEASE(test_serialize_mutex);
}

/* fabric_async_op_cb_create */

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_001: [ If user_invoke_cb is NULL, fabric_async_op_cb_create shall fail and return NULL. ]*/
TEST_FUNCTION(fabric_async_op_cb_create_with_NULL_user_invoke_cb_fails)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE result;

    // act
    result = fabric_async_op_cb_create(NULL, (void*)0x4242);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_003: [ Otherwise, fabric_async_op_cb_create shall allocate a new callback instance and on success return a non-NULL pointer to it. ]*/
TEST_FUNCTION(fabric_async_op_cb_create_succeeds)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));

    // act
    result = fabric_async_op_cb_create(test_user_callback, (void*)0x4242);

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_async_op_cb_destroy(result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_002: [ user_invoke_cb_context shall be allowed to be NULL. ]*/
TEST_FUNCTION(fabric_async_op_cb_create_with_NULL_user_invoke_cb_context_succeeds)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));

    // act
    result = fabric_async_op_cb_create(test_user_callback, NULL);

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_async_op_cb_destroy(result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_004: [ If any error occurs, fabric_async_op_cb_create shall fail and return NULL. ]*/
TEST_FUNCTION(when_underlying_calls_fails_fabric_async_op_cb_create_also_fails)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));

    umock_c_negative_tests_snapshot();

    for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            // act
            result = fabric_async_op_cb_create(test_user_callback, (void*)0x4242);

            //assert
            ASSERT_IS_NULL(result, "On failed call %zu", i);
        }
    }
}

/* fabric_async_op_cb_destroy */

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_005: [ If fabric_async_op_cb is NULL, fabric_async_op_cb_destroy shall return. ]*/
TEST_FUNCTION(fabric_async_op_cb_destroy_with_NULL_fabric_async_op_cb_returns)
{
    // arrange

    // act
    fabric_async_op_cb_destroy(NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_006: [ Otherwise, fabric_async_op_cb_destroy shall free the memory allocated in fabric_async_op_cb_create. ]*/
TEST_FUNCTION(fabric_async_op_cb_destroy_frees_the_memory)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb = fabric_async_op_cb_create(test_user_callback, (void*)0x4242);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    // act
    fabric_async_op_cb_destroy(fabric_async_op_cb);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* fabric_async_op_cb_Invoke */

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_007: [ If fabric_async_op_cb is NULL, fabric_async_op_cb_Invoke shall return. ]*/
TEST_FUNCTION(fabric_async_op_cb_Invoke_with_NULL_fabric_async_op_cb_returns)
{
    // arrange

    // act
    fabric_async_op_cb_Invoke(NULL, test_async_operation_context);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_008: [ Otherwise fabric_async_op_cb_Invoke shall call the callback user_invoke_cb and pass as arguments user_invoke_cb_context and context. ]*/
TEST_FUNCTION(fabric_async_op_cb_Invoke_calls_the_user_callback)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb = fabric_async_op_cb_create(test_user_callback, (void*)0x4242);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(test_user_callback((void*)0x4242, test_async_operation_context));

    // act
    fabric_async_op_cb_Invoke(fabric_async_op_cb, test_async_operation_context);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_async_op_cb_destroy(fabric_async_op_cb);
}

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_008: [ Otherwise fabric_async_op_cb_Invoke shall call the callback user_invoke_cb and pass as arguments user_invoke_cb_context and context. ]*/
TEST_FUNCTION(fabric_async_op_cb_Invoke_twice_calls_the_user_callback_twice)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb = fabric_async_op_cb_create(test_user_callback, (void*)0x4242);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(test_user_callback((void*)0x4242, (IFabricAsyncOperationContext*)0x4243));
    STRICT_EXPECTED_CALL(test_user_callback((void*)0x4242, (IFabricAsyncOperationContext*)0x4244));

    // act
    fabric_async_op_cb_Invoke(fabric_async_op_cb, (IFabricAsyncOperationContext*)0x4243);
    fabric_async_op_cb_Invoke(fabric_async_op_cb, (IFabricAsyncOperationContext*)0x4244);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_async_op_cb_destroy(fabric_async_op_cb);
}

/* Tests_SRS_FABRIC_ASYNC_OP_CB_01_008: [ Otherwise fabric_async_op_cb_Invoke shall call the callback user_invoke_cb and pass as arguments user_invoke_cb_context and context. ]*/
TEST_FUNCTION(fabric_async_op_cb_Invoke_with_NULL_async_op_context_calls_the_user_callback_with_NULL_async_op_context)
{
    // arrange
    FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb = fabric_async_op_cb_create(test_user_callback, (void*)0x4243);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(test_user_callback((void*)0x4243, NULL));

    // act
    fabric_async_op_cb_Invoke(fabric_async_op_cb, NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_async_op_cb_destroy(fabric_async_op_cb);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
