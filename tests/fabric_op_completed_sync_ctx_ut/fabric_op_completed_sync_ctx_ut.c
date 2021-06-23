// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
#else
#include <stdlib.h>
#include <stdint.h>
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
#include "umock_c/umock_c_negative_tests.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_stdint.h"
#include "c_logging/xlogging.h"


#include "fabriccommon.h"
#include "unknwn.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#define ENABLE_MOCKS

#include "c_pal/gballoc_hl.h"

#define GBALLOC_HL_REDIRECT_H
#include "test_callback.h"
#include "test_callback_com.h"
#include "com_wrapper/com_wrapper.h"
DEFINE_COM_WRAPPER_OBJECT(TEST_CALLBACK_HANDLE, TEST_CALLBACK_HANDLE_INTERFACES);
#undef GBALLOC_HL_REDIRECT_H

#include "c_pal/gballoc_hl_redirect.h"
#undef ENABLE_MOCKS

#include "real_gballoc_hl.h"


#include "sf_c_util/fabric_op_completed_sync_ctx.h"


static TEST_MUTEX_HANDLE test_serialize_mutex;

static IFabricAsyncOperationCallback* test_callback_com;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

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

    ASSERT_ARE_EQUAL(int, 0, umocktypes_stdint_register_types(), "umocktypes_stdint_register_types");

    REGISTER_GLOBAL_MOCK_HOOK(malloc, my_gballoc_malloc);
    REGISTER_GLOBAL_MOCK_HOOK(free, my_gballoc_free);

    REGISTER_UMOCK_ALIAS_TYPE(ULONG, unsigned long);
    REGISTER_UMOCK_ALIAS_TYPE(HRESULT, long);
    REGISTER_UMOCK_ALIAS_TYPE(BOOLEAN, uint8_t);
    REGISTER_UMOCK_ALIAS_TYPE(TEST_CALLBACK_HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(TEST_CALLBACK_HANDLE_DESTROY_FUNC, void*);
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

    TEST_CALLBACK_HANDLE test_callback = test_callback_create();
    ASSERT_IS_NOT_NULL(test_callback, "creating test callback failed");

    test_callback_com = COM_WRAPPER_CREATE(TEST_CALLBACK_HANDLE, IFabricAsyncOperationCallback, test_callback, test_callback_destroy);
    ASSERT_IS_NOT_NULL(test_callback_com, "creating test callback COM wrapper failed");

    umock_c_reset_all_calls();
    umock_c_negative_tests_init();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    (void)test_callback_com->lpVtbl->Release(test_callback_com);
    umock_c_negative_tests_deinit();
    TEST_MUTEX_RELEASE(test_serialize_mutex);
}

/* fabric_op_completed_sync_ctx_create */

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_001: [ If callback is NULL, fabric_op_completed_sync_ctx_create shall fail and return NULL. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_create_with_NULL_callback_fails)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx;

    // act
    fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(NULL);

    // assert
    ASSERT_IS_NULL(fabric_op_completed_sync_ctx);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_002: [ Otherwise fabric_op_completed_sync_ctx_create shall allocate a new fabric async operation context and on success return a non-NULL pointer to it. ]*/
/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_003: [ fabric_op_completed_sync_ctx_create shall increment the reference count for callback and store it. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_create_succeeds)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_CALLBACK_HANDLE_IFabricAsyncOperationCallback_AddRef(test_callback_com));

    // act
    fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);

    // assert
    ASSERT_IS_NOT_NULL(fabric_op_completed_sync_ctx);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_op_completed_sync_ctx_destroy(fabric_op_completed_sync_ctx);
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_004: [ If any error occurs, fabric_op_completed_sync_ctx_create shall fail and return NULL. ]*/
TEST_FUNCTION(when_underlying_calls_fail_fabric_op_completed_sync_ctx_create_fails)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_CALLBACK_HANDLE_IFabricAsyncOperationCallback_AddRef(test_callback_com))
        .CallCannotFail();

    umock_c_negative_tests_snapshot();

    for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            // act
            fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);

            //assert
            ASSERT_IS_NULL(fabric_op_completed_sync_ctx, "On failed call %zu", i);
        }
    }
}

/* fabric_op_completed_sync_ctx_destroy */

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_005: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_destroy shall return. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_destroy_with_NULL_fabric_op_completed_sync_ctx_returns)
{
    // arrange

    // act
    fabric_op_completed_sync_ctx_destroy(NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_006: [ Otherwise, fabric_op_completed_sync_ctx_destroy shall decrement the reference count for the callback passed to fabric_op_completed_sync_ctx_create. ]*/
/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_007: [ fabric_op_completed_sync_ctx_destroy shall free the memory associated with fabric_op_completed_sync_ctx. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_destroy_frees_resources)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(TEST_CALLBACK_HANDLE_IFabricAsyncOperationCallback_Release(test_callback_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    // act
    fabric_op_completed_sync_ctx_destroy(fabric_op_completed_sync_ctx);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* fabric_op_completed_sync_ctx_IsCompleted */

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_008: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_IsCompleted shall return FALSE. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_IsCompleted_with_NULL_fabric_op_completed_sync_ctx_returns_FALSE)
{
    // arrange
    BOOLEAN result;

    // act
    result = fabric_op_completed_sync_ctx_IsCompleted(NULL);

    // assert
    ASSERT_IS_FALSE(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_009: [ Otherwise fabric_op_completed_sync_ctx_IsCompleted shall always return TRUE. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_IsCompleted_returns_TRUE)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);
    BOOLEAN result;
    umock_c_reset_all_calls();

    // act
    result = fabric_op_completed_sync_ctx_IsCompleted(fabric_op_completed_sync_ctx);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_op_completed_sync_ctx_destroy(fabric_op_completed_sync_ctx);
}

/* fabric_op_completed_sync_ctx_CompletedSynchronously */

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_010: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_CompletedSynchronously shall return FALSE. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_CompletedSynchronously_with_NULL_fabric_op_completed_sync_ctx_returns_FALSE)
{
    // arrange
    BOOLEAN result;

    // act
    result = fabric_op_completed_sync_ctx_CompletedSynchronously(NULL);

    // assert
    ASSERT_IS_FALSE(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_011: [ Otherwise fabric_op_completed_sync_ctx_CompletedSynchronously shall return TRUE. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_CompletedSynchronously_returns_TRUE)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);
    BOOLEAN result;
    umock_c_reset_all_calls();

    // act
    result = fabric_op_completed_sync_ctx_CompletedSynchronously(fabric_op_completed_sync_ctx);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_op_completed_sync_ctx_destroy(fabric_op_completed_sync_ctx);
}

/* fabric_op_completed_sync_ctx_get_Callback */

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_012: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_get_Callback shall return E_FAIL. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_get_Callback_with_NULL_fabric_op_completed_sync_ctx_fails)
{
    // arrange
    HRESULT result;
    IFabricAsyncOperationCallback* result_callback;

    // act
    result = fabric_op_completed_sync_ctx_get_Callback(NULL, &result_callback);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_013: [ If callback is NULL, fabric_op_completed_sync_ctx_get_Callback shall return E_POINTER. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_get_Callback_with_NULL_callback_fails)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);
    HRESULT result;
    umock_c_reset_all_calls();

    // act
    result = fabric_op_completed_sync_ctx_get_Callback(fabric_op_completed_sync_ctx, NULL);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_POINTER, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_op_completed_sync_ctx_destroy(fabric_op_completed_sync_ctx);
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_014: [ Otherwise fabric_op_completed_sync_ctx_get_Callback shall increment the ref count for the callback passed as argument to fabric_op_completed_sync_ctx_create. ]*/
/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_015: [ fabric_op_completed_sync_ctx_get_Callback shall return in callback the callback passed as argument to fabric_op_completed_sync_ctx_create. ]*/
/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_016: [ On success, fabric_op_completed_sync_ctx_get_Callback shall return S_OK. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_get_Callback_succeeds)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);
    IFabricAsyncOperationCallback* result_callback;
    HRESULT result;
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(TEST_CALLBACK_HANDLE_IFabricAsyncOperationCallback_AddRef(test_callback_com));

    // act
    result = fabric_op_completed_sync_ctx_get_Callback(fabric_op_completed_sync_ctx, &result_callback);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, S_OK, result);
    ASSERT_ARE_EQUAL(void_ptr, test_callback_com, result_callback);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_op_completed_sync_ctx_destroy(fabric_op_completed_sync_ctx);
    (void)result_callback->lpVtbl->Release(result_callback);
}

/* fabric_op_completed_sync_ctx_Cancel */

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_017: [ If fabric_op_completed_sync_ctx is NULL, fabric_op_completed_sync_ctx_Cancel shall return E_FAIL. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_Cancel_with_NULL_fabric_op_completed_sync_ctx_fails)
{
    // arrange
    HRESULT result;

    // act
    result = fabric_op_completed_sync_ctx_Cancel(NULL);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_018: [ Otherwise fabric_op_completed_sync_ctx shall return E_FAIL as a completed operation cannot be cancelled. ]*/
TEST_FUNCTION(fabric_op_completed_sync_ctx_Cancel_returns_E_FAIL)
{
    // arrange
    FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE fabric_op_completed_sync_ctx = fabric_op_completed_sync_ctx_create(test_callback_com);
    HRESULT result;
    umock_c_reset_all_calls();

    // act
    result = fabric_op_completed_sync_ctx_Cancel(NULL);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    fabric_op_completed_sync_ctx_destroy(fabric_op_completed_sync_ctx);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
