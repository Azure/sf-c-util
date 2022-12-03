// Copyright (c) Microsoft. All rights reserved.

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
#else
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
#include "umock_c/umocktypes_stdint.h"
#include "umock_c/umocktypes_charptr.h"
#include "umock_c/umocktypes_windows.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/


/*for the purpose of these tests, there's going to be a phony IFabricZZZ interface*/

#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/timer.h"
#include "c_pal/threadapi.h"

#include "ifabriczzzz_sync.h"

MOCKABLE_FUNCTION(, ULONG, DoNothingRelease, IFabricZZZZ*, h);

#undef ENABLE_MOCKS

#include "real_gballoc_hl.h"

#include "hfabriczzzz.h"

static TEST_MUTEX_HANDLE test_serialize_mutex;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)
static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

static ULONG DoNothingRelease_hook(IFabricZZZZ* h)
{
    my_gballoc_free(h); /*no ref counting*/
    return 0;
}

static IFabricZZZZVtbl vtbl =
{
    DoNothingRelease,
    DoSomethingAwesome,
    DoSomethingAwesomeNoSFTimeout,
    DoSomethingWithPossibleFailures,
    DoSomethingWithPossibleFailuresNoSFTimeout
};

/*sort of "reals"*/
static HRESULT MU_C2(real_, CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ))(IFabricZZZZ** fabricVariable)
{
    *fabricVariable = my_gballoc_malloc(sizeof(IFabricZZZZ));
    (*fabricVariable)->lpVtbl = &vtbl;
    return S_OK;
}

#define TIME_START_OF_TIME 2000 /*time starts at 2 second*/
#define TIME_MS_BETWEEN_RETRIES 100 /*APIs have a timeout of 1 second, this includes all retries and individual timeouts*/
#define TIME_TIMEOUT 1000 /*timeout for hte call is 1000*/

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error), "umock_c_init");

    ASSERT_ARE_EQUAL(int, 0, umocktypes_stdint_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_charptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_windows_register_types());

    REGISTER_GLOBAL_MOCK_HOOK(malloc, my_gballoc_malloc);
    REGISTER_GLOBAL_MOCK_HOOK(free, my_gballoc_free);

    REGISTER_GLOBAL_MOCK_HOOK(DoNothingRelease, DoNothingRelease_hook);
    REGISTER_GLOBAL_MOCK_RETURNS(DoSomethingAwesome, S_OK, E_FAIL);
    REGISTER_GLOBAL_MOCK_RETURNS(DoSomethingAwesomeNoSFTimeout, S_OK, E_FAIL);
    REGISTER_GLOBAL_MOCK_RETURNS(DoSomethingWithPossibleFailures, S_OK, E_FAIL);
    REGISTER_GLOBAL_MOCK_RETURNS(DoSomethingWithPossibleFailuresNoSFTimeout, S_OK, E_FAIL);

    REGISTER_GLOBAL_MOCK_HOOK(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ), MU_C2(real_, CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)));

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

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_001: [ H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall allocate memory to hold nMaxRetries, msBetweenRetries and an instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_002: [ H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME by calling CREATE_IFABRICINSTANCE_NAME(IFABRIC_INTERFACE_NAME). ]*/
TEST_FUNCTION(H_FABRIC_HANDLE_CREATE_IFABRIC_INTERFACE_NAME_succeeds)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));

    ///act
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(0,0);

    ///assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_003: [ If there are any failures then H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall fail and return NULL. ]*/
TEST_FUNCTION(H_FABRIC_HANDLE_CREATE_IFABRIC_INTERFACE_NAME_fails_when_CREATE_IFABRICINSTANCE_NAME_IFabricZZZZ_fails)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG))
        .SetReturn(E_FAIL);
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(0, 0);

    ///assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_003: [ If there are any failures then H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall fail and return NULL. ]*/
TEST_FUNCTION(H_FABRIC_HANDLE_CREATE_IFABRIC_INTERFACE_NAME_fails_when_malloc_fails)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .SetReturn(NULL);

    ///act
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(0, 0);

    ///assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_004: [ If handle is NULL then H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME) shall return. ]*/
TEST_FUNCTION(H_FABRIC_HANDLE_DESTROY_IFABRIC_INTERFACE_NAME_with_handle_NULL_returns)
{
    ///arrange

    ///act
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(NULL);

    ///assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_005: [ H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME) shall call ->Release() on the instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_006: [ H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME) shall free the allocated memory. ]*/
TEST_FUNCTION(H_FABRIC_HANDLE_DESTROY_IFABRIC_INTERFACE_NAME_succeeds)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(0, 0);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());


    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(free(result));

    ///act
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);

    ///assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_007: [ If handle is NULL then H_FABRIC_API(IFABRIC_METHOD_NAME) shall fail and return NULL. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_with_handle_NULL_fails)
{
    ///arrange

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(NULL, "a", 0);

    ///assert
    ASSERT_IS_TRUE(FAILED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_008: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall record the start time of the request by calling timer_global_get_elapsed_ms. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_009: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall call IFABRIC_METHOD_NAME on the instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_010: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(0, 0); /*0 retries, 0 ms timeout*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms());
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", 0));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", 0);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_011: [ If the result is FABRIC_E_OBJECT_CLOSED or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_013: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_015: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_010: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_E_ABORT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(E_ABORT);
    /*because of E_ABORT the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + 100);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_011: [ If the result is FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_013: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_015: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_010: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_FABRIC_E_GATEWAY_NOT_REACHABLE)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(FABRIC_E_GATEWAY_NOT_REACHABLE);
    /*because of E_ABORT the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + 100);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_011: [ If the result is FABRIC_E_OBJECT_CLOSED or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_013: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_015: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_010: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_FABRIC_E_OBJECT_CLOSED)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(FABRIC_E_OBJECT_CLOSED);
    /*because of FABRIC_E_OBJECT_CLOSED the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + 100);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_012: [ If creating the new instance of IFABRIC_INTERFACE_NAME fails then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry using the existing IFABRIC_INTERFACE_NAME. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_FABRIC_E_OBJECT_CLOSED_and_object_creation_fails_reuses_old_object)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(FABRIC_E_OBJECT_CLOSED);
    /*because of FABRIC_E_OBJECT_CLOSED the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG))
        .SetReturn(E_FAIL); /*... and re-creation fails so old object is used anyway*/
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + 100);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_014: [ If the result is any other value except FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_E_FAIL)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(E_FAIL);
    /*because of E_FAIL the object is NOT recreated*/
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + 100);
    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_017: [ If the total number of retries exceeds nMaxRetries then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_FABRIC_E_OBJECT_CLOSED_after_2_tries)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(2, TIME_MS_BETWEEN_RETRIES); /*2 tries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    for (int retries = 1; retries <= 2; retries++)
    {
        STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
            .SetReturn(FABRIC_E_OBJECT_CLOSED);
        /*because of FABRIC_E_OBJECT_CLOSED the object is recreated*/
        STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
        STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
        STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
        STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
            .SetReturn(TIME_START_OF_TIME + 100);
    }

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(hr == FABRIC_E_OBJECT_CLOSED);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_016: [ If the total time spend retrying exceeds timeoutMilliseconds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_FABRIC_E_OBJECT_CLOSED_after_1_tries_because_timeout)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(2, TIME_MS_BETWEEN_RETRIES); /*2 tries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);

    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(FABRIC_E_OBJECT_CLOSED);
    /*because of FABRIC_E_OBJECT_CLOSED the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + TIME_TIMEOUT);


    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(hr == FABRIC_E_OBJECT_CLOSED);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_016: [ If the total time spend retrying exceeds timeoutMilliseconds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_FABRIC_E_GATEWAY_NOT_REACHABLE_after_1_retry)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(1, TIME_MS_BETWEEN_RETRIES); /*2 tries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);

    STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(FABRIC_E_GATEWAY_NOT_REACHABLE);
    /*because of FABRIC_E_GATEWAY_NOT_REACHABLE the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + TIME_TIMEOUT);


    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_ARE_EQUAL(HRESULT, FABRIC_E_GATEWAY_NOT_REACHABLE, hr);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_02_016: [ If the total time spend retrying exceeds timeoutMilliseconds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_FABRIC_E_OBJECT_CLOSED_after_2_tries_because_timeout)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 tries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    for (int retries = 1; retries <= 2; retries++)
    {
        STRICT_EXPECTED_CALL(DoSomethingAwesome(IGNORED_ARG, "a", TIME_TIMEOUT))
            .SetReturn(E_ACCESSDENIED);
        /*because of E_ACCESSDENIED the object is reused*/
        STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
        STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
            .SetReturn(TIME_START_OF_TIME + retries * 600); /*retries *600 will record 1600 and 2200 as current time. 2200 exceeds 2000 which is the limit*/
    }

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesome)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(hr == E_ACCESSDENIED);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_42_001: [ If the result is any value from permanent_failures then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_after_1_retry_with_FABRIC_E_INVALID_ADDRESS_which_is_permanent_failure)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    STRICT_EXPECTED_CALL(DoSomethingWithPossibleFailures(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(E_ACCESSDENIED);
    /*because of E_ACCESSDENIED retry as-is*/
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + 100);
    /*does not cause retry*/
    STRICT_EXPECTED_CALL(DoSomethingWithPossibleFailures(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(FABRIC_E_INVALID_ADDRESS);

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingWithPossibleFailures)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(hr == FABRIC_E_INVALID_ADDRESS);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_42_001: [ If the result is any value from permanent_failures then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_after_1_retry_with_FABRIC_E_INVALID_NAME_URI_which_is_permanent_failure)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME);
    STRICT_EXPECTED_CALL(DoSomethingWithPossibleFailures(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(E_ACCESSDENIED);
    /*because of E_ACCESSDENIED retry as-is*/
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(timer_global_get_elapsed_ms())
        .SetReturn(TIME_START_OF_TIME + 100);
    /*does not cause retry*/
    STRICT_EXPECTED_CALL(DoSomethingWithPossibleFailures(IGNORED_ARG, "a", TIME_TIMEOUT))
        .SetReturn(FABRIC_E_INVALID_NAME_URI);

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingWithPossibleFailures)(result, "a", TIME_TIMEOUT);

    ///assert
    ASSERT_IS_TRUE(hr == FABRIC_E_INVALID_NAME_URI);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/* H_FABRIC_DEFINE_API_NO_SF_TIMEOUT */

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_001: [ If handle is NULL then H_FABRIC_API(IFABRIC_METHOD_NAME) shall fail and return NULL. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_with_handle_NULL_fails_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(NULL, "a");

    ///assert
    ASSERT_IS_TRUE(FAILED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_002: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall call IFABRIC_METHOD_NAME on the instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_003: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(0, 0); /*0 retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_004: [ If the result is FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_006: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_008: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_003: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_E_ABORT_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"))
        .SetReturn(E_ABORT);
    /*because of E_ABORT the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_004: [ If the result is FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_006: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_008: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_003: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_FABRIC_E_GATEWAY_NOT_REACHABLE_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"))
        .SetReturn(FABRIC_E_GATEWAY_NOT_REACHABLE);
    /*because of E_ABORT the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_004: [ If the result is FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_006: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_008: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/
/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_003: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_FABRIC_E_OBJECT_CLOSED_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"))
        .SetReturn(FABRIC_E_OBJECT_CLOSED);
    /*because of FABRIC_E_OBJECT_CLOSED the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_005: [ If creating the new instance of IFABRIC_INTERFACE_NAME fails then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry using the existing IFABRIC_INTERFACE_NAME. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_FABRIC_E_OBJECT_CLOSED_and_object_creation_fails_reuses_old_object_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"))
        .SetReturn(FABRIC_E_OBJECT_CLOSED);
    /*because of FABRIC_E_OBJECT_CLOSED the object is recreated*/
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG))
        .SetReturn(E_FAIL); /*... and re-creation fails so old object is used anyway*/
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_007: [ If the result is any other value except FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_succeeds_after_1_retry_with_E_FAIL_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"))
        .SetReturn(E_FAIL);
    /*because of E_FAIL the object is NOT recreated*/
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"));

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(SUCCEEDED(hr));
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_01_009: [ If the total number of retries exceeds nMaxRetries then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_FABRIC_E_OBJECT_CLOSED_after_2_tries_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(2, TIME_MS_BETWEEN_RETRIES); /*2 tries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    for (int retries = 1; retries <= 2; retries++)
    {
        STRICT_EXPECTED_CALL(DoSomethingAwesomeNoSFTimeout(IGNORED_ARG, "a"))
            .SetReturn(FABRIC_E_OBJECT_CLOSED);
        /*because of FABRIC_E_OBJECT_CLOSED the object is recreated*/
        STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
        STRICT_EXPECTED_CALL(DoNothingRelease(IGNORED_ARG));
        STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    }

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingAwesomeNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(hr == FABRIC_E_OBJECT_CLOSED);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

/*Tests_SRS_H_FABRIC_MACRO_GENERATOR_42_002: [ If the result is any value from permanent_failures then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return. ]*/
TEST_FUNCTION(H_FABRIC_API_IFABRIC_METHOD_NAME_returns_after_1_retry_with_FABRIC_E_NAME_NOT_EMPTY_which_is_permanent_failure_for_H_FABRIC_DEFINE_API_NO_SF_TIMEOUT)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(CREATE_IFABRICINSTANCE_NAME(IFabricZZZZ)(IGNORED_ARG));
    H_FABRIC_HANDLE(IFabricZZZZ) result = H_FABRIC_HANDLE_CREATE(IFabricZZZZ)(3, TIME_MS_BETWEEN_RETRIES); /*3 retries, 0.1 s between retries*/
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    STRICT_EXPECTED_CALL(DoSomethingWithPossibleFailuresNoSFTimeout(IGNORED_ARG, "a"))
        .SetReturn(E_ACCESSDENIED);
    /*because of E_ACCESSDENIED retry as-is*/
    STRICT_EXPECTED_CALL(ThreadAPI_Sleep(TIME_MS_BETWEEN_RETRIES));
    /*does not cause retry*/
    STRICT_EXPECTED_CALL(DoSomethingWithPossibleFailuresNoSFTimeout(IGNORED_ARG, "a"))
        .SetReturn(FABRIC_E_NAME_NOT_EMPTY);

    ///act
    HRESULT hr = H_FABRIC_API(DoSomethingWithPossibleFailuresNoSFTimeout)(result, "a");

    ///assert
    ASSERT_IS_TRUE(hr == FABRIC_E_NAME_NOT_EMPTY);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///clean
    H_FABRIC_HANDLE_DESTROY(IFabricZZZZ)(result);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
