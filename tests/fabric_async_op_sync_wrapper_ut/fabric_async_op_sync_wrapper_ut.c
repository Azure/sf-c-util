// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "fabric_async_op_sync_wrapper_ut_pch.h"

#define ENABLE_MOCKS
#undef ENABLE_MOCKS_DECL
#include "umock_c/umock_c_prod.h"
#include "../../src/fabric_async_op_cb_com.c"
#include "test_fabric_async_operation_com.c"
#include "test_async_operation_context_com.c"
#undef ENABLE_MOCKS


static ITestAsyncOperation* test_async_operation_com;
static IFabricAsyncOperationContext* test_async_operation_context_com;
static FABRIC_ASYNC_OP_CB_HANDLE test_fabric_async_op_cb = (FABRIC_ASYNC_OP_CB_HANDLE)0x4242;
static bool call_callback_with_NULL_context;
static bool call_callback_with_NULL_fabric_async_operation_context;
static bool call_sync_wrapper_callback;
static USER_INVOKE_CB sync_wrapper_cb;
static void* sync_wrapper_cb_context;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

static HRESULT my_TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(ITestAsyncOperation* com_object, int arg1, IFabricAsyncOperationCallback* callback, IFabricAsyncOperationContext** context)
{
    (void)com_object;
    (void)arg1;
    (void)callback;
    (void)context;
    if (call_callback_with_NULL_context)
    {
        // call once with NULL context
        sync_wrapper_cb(NULL, test_async_operation_context_com);
        // call once with the real context to unblock
        sync_wrapper_cb(sync_wrapper_cb_context, test_async_operation_context_com);
    } else if (call_callback_with_NULL_fabric_async_operation_context)
    {
        // call once with NULL context
        sync_wrapper_cb(sync_wrapper_cb_context, NULL);
        // call once with the real context to unblock
        sync_wrapper_cb(sync_wrapper_cb_context, test_async_operation_context_com);
    }
    else if (call_sync_wrapper_callback)
    {
        sync_wrapper_cb(sync_wrapper_cb_context, test_async_operation_context_com);
    }
    return S_OK;
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error), "umock_c_init");
    ASSERT_ARE_EQUAL(int, 0, umocktypes_stdint_register_types(), "umocktypes_stdint_register_types");
    ASSERT_ARE_EQUAL(int, 0, umocktypes_windows_register_types(), "umocktypes_windows_register_types");

    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(malloc, NULL);
    REGISTER_GLOBAL_MOCK_RETURNS(fabric_async_op_cb_create, test_fabric_async_op_cb, NULL);
    REGISTER_GLOBAL_MOCK_RETURN(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously, TRUE);
    REGISTER_GLOBAL_MOCK_HOOK(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation, my_TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation);

    REGISTER_UMOCK_ALIAS_TYPE(USER_INVOKE_CB, void*);
    REGISTER_UMOCK_ALIAS_TYPE(FABRIC_ASYNC_OP_CB_HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(FABRIC_ASYNC_OP_CB_HANDLE_DESTROY_FUNC, void*);
    REGISTER_UMOCK_ALIAS_TYPE(TEST_FABRIC_ASYNC_OPERATION_HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(TEST_FABRIC_ASYNC_OPERATION_HANDLE_DESTROY_FUNC, void*);
    REGISTER_UMOCK_ALIAS_TYPE(TEST_ASYNC_OPERATION_CONTEXT_HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_DESTROY_FUNC, void*);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    call_callback_with_NULL_context = false;
    call_callback_with_NULL_fabric_async_operation_context = false;
    call_sync_wrapper_callback = false;

    TEST_FABRIC_ASYNC_OPERATION_HANDLE test_fabric_async_operation = test_fabric_async_operation_create();
    ASSERT_IS_NOT_NULL(test_fabric_async_operation);
    test_async_operation_com = COM_WRAPPER_CREATE(TEST_FABRIC_ASYNC_OPERATION_HANDLE, ITestAsyncOperation, test_fabric_async_operation, test_fabric_async_operation_destroy);
    ASSERT_IS_NOT_NULL(test_async_operation_com);

    TEST_ASYNC_OPERATION_CONTEXT_HANDLE test_async_operation_context = test_async_operation_context_create();
    ASSERT_IS_NOT_NULL(test_async_operation_context);
    test_async_operation_context_com = COM_WRAPPER_CREATE(TEST_ASYNC_OPERATION_CONTEXT_HANDLE, IFabricAsyncOperationContext, test_async_operation_context, test_async_operation_context_destroy);
    ASSERT_IS_NOT_NULL(test_async_operation_context_com);

    umock_c_reset_all_calls();
    umock_c_negative_tests_init();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();
    (void)test_async_operation_com->lpVtbl->Release(test_async_operation_com);
    (void)test_async_operation_context_com->lpVtbl->Release(test_async_operation_context_com);
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_003: [ If com_object is NULL, _execute shall fail and return E_INVALIDARG. ]*/
TEST_FUNCTION(fabric_async_op_sync_wrapper_execute_with_NULL_com_object_fails)
{
    // arrange
    HRESULT result;
    int operation_result_1;
    double operation_result_2;

    // act
    result = ITestAsyncOperation_TestOperation_execute(NULL, 42, &operation_result_1, &operation_result_2);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_020: [ If any of the end arg pointers is NULL, _execute shall fail and return E_INVALIDARG. ]*/
TEST_FUNCTION(fabric_async_op_sync_wrapper_execute_with_NULL_end_arg_1)
{
    // arrange
    HRESULT result;
    double operation_result_2;

    // act
    result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, NULL, &operation_result_2);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_020: [ If any of the end arg pointers is NULL, _execute shall fail and return E_INVALIDARG. ]*/
TEST_FUNCTION(fabric_async_op_sync_wrapper_execute_with_NULL_end_arg_2)
{
    // arrange
    HRESULT result;
    int operation_result_1;

    // act
    result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_result_1, NULL);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_001: [ DECLARE_FABRIC_ASYNC_OPERATION_SYNC shall declare a function with the following prototype: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_002: [ DEFINE_FABRIC_ASYNC_OPERATION_SYNC shall implement a function with the following prototype: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_004: [ _execute shall create a async operation callback object by calling fabric_async_op_cb_create, passing as arguments the wrapper complete callback. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_005: [ A COM wrapper shall be created for the async operation callback object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_006: [ _execute shall call Begin{operation_name} on com_object, passing as arguments the begin arguments and the async operation callback COM object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_008: [ _execute shall call End{operation_name} on com_object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_009: [ _execute shall release the asynchronous operation context obtained from Begin{operation_name}. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_010: [ On success, _execute shall return S_OK. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_014: [ DEFINE_FABRIC_ASYNC_OPERATION_SYNC shall define the wrapper completion callback: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_017: [ Otherwise, _sync_wrapper_cb shall check whether the async operation has completed synchronously. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_018: [ If the async operation has completed synchronously, _sync_wrapper_cb shall return. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_007: [ If fabric_operation_context has not completed synchronously, _execute shall wait to be signalled by the _sync_wrapper_cb function. ]*/
TEST_FUNCTION(fabric_async_op_sync_wrapper_execute_succeeds_when_completed_synchronously)
{
    // arrange
    HRESULT result;
    int injected_operation_int_result = 43;
    double injected_operation_double_result = 1.42;
    int operation_int_result = 0;
    double operation_double_result = 0.00;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &injected_operation_int_result, sizeof(injected_operation_int_result))
        .CopyOutArgumentBuffer(4, &injected_operation_double_result, sizeof(injected_operation_double_result));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    // act
    result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, S_OK, result);
    ASSERT_ARE_EQUAL(int, injected_operation_int_result, operation_int_result);
    ASSERT_ARE_EQUAL(double, injected_operation_double_result, operation_double_result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_011: [ If Begin{operation_name} fails, _execute shall return the error returned by Begin{operation_name}. ]*/
TEST_FUNCTION(when_Begin_fails_fabric_async_op_sync_wrapper_execute_also_fails)
{
    // arrange
    HRESULT result;
    int operation_int_result = 0;
    double operation_double_result = 0.00;

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .SetReturn(E_FAIL);
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    // act
    result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_011: [ If Begin{operation_name} fails, _execute shall return the error returned by Begin{operation_name}. ]*/
TEST_FUNCTION(when_Begin_fails_fabric_async_op_sync_wrapper_execute_also_fails_with_E_INVALIDARG)
{
    // arrange
    HRESULT result;
    int operation_int_result = 0;
    double operation_double_result = 0.00;

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .SetReturn(E_INVALIDARG);
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    // act
    result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_012: [ If End{operation_name} fails, _execute shall return the error returned by End{operation_name}. ]*/
TEST_FUNCTION(when_End_fails_fabric_async_op_sync_wrapper_execute_also_fails_with_E_FAIL)
{
    // arrange
    HRESULT result;
    int operation_int_result = 0;
    double operation_double_result = 0.00;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG, IGNORED_ARG))
        .SetReturn(E_FAIL);
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    // act
    result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_012: [ If End{operation_name} fails, _execute shall return the error returned by End{operation_name}. ]*/
TEST_FUNCTION(when_End_fails_fabric_async_op_sync_wrapper_execute_also_fails_with_E_INVALIDARG)
{
    // arrange
    HRESULT result;
    int operation_int_result = 0;
    double operation_double_result = 0.00;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG, IGNORED_ARG))
        .SetReturn(E_INVALIDARG);
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    // act
    result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_013: [ If any other error occurs, _execute shall fail and return E_FAIL. ]*/
TEST_FUNCTION(when_underlying_calls_fail_fabric_async_op_sync_wrapper_execute_fails)
{
    // arrange
    HRESULT result;
    int operation_int_result = 0;
    double operation_double_result = 0.00;
    int injected_operation_int_result = 43;
    double injected_operation_double_result = 1.42;

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com))
        .CallCannotFail(); // tested with other tests
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE)
        .CallCannotFail();
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &injected_operation_int_result, sizeof(injected_operation_int_result))
        .CopyOutArgumentBuffer(4, &injected_operation_double_result, sizeof(injected_operation_double_result))
        .CallCannotFail(); // tested with other tests
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    umock_c_negative_tests_snapshot();

    for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            if (i > 2)
            {
                (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
            }

            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            // act
            result = ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result);

            //assert
            ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result, "On failed call %zu", i);
        }
    }
}

static void setup_success_async_execute(void)
{
    int injected_operation_int_result = 43;
    double injected_operation_double_result = 1.42;
    int operation_int_result = 0;
    double operation_double_result = 0.00;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG))
        .CaptureArgumentValue_user_invoke_cb(&sync_wrapper_cb)
        .CaptureArgumentValue_user_invoke_cb_context(&sync_wrapper_cb_context);
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));

    if (call_callback_with_NULL_context ||
        call_callback_with_NULL_fabric_async_operation_context)
    {
        STRICT_EXPECTED_CALL(ps_util_terminate_process());
    }

    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE); // called by callback
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE);
    STRICT_EXPECTED_CALL(InterlockedHL_WaitForValue(IGNORED_ARG, IGNORED_ARG, UINT32_MAX));

    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &injected_operation_int_result, sizeof(injected_operation_int_result))
        .CopyOutArgumentBuffer(4, &injected_operation_double_result, sizeof(injected_operation_double_result));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    ASSERT_ARE_EQUAL(HRESULT, S_OK, ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result));
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_015: [ If context is NULL, _sync_wrapper_cb shall terminate the process. ]*/
TEST_FUNCTION(sync_wrapper_cb_with_NULL_context_returns)
{
    // arrange
    call_callback_with_NULL_context = true;
    setup_success_async_execute();

    // act
    // done by Begin in setup_success_async_execute

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_016: [ If fabric_async_operation_context is NULL, _sync_wrapper_cb shall terminate the process. ]*/
TEST_FUNCTION(sync_wrapper_cb_with_NULL_fabric_async_operation_context_returns)
{
    // arrange
    call_callback_with_NULL_fabric_async_operation_context = true;
    setup_success_async_execute();

    // act
    // done by Begin in setup_success_async_execute

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_014: [ DEFINE_FABRIC_ASYNC_OPERATION_SYNC shall define the wrapper completion callback: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_017: [ Otherwise, _sync_wrapper_cb shall check whether the async operation has completed synchronously. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_019: [ If the async operation has not completed synchronously _sync_wrapper_cb shall signal to unblock _execute. ]*/
TEST_FUNCTION(sync_wrapper_cb_when_completed_synchronously_returns)
{
    // arrange
    int injected_operation_int_result = 43;
    double injected_operation_double_result = 1.42;
    int operation_int_result = 0;
    double operation_double_result = 0.00;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    call_sync_wrapper_callback = true;

    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG))
        .CaptureArgumentValue_user_invoke_cb(&sync_wrapper_cb)
        .CaptureArgumentValue_user_invoke_cb_context(&sync_wrapper_cb_context);
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE); // called by callback
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE);
    STRICT_EXPECTED_CALL(InterlockedHL_WaitForValue(IGNORED_ARG, IGNORED_ARG, UINT32_MAX));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &injected_operation_int_result, sizeof(injected_operation_int_result))
        .CopyOutArgumentBuffer(4, &injected_operation_double_result, sizeof(injected_operation_double_result));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));

    // act
    ASSERT_ARE_EQUAL(HRESULT, S_OK, ITestAsyncOperation_TestOperation_execute(test_async_operation_com, 42, &operation_int_result, &operation_double_result));

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
