// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FABRIC_ASYNC_OP_SYNC_WRAPPER_H
#define FABRIC_ASYNC_OP_SYNC_WRAPPER_H

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
#else
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#endif


#include "fabriccommon.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/interlocked.h"
#include "c_pal/sync.h"

#include "c_util/interlocked_hl.h"
#include "c_util/log_critical_and_terminate.h"

#include "com_wrapper/com_wrapper.h"
#include "sf_c_util/fabric_async_op_cb.h"
#include "sf_c_util/fabric_async_op_cb_com.h"
#include "sf_c_util/hresult_to_string.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

#define BS2SF_ASYNC_OP_SYNC_PASTE_ARG_IN_SIGNATURE_WITH_COMMA(arg_type, arg_name) \
    , arg_type arg_name

#define BS2SF_ASYNC_OP_SYNC_ARGS_IN_SIGNATURE_WITH_COMMA(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_SYNC_PASTE_ARG_IN_SIGNATURE_WITH_COMMA, __VA_ARGS__)

// this section is for pasting the end arg pointers in a signature
#define BS2SF_ASYNC_OP_SYNC_PASTE_ARG_POINTER_IN_SIGNATURE(arg_type, arg_name) \
    , arg_type* arg_name

#define BS2SF_ASYNC_OP_SYNC_ARGS_POINTERS_IN_SIGNATURE(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_SYNC_PASTE_ARG_POINTER_IN_SIGNATURE, __VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_POINTERS_BEGIN_ARGS(...) \

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_POINTERS_END_ARGS(...) \
    BS2SF_ASYNC_OP_SYNC_ARGS_POINTERS_IN_SIGNATURE(__VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_POINTERS_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_SYNC_EXTRACT_END_POINTERS_, a)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_POINTERS(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_POINTERS_BRIDGE, __VA_ARGS__)

// this section is for pasting the NULL compare for the end args
#define BS2SF_ASYNC_OP_SYNC_PASTE_NULL_COMPARE(arg_type, arg_name) \
    || (arg_name == NULL)

#define BS2SF_ASYNC_OP_SYNC_ARGS_NULL_COMPARE_IN_SIGNATURE(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_SYNC_PASTE_NULL_COMPARE, __VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_NULL_COMPARE_BEGIN_ARGS(...) \

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_NULL_COMPARE_END_ARGS(...) \
    BS2SF_ASYNC_OP_SYNC_ARGS_NULL_COMPARE_IN_SIGNATURE(__VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_NULL_COMPARE_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_SYNC_EXTRACT_END_NULL_COMPARE_, a)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_NULL_COMPARE(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_NULL_COMPARE_BRIDGE, __VA_ARGS__)

// this section is for pasting the begin arg types and names in a signature
#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_BEGIN_ARGS(...) \
    BS2SF_ASYNC_OP_SYNC_ARGS_IN_SIGNATURE_WITH_COMMA(__VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_END_ARGS(...) \

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARGS_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_, a)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARGS(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARGS_BRIDGE, __VA_ARGS__)

// this section is for pasting the begin arg values (names) in a call
#define BS2SF_ASYNC_OP_SYNC_PASTE_ARG_VALUE(arg_type, arg_name) \
    , arg_name

#define BS2SF_ASYNC_OP_SYNC_ARGS_AS_VALUES(...) \
    MU_FOR_EACH_2(BS2SF_ASYNC_OP_SYNC_PASTE_ARG_VALUE, __VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_VALUES_BEGIN_ARGS(...) \
    BS2SF_ASYNC_OP_SYNC_ARGS_AS_VALUES(__VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_VALUES_END_ARGS(...) \

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARG_VALUES_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_VALUES_, a)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARG_VALUES(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARG_VALUES_BRIDGE, __VA_ARGS__)

// this section is for pasting the end arg values (names) in a call
#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_VALUES_BEGIN_ARGS(...) \

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_VALUES_END_ARGS(...) \
    BS2SF_ASYNC_OP_SYNC_ARGS_AS_VALUES(__VA_ARGS__)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARG_VALUES_BRIDGE(a) \
    MU_C2A(BS2SF_ASYNC_OP_SYNC_EXTRACT_END_VALUES_, a)

#define BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARG_VALUES(...) \
    MU_FOR_EACH_1(BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARG_VALUES_BRIDGE, __VA_ARGS__)

/* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_001: [ DECLARE_FABRIC_ASYNC_OPERATION_SYNC shall declare a function with the following prototype: ]*/
#define DECLARE_FABRIC_ASYNC_OPERATION_SYNC(interface_name, operation_name, ...) \
    HRESULT MU_C4(interface_name, _, operation_name, _execute)(interface_name* com_object BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARGS(__VA_ARGS__) BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_POINTERS(__VA_ARGS__));

/* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_002: [ DEFINE_FABRIC_ASYNC_OPERATION_SYNC shall implement a function with the following prototype: ]*/
#define BS2SF_ASYNC_OP_SYNC_IMPLEMENT_EXECUTE(interface_name, operation_name, ...) \
    HRESULT MU_C4(interface_name, _, operation_name, _execute)(interface_name* com_object BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARGS(__VA_ARGS__) BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_POINTERS(__VA_ARGS__)) \
    { \
        HRESULT result; \
        /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_003: [ If com_object is NULL, _execute shall fail and return E_INVALIDARG. ]*/ \
        if  ( \
             (com_object == NULL) \
            /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_020: [ If any of the end arg pointers is NULL, _execute shall fail and return E_INVALIDARG. ]*/ \
            BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARGS_NULL_COMPARE(__VA_ARGS__) \
            ) \
        { \
            LogError("Invalid arguments: interface_name* com_object=%p, ...", com_object); \
            result = E_INVALIDARG; \
        } \
        else \
        { \
            volatile_atomic int32_t is_completed; \
            (void)interlocked_exchange(&is_completed, 0); \
            /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_004: [ _execute shall create a async operation callback object by calling fabric_async_op_cb_create, passing as arguments the wrapper complete callback. ]*/ \
            FABRIC_ASYNC_OP_CB_HANDLE fabric_async_op_cb = fabric_async_op_cb_create(MU_C4(interface_name, _, operation_name, sync_wrapper_cb), (void*)&is_completed); \
            if (fabric_async_op_cb == NULL) \
            { \
                    /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_013: [ If any other error occurs, _execute shall fail and return E_FAIL. ]*/ \
                LogError("fabric_async_op_cb_create failed"); \
                result = E_FAIL; \
            } \
            else \
            { \
                /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_005: [ A COM wrapper shall be created for the async operation callback object. ]*/ \
                IFabricAsyncOperationCallback* callback = COM_WRAPPER_CREATE(FABRIC_ASYNC_OP_CB_HANDLE, IFabricAsyncOperationCallback, fabric_async_op_cb, fabric_async_op_cb_destroy); \
                if (callback == NULL) \
                { \
                    /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_013: [ If any other error occurs, _execute shall fail and return E_FAIL. ]*/ \
                    LogError("COM wrapper object creation failed"); \
                    fabric_async_op_cb_destroy(fabric_async_op_cb); \
                    result = E_FAIL; \
                } \
                else \
                { \
                    IFabricAsyncOperationContext* fabric_operation_context; \
                    /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_006: [ _execute shall call Begin{operation_name} on com_object, passing as arguments the begin arguments and the async operation callback COM object. ]*/ \
                    result = com_object->lpVtbl->MU_C2(Begin, operation_name)(com_object BS2SF_ASYNC_OP_SYNC_EXTRACT_BEGIN_ARG_VALUES(__VA_ARGS__), callback, &fabric_operation_context); \
                    if (FAILED(result)) \
                    { \
                        /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_011: [ If Begin{operation_name} fails, _execute shall return the error returned by Begin{operation_name}. ]*/ \
                        LogHRESULTError(result, "com_object->lpVtbl->Begin" MU_TOSTRING(operation_name) " failed."); \
                        /* return result as is */ \
                    } \
                    else \
                    { \
                        result = S_OK; \
                        if (!fabric_operation_context->lpVtbl->CompletedSynchronously(fabric_operation_context)) \
                        { \
                            /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_007: [ If fabric_operation_context has not completed synchronously, _execute shall wait to be signalled by the _sync_wrapper_cb function. ]*/ \
                            if (InterlockedHL_WaitForValue(&is_completed, 1, UINT32_MAX) != INTERLOCKED_HL_OK) \
                            { \
                                /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_013: [ If any other error occurs, _execute shall fail and return E_FAIL. ]*/ \
                                LogError("InterlockedHL_WaitForValue failed"); \
                                result = E_FAIL; \
                            } \
                        } \
                        if (!FAILED(result)) \
                        { \
                            /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_008: [ _execute shall call End{operation_name} on com_object. ]*/ \
                            result = com_object->lpVtbl->MU_C2(End, operation_name)(com_object, fabric_operation_context BS2SF_ASYNC_OP_SYNC_EXTRACT_END_ARG_VALUES(__VA_ARGS__)); \
                            if (FAILED(result)) \
                            { \
                                /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_012: [ If End{operation_name} fails, _execute shall return the error returned by End{operation_name}. ]*/ \
                                LogHRESULTError(result, MU_TOSTRING(MU_C2(End, operation_name)) " failed"); \
                                /* return result as is */ \
                            } \
                            else \
                            { \
                                /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_010: [ On success, _execute shall return S_OK. ]*/ \
                                result = S_OK; \
                            } \
                        } \
                        /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_009: [ _execute shall release the asynchronous operation context obtained from Begin{operation_name}. ]*/ \
                        (void)fabric_operation_context->lpVtbl->Release(fabric_operation_context); \
                    } \
                    (void)callback->lpVtbl->Release(callback); \
                } \
            } \
        } \
        return result; \
    }

/* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_014: [ DEFINE_FABRIC_ASYNC_OPERATION_SYNC shall define the wrapper completion callback: ]*/
#define BS2SF_ASYNC_OP_SYNC_IMPLEMENT_WRAPPER_CB(interface_name, operation_name, ...) \
    static void MU_C4(interface_name, _, operation_name, sync_wrapper_cb)(void* context, IFabricAsyncOperationContext* fabric_async_operation_context) \
    { \
        if ( \
            /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_015: [ If context is NULL, _sync_wrapper_cb shall terminate the process. ]*/ \
            (context == NULL) || \
            /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_016: [ If fabric_async_operation_context is NULL, _sync_wrapper_cb shall terminate the process. ]*/ \
            (fabric_async_operation_context == NULL) \
           ) \
        { \
            LogCriticalAndTerminate("Invalid arguments: void* context=%p, IFabricAsyncOperationContext* fabric_async_operation_context=%p", context, fabric_async_operation_context); \
        } \
        else \
        { \
            /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_017: [ Otherwise, _sync_wrapper_cb shall check whether the async operation has completed synchronously. ]*/ \
            if (fabric_async_operation_context->lpVtbl->CompletedSynchronously(fabric_async_operation_context)) \
            { \
                /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_018: [ If the async operation has completed synchronously, _sync_wrapper_cb shall return. ]*/ \
            } \
            else \
            { \
                /* Codes_SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_019: [ If the async operation has not completed synchronously _sync_wrapper_cb shall signal to unblock _execute. ]*/ \
                volatile_atomic int32_t* is_completed = (volatile_atomic int32_t*)context; \
                (void)interlocked_exchange(is_completed, 1); \
                wake_by_address_single(is_completed); \
            } \
        } \
    } \

#define DEFINE_FABRIC_ASYNC_OPERATION_SYNC(interface_name, operation_name, ...) \
    BS2SF_ASYNC_OP_SYNC_IMPLEMENT_WRAPPER_CB(interface_name, operation_name, __VA_ARGS__) \
    BS2SF_ASYNC_OP_SYNC_IMPLEMENT_EXECUTE(interface_name, operation_name, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_ASYNC_OP_SYNC_WRAPPER_H */
