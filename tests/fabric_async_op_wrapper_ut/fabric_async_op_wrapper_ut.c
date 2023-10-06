// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdint.h>


#include "fabriccommon.h"

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_stdint.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#define ENABLE_MOCKS

#include "c_pal/gballoc_hl.h"

#define GBALLOC_HL_REDIRECT_H
#include "sf_c_util/fabric_async_op_cb.h"
#include "com_wrapper/com_wrapper.h"
#include "sf_c_util/fabric_async_op_cb_com.h"
#include "../../src/fabric_async_op_cb_com.c"
#include "test_fabric_async_operation.h"
#include "test_fabric_async_operation_com.h"
#include "test_fabric_async_operation_com.c"
#include "test_async_operation_context.h"
#include "test_async_operation_context_com.h"
#include "test_async_operation_context_com.c"
#undef GBALLOC_HL_REDIRECT_H

#include "c_pal/gballoc_hl_redirect.h"
#undef ENABLE_MOCKS

#include "real_gballoc_hl.h"


#include "test_fabric_async_operation_wrapper.h"

static ITestAsyncOperation* test_async_operation_com;
static IFabricAsyncOperationContext* test_async_operation_context_com;
static FABRIC_ASYNC_OP_CB_HANDLE test_fabric_async_op_cb = (FABRIC_ASYNC_OP_CB_HANDLE)0x4242;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_002: [ DECLARE_FABRIC_ASYNC_OPERATION shall declare the async operation completion callback with the prototype: ]*/
MOCK_FUNCTION_WITH_CODE(, void, on_test_fabric_operation_complete, void*, context, HRESULT, async_operation_result, int, operation_result)
MOCK_FUNCTION_END()

static void setup_success_async_execute(USER_INVOKE_CB* wrapper_cb, void** wrapper_cb_context)
{
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG))
        .CaptureArgumentValue_user_invoke_cb(wrapper_cb)
        .CaptureArgumentValue_user_invoke_cb_context(wrapper_cb_context);
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE);
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context
    ASSERT_ARE_EQUAL(HRESULT, S_OK, ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242));
    umock_c_reset_all_calls();
}

static void setup_success_async_execute_completed_synchronously(USER_INVOKE_CB* wrapper_cb, void** wrapper_cb_context)
{
    int operation_int_result = 43;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG))
        .CaptureArgumentValue_user_invoke_cb(wrapper_cb)
        .CaptureArgumentValue_user_invoke_cb_context(wrapper_cb_context);
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &operation_int_result, sizeof(operation_int_result));
    STRICT_EXPECTED_CALL(on_test_fabric_operation_complete((void*)0x4242, S_OK, 43));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context
    ASSERT_ARE_EQUAL(HRESULT, S_OK, ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242));
    umock_c_reset_all_calls();
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error), "umock_c_init");

    ASSERT_ARE_EQUAL(int, 0, umocktypes_stdint_register_types(), "umocktypes_stdint_register_types");

    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(malloc, NULL);
    REGISTER_GLOBAL_MOCK_RETURNS(fabric_async_op_cb_create, test_fabric_async_op_cb, NULL);
    REGISTER_GLOBAL_MOCK_RETURN(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously, TRUE);

    REGISTER_UMOCK_ALIAS_TYPE(USER_INVOKE_CB, void*);
    REGISTER_UMOCK_ALIAS_TYPE(BOOLEAN, uint8_t);
    REGISTER_UMOCK_ALIAS_TYPE(HRESULT, long);
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

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_003: [ If com_object is NULL, _execute_async shall fail and return E_INVALIDARG. ]*/
TEST_FUNCTION(fabric_async_op_wrapper_execute_async_with_NULL_com_object_fails)
{
    // arrange
    HRESULT result;

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(NULL, 42, on_test_fabric_operation_complete, (void*)0x4242);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_004: [ If on_complete is NULL, _execute_async shall fail and return E_INVALIDARG. ]*/
TEST_FUNCTION(fabric_async_op_wrapper_execute_async_with_NULL_on_complete_fails)
{
    // arrange
    HRESULT result;

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, NULL, (void*)0x4242);

    // assert
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_001: [ DECLARE_FABRIC_ASYNC_OPERATION shall declare a function with the following prototype: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_025: [ DEFINE_FABRIC_ASYNC_OPERATION shall implement a function with the following prototype: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_006: [ _execute_async shall allocate a context used to store on_complete and on_complete_context. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_027: [ _execute_async shall increment the reference count for com_object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_007: [ _execute_async shall create a async operation callback object by calling fabric_async_op_cb_create, passing as arguments the wrapper complete callback and the context with on_complete and on_complete_context. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_008: [ A COM wrapper shall be created for the async operation callback object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_009: [ _execute_async shall call Begin{operation_name} on com_object, passing as arguments the begin arguments and the async operation callback COM object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_010: [ If fabric_operation_context has completed synchronously: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_011: [ _execute_async shall call End{operation_name} on com_object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_012: [ _execute_async shall call the on_complete and pass as arguments on_complete_context, S_OK and the end argument values obtained from End{operation_name}. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_028: [ _execute_async shall decrement the reference count for com_object. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_029: [ _execute_async shall free the context used to store on_complete and on_complete_context. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_013: [ _execute_async shall release the asynchronous operation context obtained from Begin{operation_name}. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_026: [ On success, _execute_async shall return S_OK. ]*/ \
TEST_FUNCTION(fabric_async_op_wrapper_execute_async_succeeds_when_completed_synchronously)
{
    // arrange
    HRESULT result;
    int operation_int_result = 43;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &operation_int_result, sizeof(operation_int_result));
    STRICT_EXPECTED_CALL(on_test_fabric_operation_complete((void*)0x4242, S_OK, 43));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, S_OK, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_005: [ on_complete_context shall be allowed to be NULL. ]*/
TEST_FUNCTION(fabric_async_op_wrapper_execute_async_succeeds_when_completed_synchronously_with_NULL_context)
{
    // arrange
    HRESULT result;
    int operation_int_result = 43;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &operation_int_result, sizeof(operation_int_result));
    STRICT_EXPECTED_CALL(on_test_fabric_operation_complete(NULL, S_OK, 43));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, S_OK, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_014: [ If Begin{operation_name} fails, _execute_async shall return the error returned by Begin{operation_name}. ]*/
TEST_FUNCTION(when_Begin_fails_fabric_async_op_wrapper_execute_async_also_fails)
{
    // arrange
    HRESULT result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .SetReturn(E_FAIL);
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_014: [ If Begin{operation_name} fails, _execute_async shall return the error returned by Begin{operation_name}. ]*/
TEST_FUNCTION(when_Begin_fails_fabric_async_op_wrapper_execute_async_also_fails_with_E_INVALIDARG)
{
    // arrange
    HRESULT result;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .SetReturn(E_INVALIDARG);
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_015: [ If End{operation_name} fails, _execute_async shall return the error returned by End{operation_name}. ]*/
TEST_FUNCTION(when_End_fails_fabric_async_op_wrapper_execute_async_also_fails_with_E_FAIL)
{
    // arrange
    HRESULT result;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .SetReturn(E_FAIL);
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_015: [ If End{operation_name} fails, _execute_async shall return the error returned by End{operation_name}. ]*/
TEST_FUNCTION(when_End_fails_fabric_async_op_wrapper_execute_async_also_fails_with_E_INVALIDARG)
{
    // arrange
    HRESULT result;
    (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .SetReturn(E_INVALIDARG);
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com));
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG));
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context

    // act
    result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(HRESULT, E_INVALIDARG, result);
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_016: [ If any other error occurs, _execute_async shall fail and return E_FAIL. ]*/
TEST_FUNCTION(when_underlying_calls_fail_fabric_async_op_wrapper_execute_async_fails)
{
    // arrange
    HRESULT result;
    int operation_int_result = 43;

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); // context
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_AddRef(test_async_operation_com))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(fabric_async_op_cb_create(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(COM_WRAPPER_TYPE_CREATE_FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback(test_fabric_async_op_cb, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_BeginTestOperation(test_async_operation_com, 42, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer(4, &test_async_operation_context_com, sizeof(test_async_operation_context_com))
        .CallCannotFail(); // tested with other tests
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE)
        .CallCannotFail();
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &operation_int_result, sizeof(operation_int_result))
        .CallCannotFail(); // tested with other tests
    STRICT_EXPECTED_CALL(on_test_fabric_operation_complete((void*)0x4242, S_OK, 43));
    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_Release(test_async_operation_context_com))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(FABRIC_ASYNC_OP_CB_HANDLE_IFabricAsyncOperationCallback_Release(IGNORED_ARG))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(fabric_async_op_cb_destroy(IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(free(IGNORED_ARG)); // context

    umock_c_negative_tests_snapshot();

    for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            if (i > 4)
            {
                (void)test_async_operation_context_com->lpVtbl->AddRef(test_async_operation_context_com); // returned in the Begin
            }

            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            // act
            result = ITestAsyncOperation_TestOperation_execute_async(test_async_operation_com, 42, on_test_fabric_operation_complete, (void*)0x4242);

            //assert
            ASSERT_ARE_EQUAL(HRESULT, E_FAIL, result, "On failed call %zu", i);
        }
    }
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_018: [ If context is NULL, _wrapper_cb shall return. ]*/
TEST_FUNCTION(wrapper_cb_with_NULL_context_returns)
{
    // arrange
    USER_INVOKE_CB wrapper_cb;
    void* wrapper_cb_context;
    setup_success_async_execute(&wrapper_cb, &wrapper_cb_context);

    // act
    wrapper_cb(NULL, test_async_operation_context_com);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    wrapper_cb(wrapper_cb_context, test_async_operation_context_com);
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_017: [ DEFINE_FABRIC_ASYNC_OPERATION shall define the wrapper completion callback: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_019: [ Otherwise, _wrapper_cb shall check whether the async operation has completed synchronously. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_020: [ If the async operation has completed synchronously, _wrapper_cb shall return. ]*/
TEST_FUNCTION(wrapper_cb_when_completed_synchronously_returns)
{
    // arrange
    USER_INVOKE_CB wrapper_cb;
    void* wrapper_cb_context;
    setup_success_async_execute_completed_synchronously(&wrapper_cb, &wrapper_cb_context);

    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(TRUE);

    // act
    wrapper_cb(wrapper_cb_context, test_async_operation_context_com);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_021: [ If the async operation has not completed synchronously: ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_022: [ _wrapper_cb shall call End{operation_name} on the com_object passed to _execute_async. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_023: [ _wrapper_cb shall call the on_complete and pass as arguments on_complete_context, S_OK and the end argument values obtained from End{operation_name}. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_030: [ _wrapper_cb shall release the com object passed as argument to _execute_async. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_031: [ _wrapper_cb shall free the context created in _execute_async. ]*/
TEST_FUNCTION(wrapper_cb_when_not_completed_synchronously_calls_end)
{
    // arrange
    USER_INVOKE_CB wrapper_cb;
    int operation_int_result = 44;
    void* wrapper_cb_context;
    setup_success_async_execute(&wrapper_cb, &wrapper_cb_context);

    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .CopyOutArgumentBuffer(3, &operation_int_result, sizeof(operation_int_result));
    STRICT_EXPECTED_CALL(on_test_fabric_operation_complete((void*)0x4242, S_OK, 44));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    // act
    wrapper_cb(wrapper_cb_context, test_async_operation_context_com);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_024: [ If the End{operation_name} fails, _wrapper_cb shall call the on_complete and pass as arguments on_complete_context and the result of the End{operation_name} call. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_030: [ _wrapper_cb shall release the com object passed as argument to _execute_async. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_031: [ _wrapper_cb shall free the context created in _execute_async. ]*/
TEST_FUNCTION(when_End_fails_wrapper_cb_indicates_failure)
{
    // arrange
    USER_INVOKE_CB wrapper_cb;
    void* wrapper_cb_context;
    setup_success_async_execute(&wrapper_cb, &wrapper_cb_context);

    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .SetReturn(E_FAIL);
    STRICT_EXPECTED_CALL(on_test_fabric_operation_complete((void*)0x4242, E_FAIL, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    // act
    wrapper_cb(wrapper_cb_context, test_async_operation_context_com);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_024: [ If the End{operation_name} fails, _wrapper_cb shall call the on_complete and pass as arguments on_complete_context and the result of the End{operation_name} call. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_030: [ _wrapper_cb shall release the com object passed as argument to _execute_async. ]*/
/* Tests_SRS_FABRIC_ASYNC_OP_WRAPPER_01_031: [ _wrapper_cb shall free the context created in _execute_async. ]*/
TEST_FUNCTION(when_End_fails_wrapper_cb_indicates_failure_with_INVALIDARG)
{
    // arrange
    USER_INVOKE_CB wrapper_cb;
    void* wrapper_cb_context;
    setup_success_async_execute(&wrapper_cb, &wrapper_cb_context);

    STRICT_EXPECTED_CALL(TEST_ASYNC_OPERATION_CONTEXT_HANDLE_IFabricAsyncOperationContext_test_async_operation_context_CompletedSynchronously(test_async_operation_context_com))
        .SetReturn(FALSE);
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_test_fabric_async_operation_EndTestOperation(test_async_operation_com, test_async_operation_context_com, IGNORED_ARG))
        .SetReturn(E_INVALIDARG);
    STRICT_EXPECTED_CALL(on_test_fabric_operation_complete((void*)0x4242, E_INVALIDARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(TEST_FABRIC_ASYNC_OPERATION_HANDLE_ITestAsyncOperation_Release(test_async_operation_com));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    // act
    wrapper_cb(wrapper_cb_context, test_async_operation_context_com);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
