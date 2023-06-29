// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_ASYNC_OP_WRAPPER_H
#define FABRIC_ASYNC_OP_WRAPPER_H

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif

#include "windows.h"


#include "fabriccommon.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "com_wrapper/com_wrapper.h"
#include "sf_c_util/fabric_async_op_cb.h"
#include "sf_c_util/fabric_async_op_cb_com.h"
#include "sf_c_util/hresult_to_string.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

#define BS2SF_ASYNC_OP_PASTE_ARG_IN_SIGNATURE(arg_type, arg_name) \
    , arg_type arg_name

#define BS2SF_ASYNC_OP_PASTE_ARG_ADDRESS_IN_CALL(arg_type, arg_name) \
    , &arg_name

#define BS2SF_ASYNC_OP_ARGS_IN_SIGNATURE_WITH_COMMA(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_PASTE_ARG_IN_SIGNATURE, __VA_ARGS__)

// this section is for pasting the end arg types and names in a signature
#define BS2SF_ASYNC_OP_EXTRACT_END_BEGIN_ARGS(...) \

#define BS2SF_ASYNC_OP_EXTRACT_END_END_ARGS(...) \
    BS2SF_ASYNC_OP_ARGS_IN_SIGNATURE_WITH_COMMA(__VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_END_ARGS_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_EXTRACT_END_, a)

#define BS2SF_ASYNC_OP_EXTRACT_END_ARGS(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_EXTRACT_END_ARGS_BRIDGE, __VA_ARGS__)

// this section is for pasting the address of each end arg in a call
#define BS2SF_ASYNC_OP_ARGS_ADDRESS_IN_CALL(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_PASTE_ARG_ADDRESS_IN_CALL, __VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_BEGIN_ARGS(...) \

#define BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_END_ARGS(...) \
    BS2SF_ASYNC_OP_ARGS_ADDRESS_IN_CALL(__VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_ARGS_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_, a)

#define BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_ARG_VALUES(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_ARGS_BRIDGE, __VA_ARGS__)

// this section is for pasting variable definitions for each end arg
#define BS2SF_ASYNC_OP_PASTE_ARG_VAR_DEFINITION(arg_type, arg_name) \
    arg_type arg_name;

#define BS2SF_ASYNC_OP_ARG_VAR_DEFINITIONS(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_PASTE_ARG_VAR_DEFINITION, __VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_BEGIN_ARGS(...) \

#define BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_END_ARGS(...) \
    BS2SF_ASYNC_OP_ARG_VAR_DEFINITIONS(__VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_ARGS_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_, a)

#define BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_ARGS(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_ARGS_BRIDGE, __VA_ARGS__)

// this section is for pasting the begin arg types and names in a signature
#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_BEGIN_ARGS(...) \
    BS2SF_ASYNC_OP_ARGS_IN_SIGNATURE_WITH_COMMA(__VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_END_ARGS(...) \

#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARGS_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_EXTRACT_BEGIN_, a)

#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARGS(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARGS_BRIDGE, __VA_ARGS__)

// this section is for pasting the begin arg values (names) in a call
#define BS2SF_ASYNC_OP_PASTE_ARG_VALUE(arg_type, arg_name) \
    , arg_name

#define BS2SF_ASYNC_OP_ARGS_AS_VALUES(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_PASTE_ARG_VALUE, __VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_VALUES_BEGIN_ARGS(...) \
    BS2SF_ASYNC_OP_ARGS_AS_VALUES(__VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_VALUES_END_ARGS(...) \

#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARG_VALUES_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_EXTRACT_BEGIN_VALUES_, a)

#define BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARG_VALUES(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARG_VALUES_BRIDGE, __VA_ARGS__)

// this section is for pasting the end arg values (names) in a call
#define BS2SF_ASYNC_OP_EXTRACT_END_VALUES_BEGIN_ARGS(...) \

#define BS2SF_ASYNC_OP_EXTRACT_END_VALUES_END_ARGS(...) \
    BS2SF_ASYNC_OP_ARGS_AS_VALUES(__VA_ARGS__)

#define BS2SF_ASYNC_OP_EXTRACT_END_ARG_VALUES_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_EXTRACT_END_VALUES_, a)

#define BS2SF_ASYNC_OP_EXTRACT_END_ARG_VALUES(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_EXTRACT_END_ARG_VALUES_BRIDGE, __VA_ARGS__)

/* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_001: [ DECLARE_FABRIC_ASYNC_OPERATION shall declare a function with the following prototype: ]*/
/* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_002: [ DECLARE_FABRIC_ASYNC_OPERATION shall declare the async operation completion callback with the prototype: ]*/
#define DECLARE_FABRIC_ASYNC_OPERATION(interface_name, operation_name, ...) \
    typedef void (*MU_C4(interface_name, _, operation_name, _COMPLETE_CB))(void* context, HRESULT async_operation_result BS2SF_ASYNC_OP_EXTRACT_END_ARGS(__VA_ARGS__)); \
    HRESULT MU_C4(interface_name, _, operation_name, _execute_async)(interface_name* com_object BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARGS(__VA_ARGS__), MU_C4(interface_name, _, operation_name, _COMPLETE_CB) on_complete, void* on_complete_context);

/* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_025: [ DEFINE_FABRIC_ASYNC_OPERATION shall implement a function with the following prototype: ]*/
#define BS2SF_IMPLEMENT_EXECUTE_ASYNC(interface_name, operation_name, ...) \
    HRESULT MU_C4(interface_name, _, operation_name, _execute_async)(interface_name* com_object BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARGS(__VA_ARGS__), MU_C4(interface_name, _, operation_name, _COMPLETE_CB) on_complete, void* on_complete_context) \
    { \
        /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_005: [ on_complete_context shall be allowed to be NULL. ]*/ \
        HRESULT result; \
        if ( \
            /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_003: [ If com_object is NULL, _execute_async shall fail and return E_INVALIDARG. ]*/ \
            (com_object == NULL) || \
            /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_004: [ If on_complete is NULL, _execute_async shall fail and return E_INVALIDARG. ]*/ \
            (on_complete == NULL) \
           ) \
        { \
            LogError("Invalid arguments: interface_name* com_object=%p, ..., " MU_TOSTRING(MU_C4(interface_name, _, operation_name, _COMPLETE_CB)) " on_complete=%p, void* on_complete_context=%p", com_object, on_complete, on_complete_context); \
            result = E_INVALIDARG; \
        } \
        else \
        { \
            bool callback_expected = false; \
            /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_006: [ _execute_async shall allocate a context used to store on_complete and on_complete_context. ]*/ \
            MU_C4(interface_name, _, operation_name, _CONTEXT)* fabric_async_operation_wrapper_context = (MU_C4(interface_name, _, operation_name, _CONTEXT)*)malloc(sizeof(MU_C4(interface_name, _, operation_name, _CONTEXT))); \
            if (fabric_async_operation_wrapper_context == NULL) \
            { \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_016: [ If any other error occurs, _execute_async shall fail and return E_FAIL. ]*/ \
                LogError("malloc failed for size %zu", sizeof(MU_C4(interface_name, _, operation_name, _CONTEXT))); \
                result = E_FAIL; \
            } \
            else \
            { \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_027: [ _execute_async shall increment the reference count for com_object. ]*/ \
                (void)com_object->lpVtbl->AddRef(com_object); \
                { \
                    fabric_async_operation_wrapper_context->com_object = com_object; \
                    fabric_async_operation_wrapper_context->on_complete = on_complete; \
                    fabric_async_operation_wrapper_context->on_complete_context = on_complete_context; \
                    IFabricAsyncOperationContext* fabric_operation_context; \
                    /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_007: [ _execute_async shall create a async operation callback object by calling fabric_async_op_cb_create, passing as arguments the wrapper complete callback and the context with on_complete and on_complete_context. ]*/ \
                    FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb = fabric_async_op_cb_create(MU_C4(interface_name, _, operation_name, wrapper_cb), fabric_async_operation_wrapper_context); \
                    if (fabric_async_op_cb == NULL) \
                    { \
                        /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_016: [ If any other error occurs, _execute_async shall fail and return E_FAIL. ]*/ \
                        LogError("fabric_async_op_cb_create failed"); \
                        result = E_FAIL; \
                    } \
                    else \
                    { \
                        /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_008: [ A COM wrapper shall be created for the async operation callback object. ]*/ \
                        IFabricAsyncOperationCallback* callback = COM_WRAPPER_CREATE(FABRIC_ASYNC_OP_CB_HANDLE, IFabricAsyncOperationCallback, fabric_async_op_cb, fabric_async_op_cb_destroy); \
                        if (callback == NULL) \
                        { \
                            /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_016: [ If any other error occurs, _execute_async shall fail and return E_FAIL. ]*/ \
                            LogError("COM wrapper object creation failed"); \
                            fabric_async_op_cb_destroy(fabric_async_op_cb); \
                            result = E_FAIL; \
                        } \
                        else \
                        { \
                            /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_009: [ _execute_async shall call Begin{operation_name} on com_object, passing as arguments the begin arguments and the async operation callback COM object. ]*/ \
                            result = com_object->lpVtbl->MU_C2(Begin, operation_name)(com_object BS2SF_ASYNC_OP_EXTRACT_BEGIN_ARG_VALUES(__VA_ARGS__), callback, &fabric_operation_context); \
                            if (FAILED(result)) \
                            { \
                                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_014: [ If Begin{operation_name} fails, _execute_async shall return the error returned by Begin{operation_name}. ]*/ \
                                LogHRESULTError(result, "com_object->lpVtbl->Begin" MU_TOSTRING(operation_name) " failed."); \
                                /* return result as is */ \
                            } \
                            else \
                            { \
                                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_010: [ If fabric_operation_context has completed synchronously: ]*/ \
                                if (fabric_operation_context->lpVtbl->CompletedSynchronously(fabric_operation_context)) \
                                { \
                                    /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_011: [ _execute_async shall call End{operation_name} on com_object. ]*/ \
                                    BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_ARGS(__VA_ARGS__) \
                                    result = com_object->lpVtbl->MU_C2(End, operation_name)(com_object, fabric_operation_context BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_ARG_VALUES(__VA_ARGS__)); \
                                    if (FAILED(result)) \
                                    { \
                                        /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_015: [ If End{operation_name} fails, _execute_async shall return the error returned by End{operation_name}. ]*/ \
                                        LogHRESULTError(result, MU_TOSTRING(MU_C2(End, operation_name)) " failed"); \
                                        /* return result as is */ \
                                    } \
                                    else \
                                    { \
                                        /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_012: [ _execute_async shall call the on_complete and pass as arguments on_complete_context, S_OK and the end argument values obtained from End{operation_name}. ]*/ \
                                        on_complete(on_complete_context, S_OK BS2SF_ASYNC_OP_EXTRACT_END_ARG_VALUES(__VA_ARGS__)); \
                                        /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_026: [ On success, _execute_async shall return S_OK. ]*/ \
                                        result = S_OK; \
                                    } \
                                } \
                                else \
                                { \
                                    callback_expected = true; \
                                    /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_026: [ On success, _execute_async shall return S_OK. ]*/ \
                                    result = S_OK; \
                                } \
                                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_013: [ _execute_async shall release the asynchronous operation context obtained from Begin{operation_name}. ]*/ \
                                (void)fabric_operation_context->lpVtbl->Release(fabric_operation_context); \
                            } \
                            (void)callback->lpVtbl->Release(callback); \
                        } \
                    } \
                    if (!callback_expected) \
                    { \
                        /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_028: [ _execute_async shall decrement the reference count for com_object. ]*/ \
                        (void)com_object->lpVtbl->Release(com_object); \
                    } \
                } \
                if (!callback_expected) \
                { \
                    /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_029: [ _execute_async shall free the context used to store on_complete and on_complete_context. ]*/ \
                    free(fabric_async_operation_wrapper_context); \
                } \
            } \
        } \
        return result; \
    }

/* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_017: [ DEFINE_FABRIC_ASYNC_OPERATION shall define the wrapper completion callback: ]*/ \
#define BS2SF_IMPLEMENT_WRAPPER_CB(interface_name, operation_name, ...) \
    static void MU_C4(interface_name, _, operation_name, wrapper_cb)(void* context, IFabricAsyncOperationContext* fabric_async_operation_context) \
    { \
        if (context == NULL) \
        { \
            /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_018: [ If context is NULL, _wrapper_cb shall return. ]*/ \
            LogError("Invalid arguments: void* context=%p, IFabricAsyncOperationContext* fabric_async_operation_context=%p", context, fabric_async_operation_context); \
        } \
        else \
        { \
            /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_019: [ Otherwise, _wrapper_cb shall check whether the async operation has completed synchronously. ]*/ \
            if (fabric_async_operation_context->lpVtbl->CompletedSynchronously(fabric_async_operation_context)) \
            { \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_020: [ If the async operation has completed synchronously, _wrapper_cb shall return. ]*/ \
            } \
            else \
            { \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_021: [ If the async operation has not completed synchronously: ]*/ \
                BS2SF_ASYNC_OP_EXTRACT_VARS_FOR_END_ARGS(__VA_ARGS__) \
                MU_C4(interface_name, _, operation_name, _CONTEXT)* fabric_async_operation_wrapper_context = (MU_C4(interface_name, _, operation_name, _CONTEXT)*)context; \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_022: [ _wrapper_cb shall call End{operation_name} on the com_object passed to _execute_async. ]*/ \
                HRESULT hr = fabric_async_operation_wrapper_context->com_object->lpVtbl->MU_C2(End, operation_name)(fabric_async_operation_wrapper_context->com_object, fabric_async_operation_context BS2SF_ASYNC_OP_EXTRACT_ADDRESS_END_ARG_VALUES(__VA_ARGS__)); \
                if (FAILED(hr)) \
                { \
                    /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_024: [ If the End{operation_name} fails, _wrapper_cb shall call the on_complete and pass as arguments on_complete_context and the result of the End{operation_name} call. ]*/ \
                    LogHRESULTError(hr, MU_TOSTRING(MU_C2(End, operation_name)) " failed"); \
                    /* return result as is */ \
                } \
                else \
                { \
                    hr = S_OK; \
                } \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_023: [ _wrapper_cb shall call the on_complete and pass as arguments on_complete_context, S_OK and the end argument values obtained from End{operation_name}. ]*/ \
                fabric_async_operation_wrapper_context->on_complete(fabric_async_operation_wrapper_context->on_complete_context, hr BS2SF_ASYNC_OP_EXTRACT_END_ARG_VALUES(__VA_ARGS__)); \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_030: [ _wrapper_cb shall release the com object passed as argument to _execute_async. ]*/ \
                (void)fabric_async_operation_wrapper_context->com_object->lpVtbl->Release(fabric_async_operation_wrapper_context->com_object); \
                /* Codes_SRS_FABRIC_ASYNC_OP_WRAPPER_01_031: [ _wrapper_cb shall free the context created in _execute_async. ]*/ \
                free(context); \
            } \
        } \
    } \

#define DEFINE_FABRIC_ASYNC_OPERATION(interface_name, operation_name, ...) \
    typedef struct MU_C4(interface_name, _, operation_name, _CONTEXT_TAG) \
    { \
        MU_C4(interface_name, _, operation_name, _COMPLETE_CB) on_complete; \
        void* on_complete_context; \
        interface_name* com_object; \
    } MU_C4(interface_name, _, operation_name, _CONTEXT); \
    BS2SF_IMPLEMENT_WRAPPER_CB(interface_name, operation_name, __VA_ARGS__) \
    BS2SF_IMPLEMENT_EXECUTE_ASYNC(interface_name, operation_name, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_ASYNC_OP_WRAPPER_H */
