// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef H_FABRIC_MACRO_GENERATOR_H
#define H_FABRIC_MACRO_GENERATOR_H

#ifdef __cplusplus
#include <cinttypes>
#include <cstdlib>
#include <cstddef>
#else
#include <inttypes.h>
#include <stdlib.h>
#include <stddef.h>
#endif

#include "windows.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/timer.h"
#include "c_pal/threadapi.h"

#include "sf_c_util/hresult_to_string.h"
#include "sf_macros.h"

#include "umock_c/umock_c_prod.h"
/*this is prefix that is added to all data types and all APIs that are generated with this macro-based generator*/
/*this is needed to avoid clashes in the C global namespace with existing functions, such as (existing) GetNodeList from ifabricqueryclient10sync.cpp*/
#define H_FABRIC_PREFIX H /*H comes from "HANDLE"*/

/*the below macro introduces the name of an API of hfabric. The name is composed from the method name of the underlying IFabricInterface*/
/*for example, if the IFabric method is called GetNodeList then the macro will expands to HGetNodeList (H comes from HANDLE).
Users should always use H_FABRIC_API(something) when calling/mocking etc*/
#define H_FABRIC_API(IFABRIC_METHOD_NAME) MU_C2(H_FABRIC_PREFIX, IFABRIC_METHOD_NAME)

/*the below macro introduces the name of the handle with retries. The name is always based on the IFabric interface that it wraps.*/
/*this is an example: IFabricQueryClient10 -> HIFabricQueryClient10, users should only use H_FABRIC_HANDLE macro*/
#define H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) MU_C2(H_FABRIC_PREFIX, IFABRIC_INTERFACE_NAME)

/*this macro introduces a name for function that is used to create the client*/
/*the function name should be used through the macro*/
#define H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)   MU_C3(H_FABRIC_PREFIX, IFABRIC_INTERFACE_NAME, _create)

/*this macro introduces a name for function that is used to destroy a client*/
/*the function name should be used through the macro*/
#define H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)   MU_C3(H_FABRIC_PREFIX, IFABRIC_INTERFACE_NAME, _destroy)

/*this macro introduces a name for the structure underneath the H_FABRIC_HANDLE*/
#define H_FABRIC_HANDLE_STRUCT_TYPE(IFABRIC_INTERFACE_NAME) struct MU_C2(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), _HANDLE_DATA_TAG)

/*this macro introduces the structure of the handle*/
#define H_FABRIC_DEFINE_STRUCT_TYPE(IFABRIC_INTERFACE_NAME)     \
H_FABRIC_HANDLE_STRUCT_TYPE(IFABRIC_INTERFACE_NAME)             \
{                                                               \
    IFABRIC_INTERFACE_NAME* This;                               \
    uint32_t msBetweenRetries;                                  \
    uint32_t nMaxRetries;                                       \
};                                                              \

/*this macro introduces the HANDLE typedef*/
#define H_FABRIC_DEFINE_TYPEDEF(IFABRIC_INTERFACE_NAME) \
    typedef H_FABRIC_HANDLE_STRUCT_TYPE(IFABRIC_INTERFACE_NAME)* H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME);

#if _MSC_VER<1920 /*VS 2017*/

/*the following below macros (ARGS_MOCKABLE_FUNCTION_DECLARATION, ARGS_MOCKABLE_FUNCTION_IN_ARGS, ARGS_MOCKABLE_FUNCTION_PROXY, ARGS_MOCKABLE_FUNCTION_ARG) deal with taking a IN_ARGS(ARG(type1, name1), ARG(type2, name2)
* and transforming it to , type1, name1, type2, name2... which is something that can be used in a MOCKABLE_FUNCTION argument list.*/

/*expands to something that can be placed in a MOCKABLE_FUNCTION argument list*/
#define ARGS_MOCKABLE_FUNCTION_ARG(arg_type, arg_name) , arg_type, arg_name

#define ARGS_MOCKABLE_FUNCTION_PROXY(arg) MU_C2(ARGS_MOCKABLE_FUNCTION_, arg)

/*... has the form ARG(type1, name1), ARG(type2, name2),...*/
#define ARGS_MOCKABLE_FUNCTION_IN_ARGS(...) \
    MU_FOR_EACH_1(ARGS_MOCKABLE_FUNCTION_PROXY, __VA_ARGS__)

/*in_args has the form IN_ARGS(ARG(type1, name1), ARG(type2, name2)...)*/
/*this macro will chain to ARGS_MOCKABLE_FUNCTION_IN_ARGS (if in_args has the form above) or will error if in_args has any other form*/
#define ARGS_MOCKABLE_FUNCTION_DECLARATION(in_args) \
    MU_C2A(ARGS_MOCKABLE_FUNCTION_, in_args)



/*the following below macros (ARGS_C_DECLARATION, ARGS_C_DECLARATION_IN_ARGS, ARGS_C_DECLARATION_PROXY, ARGS_C_DECLARATION_ARG) deal with taking a IN_ARGS(ARG(type1, name1), ARG(type2, name2)
* and transforming it to , name1 name2... which is something that can be used in a C function declaration.*/

/*expands to something that can be placed in a C function declaration*/
#define ARGS_C_DECLARATION_ARG(arg_type, arg_name) , arg_type arg_name

/*arg has the form ARG(type, name)*/
#define ARGS_C_DECLARATION_PROXY(arg) MU_C2(ARGS_C_DECLARATION_, arg)

/*... has the form ARG(type1, name1), ARG(type2, name2),...*/
#define ARGS_C_DECLARATION_IN_ARGS(...) \
    MU_FOR_EACH_1(ARGS_C_DECLARATION_PROXY, __VA_ARGS__)

/*in_args has the form IN_ARGS(ARG(type1, name1), ARG(type2, name2)...)*/
/*this macro will chain to ARGS_C_DECLARATION_IN_ARGS (if in_args has the form above) or will error if in_args has any other form*/
#define ARGS_C_DECLARATION(in_args) \
    MU_C2(ARGS_C_DECLARATION_, in_args)


/*the following below macros (ARGS_C_CALL, ARGS_C_CALL_IN_ARGS, ARGS_C_CALL_ARG_PROXY, ARGS_C_CALL_ARG) deal with taking a IN_ARGS(ARG(type1, name1), ARG(type2, name2),...)
* and transforming it to , name1, name2... which is something that can be used in a C function call*/
/*expands to something that can be placed in a C function call*/
#define ARGS_C_CALL_ARG(arg_type, arg_name) , arg_name

/*arg has the form ARG(type, name)*/
#define ARGS_C_CALL_ARG_PROXY(arg) MU_C2(ARGS_C_CALL_, arg)

/*... has the form ARG(type1, name1), ARG(type2, name2),...*/
#define ARGS_C_CALL_IN_ARGS(...) \
    MU_FOR_EACH_1(ARGS_C_CALL_ARG_PROXY, __VA_ARGS__)

/*in_args has the form IN_ARGS(ARG(type1, name1), ARG(type2, name2)...)*/
/*this macro will chain to ARGS_C_CALL_IN_ARGS (if in_args has the form above) or will error if in_args has any other form*/
#define ARGS_C_CALL(in_args) \
    MU_C2(ARGS_C_CALL_, in_args)

/*expands to a conditional (to be prepended by a false value)*/
#define RESULT_CHECK(result) || (hr == result)

/*... has the form value1, value2...*/
#define RESULTS_CHECK_RESULTS(...) \
    MU_FOR_EACH_1(RESULT_CHECK, __VA_ARGS__)
#define RESULTS_CHECK_NONE(...)

/*results has the form RESULTS(value1, value2...)*/
#define RESULTS_CHECK(results) \
    MU_C2(RESULTS_CHECK_, results)

#else /*VS 2019*/

/*the following below macros (ARGS_MOCKABLE_FUNCTION_DECLARATION, ARGS_MOCKABLE_FUNCTION_IN_ARGS, ARGS_MOCKABLE_FUNCTION_PROXY, ARGS_MOCKABLE_FUNCTION_ARG) deal with taking a IN_ARGS(ARG(type1, name1), ARG(type2, name2)
* and transforming it to , type1, name1, type2, name2... which is something that can be used in a MOCKABLE_FUNCTION argument list.*/

/*expands to something that can be placed in a MOCKABLE_FUNCTION argument list*/
#define ARGS_MOCKABLE_FUNCTION_ARG(arg_type, arg_name) , arg_type, arg_name

#define ARGS_MOCKABLE_FUNCTION_PROXY(arg) MU_EXPAND(MU_NOEXPAND(MU_C2(ARGS_MOCKABLE_FUNCTION_, arg)))

/*... has the form ARG(type1, name1), ARG(type2, name2),...*/
#define ARGS_MOCKABLE_FUNCTION_IN_ARGS(...) \
    MU_FOR_EACH_1(ARGS_MOCKABLE_FUNCTION_PROXY, __VA_ARGS__)

/*in_args has the form IN_ARGS(ARG(type1, name1), ARG(type2, name2)...)*/
/*this macro will chain to ARGS_MOCKABLE_FUNCTION_IN_ARGS (if in_args has the form above) or will error if in_args has any other form*/
#define ARGS_MOCKABLE_FUNCTION_DECLARATION(in_args) \
    MU_C2A(ARGS_MOCKABLE_FUNCTION_, in_args)


/*the following below macros (ARGS_C_DECLARATION, ARGS_C_DECLARATION_IN_ARGS, ARGS_C_DECLARATION_PROXY, ARGS_C_DECLARATION_ARG) deal with taking a IN_ARGS(ARG(type1, name1), ARG(type2, name2)
* and transforming it to , name1 name2... which is something that can be used in a C function declaration.*/

/*expands to something that can be placed in a C function declaration*/
#define ARGS_C_DECLARATION_ARG(arg_type, arg_name) , arg_type arg_name

/*arg has the form ARG(type, name)*/
#define ARGS_C_DECLARATION_PROXY(arg) MU_C2(ARGS_C_DECLARATION_, arg)

/*... has the form ARG(type1, name1), ARG(type2, name2),...*/
#define ARGS_C_DECLARATION_IN_ARGS(...) \
    MU_FOR_EACH_1(ARGS_C_DECLARATION_PROXY, __VA_ARGS__)

/*in_args has the form IN_ARGS(ARG(type1, name1), ARG(type2, name2)...)*/
/*this macro will chain to ARGS_C_DECLARATION_IN_ARGS (if in_args has the form above) or will error if in_args has any other form*/
#define ARGS_C_DECLARATION(in_args) \
    MU_C2A(ARGS_C_DECLARATION_, in_args)


/*the following below macros (ARGS_C_CALL, ARGS_C_CALL_IN_ARGS, ARGS_C_CALL_ARG_PROXY, ARGS_C_CALL_ARG) deal with taking a IN_ARGS(ARG(type1, name1), ARG(type2, name2)
* and transforming it to , name1, name2... which is something that can be used in a C function call*/
/*expands to something that can be placed in a C function call*/
#define ARGS_C_CALL_ARG(arg_type, arg_name) , arg_name

/*arg has the form ARG(type, name)*/
#define ARGS_C_CALL_ARG_PROXY(arg) MU_C2(ARGS_C_CALL_, arg)

/*... has the form ARG(type1, name1), ARG(type2, name2),...*/
#define ARGS_C_CALL_IN_ARGS(...) \
    MU_FOR_EACH_1(ARGS_C_CALL_ARG_PROXY, __VA_ARGS__)

/*in_args has the form IN_ARGS(ARG(type1, name1), ARG(type2, name2)...)*/
/*this macro will chain to ARGS_C_CALL_IN_ARGS (if in_args has the form above) or will error if in_args has any other form*/
#define ARGS_C_CALL(in_args) \
    MU_C2A(ARGS_C_CALL_, in_args)

/*expands to a conditional (to be prepended by a false value)*/
#define RESULT_CHECK(result) || (hr == result)

/*... has the form value1, value2...*/
#define RESULTS_CHECK_RESULTS(...) \
    MU_FOR_EACH_1(RESULT_CHECK, __VA_ARGS__)
#define RESULTS_CHECK_NONE(...)

/*results has the form RESULTS(value1, value2...)*/
#define RESULTS_CHECK(results) \
    MU_C2A(RESULTS_CHECK_, results)

#endif /*VS 2017 or VS2019*/


/*the below macro, in_args is a single preprocessor token in the form IN_ARGS(ARG(type1, name1), ARG(type2, name2),...)*/
#define H_FABRIC_DECLARE_API(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args) \
    MOCKABLE_FUNCTION(, HRESULT, H_FABRIC_API(IFABRIC_METHOD_NAME), H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), handle ARGS_MOCKABLE_FUNCTION_DECLARATION(in_args))

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries and timeout.*/
#define H_FABRIC_DEFINE_API_WITH_RESULTS(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args, permanent_failures)                                                                          \
HRESULT H_FABRIC_API(IFABRIC_METHOD_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle                                                                                                    \
    ARGS_C_DECLARATION(in_args)                                                                                                                                                             \
)                                                                                                                                                                                           \
{                                                                                                                                                                                           \
    HRESULT hr; /*also used a result*/                                                                                                                                                      \
    /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_007: [ If handle is NULL then H_FABRIC_API(IFABRIC_METHOD_NAME) shall fail and return NULL. ]*/                                                 \
    if (handle == NULL)                                                                                                                                                                     \
    {                                                                                                                                                                                       \
        LogError("invalid " MU_TOSTRING(HANDLE_TYPE) " handle=%p", handle);                                                                                                                 \
        hr = E_POINTER;                                                                                                                                                                     \
    }                                                                                                                                                                                       \
    else                                                                                                                                                                                    \
    {                                                                                                                                                                                       \
        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_008: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall record the start time of the request by calling timer_global_get_elapsed_ms. ]*/             \
        double startTime = timer_global_get_elapsed_ms(); /*time spend here cannot exceed timeoutMilliseconds*/                                                                             \
        double elapsed;                                                                                                                                                                     \
        uint32_t retries = 0; /*incremented at every API call - 0 means "it will call once", "1" means it will call 2 times". It is in the name: **re**tries.*/                             \
                                                                                                                                                                                            \
        do                                                                                                                                                                                  \
        {                                                                                                                                                                                   \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_009: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall call IFABRIC_METHOD_NAME on the instance of IFABRIC_INTERFACE_NAME. ]*/                  \
            hr = IFABRIC_METHOD_NAME(handle->This ARGS_C_CALL(in_args));                                                                                                                    \
            if (FAILED(hr))                                                                                                                                                                 \
            {                                                                                                                                                                               \
                LogHRESULTError(hr, "failure in " MU_TOSTRING(IFABRIC_METHOD_NAME) "(handle->This=%p, ...)",                                                                                \
                    handle->This);                                                                                                                                                          \
                                                                                                                                                                                            \
                /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_011: [ If the result is FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE, FABRIC_E_TIMEOUT or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/ \
                /* FABRIC_E_TIMEOUT was observed in certain cases, such as RestartPartition. We should retry by creating a new client and trying the call again, */                         \
                /* up to the timeout specified in the H_FABRIC_HANDLE. */                                                                                                                   \
                if ((hr == E_ABORT) || (hr == FABRIC_E_OBJECT_CLOSED) || (hr == FABRIC_E_GATEWAY_NOT_REACHABLE) || (hr == FABRIC_E_TIMEOUT))                                                \
                {                                                                                                                                                                           \
                    IFABRIC_INTERFACE_NAME* newInstance = NULL;                                                                                                                             \
                                                                                                                                                                                            \
                    HRESULT hr_create;                                                                                                                                                      \
                    hr_create = CREATE_IFABRICINSTANCE_NAME(IFABRIC_INTERFACE_NAME)(&newInstance);                                                                                          \
                    if (FAILED(hr_create))                                                                                                                                                  \
                    {                                                                                                                                                                       \
                        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_012: [ If creating the new instance of IFABRIC_INTERFACE_NAME fails then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry using the existing IFABRIC_INTERFACE_NAME. ]*/ \
                        LogHRESULTError(hr_create, "failure in CREATE_IFABRICINSTANCE_NAME(" MU_TOSTRING(IFABRIC_INTERFACE_NAME) ")(&newInstance=%p)", &newInstance);                       \
                        /*keep retrying until timeout*/                                                                                                                                     \
                    }                                                                                                                                                                       \
                    else                                                                                                                                                                    \
                    {                                                                                                                                                                       \
                        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_013: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/ \
                        /*let go of the old instance*/                                                                                                                                      \
                        (void)handle->This->lpVtbl->Release(handle->This);                                                                                                                  \
                                                                                                                                                                                            \
                        /*replace it with the new instance*/                                                                                                                                \
                        handle->This = newInstance;                                                                                                                                         \
                    }                                                                                                                                                                       \
                }                                                                                                                                                                           \
                else if (0 RESULTS_CHECK(permanent_failures))                                                                                                                               \
                {                                                                                                                                                                           \
                    /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_42_001: [ If the result is any value from permanent_failures then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return. ]*/              \
                    break;                                                                                                                                                                  \
                }                                                                                                                                                                           \
                else                                                                                                                                                                        \
                {                                                                                                                                                                           \
                    /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_014: [ If the result is any other value except FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry. ]*/ \
                    /*some other HRESULT, so retry without re-creating the object*/                                                                                                         \
                }                                                                                                                                                                           \
            }                                                                                                                                                                               \
            else                                                                                                                                                                            \
            {                                                                                                                                                                               \
                /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_010: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/                                    \
                break;                                                                                                                                                                      \
            }                                                                                                                                                                               \
                                                                                                                                                                                            \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_015: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/                                                              \
            ThreadAPI_Sleep(handle->msBetweenRetries);                                                                                                                                      \
            retries++;                                                                                                                                                                      \
            elapsed = timer_global_get_elapsed_ms() - startTime;                                                                                                                            \
                                                                                                                                                                                            \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_016: [ If the total time spend retrying exceeds timeoutMilliseconds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/ \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_017: [ If the total number of retries exceeds nMaxRetries then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/  \
        } while ((retries < handle->nMaxRetries) && (elapsed < timeoutMilliseconds));                                                                                                       \
                                                                                                                                                                                            \
        if(FAILED(hr))                                                                                                                                                                      \
        {                                                                                                                                                                                   \
            LogHRESULTError(hr, "tried for %" PRIu32 " times in %" PRIu32 "[ms] and it failed", retries, timeoutMilliseconds);                                                              \
        }                                                                                                                                                                                   \
    }                                                                                                                                                                                       \
                                                                                                                                                                                            \
    return hr;                                                                                                                                                                              \
}

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries and timeout.*/
#define H_FABRIC_DEFINE_API(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args)                                                                                                           \
    H_FABRIC_DEFINE_API_WITH_RESULTS(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args, NONE())

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries.*/
#define H_FABRIC_DEFINE_API_NO_SF_TIMEOUT_WITH_RESULTS(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args, permanent_failures)                                                            \
HRESULT H_FABRIC_API(IFABRIC_METHOD_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle                                                                                                    \
    ARGS_C_DECLARATION(in_args)                                                                                                                                                             \
)                                                                                                                                                                                           \
{                                                                                                                                                                                           \
    HRESULT hr; /*also used a result*/                                                                                                                                                      \
    /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_001: [ If handle is NULL then H_FABRIC_API(IFABRIC_METHOD_NAME) shall fail and return NULL. ]*/                                                 \
    if (handle == NULL)                                                                                                                                                                     \
    {                                                                                                                                                                                       \
        LogError("invalid " MU_TOSTRING(HANDLE_TYPE) " handle=%p", handle);                                                                                                                 \
        hr = E_POINTER;                                                                                                                                                                     \
    }                                                                                                                                                                                       \
    else                                                                                                                                                                                    \
    {                                                                                                                                                                                       \
        uint32_t retries = 0; /*incremented at every API call - 0 means "it will call once", "1" means it will call 2 times". It is in the name: **re**tries.*/                             \
                                                                                                                                                                                            \
        do                                                                                                                                                                                  \
        {                                                                                                                                                                                   \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_002: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall call IFABRIC_METHOD_NAME on the instance of IFABRIC_INTERFACE_NAME. ]*/                  \
            hr = IFABRIC_METHOD_NAME(handle->This ARGS_C_CALL(in_args));                                                                                                                    \
            if (FAILED(hr))                                                                                                                                                                 \
            {                                                                                                                                                                               \
                LogHRESULTError(hr, "failure in " MU_TOSTRING(IFABRIC_METHOD_NAME) "(handle->This=%p, ...)",                                                                                \
                    handle->This);                                                                                                                                                          \
                                                                                                                                                                                            \
                /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_004: [ If the result is FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME. ]*/ \
                if ((hr == E_ABORT) || (hr == FABRIC_E_OBJECT_CLOSED) || (hr == FABRIC_E_GATEWAY_NOT_REACHABLE))                                                                            \
                {                                                                                                                                                                           \
                    IFABRIC_INTERFACE_NAME* newInstance = NULL;                                                                                                                             \
                                                                                                                                                                                            \
                    HRESULT hr_create;                                                                                                                                                      \
                    hr_create = CREATE_IFABRICINSTANCE_NAME(IFABRIC_INTERFACE_NAME)(&newInstance);                                                                                          \
                    if (FAILED(hr_create))                                                                                                                                                  \
                    {                                                                                                                                                                       \
                        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_005: [ If creating the new instance of IFABRIC_INTERFACE_NAME fails then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry using the existing IFABRIC_INTERFACE_NAME. ]*/ \
                        LogHRESULTError(hr_create, "failure in CREATE_IFABRICINSTANCE_NAME(" MU_TOSTRING(IFABRIC_INTERFACE_NAME) ")(&newInstance=%p)", &newInstance);                       \
                        /*keep retrying until retry count is exceeded*/                                                                                                                                     \
                    }                                                                                                                                                                       \
                    else                                                                                                                                                                    \
                    {                                                                                                                                                                       \
                        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_006: [ Otherwise H_FABRIC_API(IFABRIC_METHOD_NAME) shall release the existing instance and shall retry using new newly created IFABRIC_INTERFACE_NAME. ]*/ \
                        /*let go of the old instance*/                                                                                                                                      \
                        (void)handle->This->lpVtbl->Release(handle->This);                                                                                                                  \
                                                                                                                                                                                            \
                        /*replace it with the new instance*/                                                                                                                                \
                        handle->This = newInstance;                                                                                                                                         \
                    }                                                                                                                                                                       \
                }                                                                                                                                                                           \
                else if (0 RESULTS_CHECK(permanent_failures))                                                                                                                               \
                {                                                                                                                                                                           \
                    /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_42_002: [ If the result is any value from permanent_failures then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return. ]*/              \
                    break;                                                                                                                                                                  \
                }                                                                                                                                                                           \
                else                                                                                                                                                                        \
                {                                                                                                                                                                           \
                    /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_007: [ If the result is any other value except FABRIC_E_OBJECT_CLOSED, FABRIC_E_GATEWAY_NOT_REACHABLE or E_ABORT then H_FABRIC_API(IFABRIC_METHOD_NAME) shall retry. ]*/ \
                    /*some other HRESULT, so retry without re-creating the object*/                                                                                                         \
                }                                                                                                                                                                           \
            }                                                                                                                                                                               \
            else                                                                                                                                                                            \
            {                                                                                                                                                                               \
                /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_003: [ If the call succeeds then H_FABRIC_API(IFABRIC_METHOD_NAME) shall succeed and return. ]*/                                    \
                break;                                                                                                                                                                      \
            }                                                                                                                                                                               \
                                                                                                                                                                                            \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_008: [ H_FABRIC_API(IFABRIC_METHOD_NAME) shall sleep msBetweenRetries. ]*/                                                              \
            ThreadAPI_Sleep(handle->msBetweenRetries);                                                                                                                                      \
            retries++;                                                                                                                                                                      \
                                                                                                                                                                                            \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_01_009: [ If the total number of retries exceeds nMaxRetries then H_FABRIC_API(IFABRIC_METHOD_NAME) shall return the last error code. ]*/  \
        } while (retries < handle->nMaxRetries);                                                                                                                                            \
                                                                                                                                                                                            \
        if(FAILED(hr))                                                                                                                                                                      \
        {                                                                                                                                                                                   \
            LogHRESULTError(hr, "tried for %" PRIu32 " times and it failed", retries);                                                                                                      \
        }                                                                                                                                                                                   \
    }                                                                                                                                                                                       \
                                                                                                                                                                                            \
    return hr;                                                                                                                                                                              \
}

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries.*/
#define H_FABRIC_DEFINE_API_NO_SF_TIMEOUT(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args)                                                                                             \
    H_FABRIC_DEFINE_API_NO_SF_TIMEOUT_WITH_RESULTS(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args, NONE())

/*this macro introduces the declaration for _create for the IFabric type wrapper*/                                                                                                          \
#define H_FABRIC_DECLARE_CREATE(IFABRIC_INTERFACE_NAME)                                                                                                                                     \
    MOCKABLE_FUNCTION(, H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME), uint32_t, nMaxRetries, uint32_t, msBetweenRetries);                        \

/*this macro introduces the definition for _create of for the IFabric type wrapper*/
#define H_FABRIC_DEFINE_CREATE(IFABRIC_INTERFACE_NAME)                                                                                                                                      \
H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)(uint32_t nMaxRetries, uint32_t msBetweenRetries)                                                     \
{                                                                                                                                                                                           \
    H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) result;                                                                                                                                         \
    /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_001: [ H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall allocate memory to hold nMaxRetries, msBetweenRetries and an instance of IFABRIC_INTERFACE_NAME. ]*/ \
    result = malloc(sizeof(H_FABRIC_HANDLE_STRUCT_TYPE(IFABRIC_INTERFACE_NAME)));                                                                                                           \
    if (result == NULL)                                                                                                                                                                     \
    {                                                                                                                                                                                       \
        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_003: [ If there are any failures then H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall fail and return NULL. ]*/                        \
        LogError("failure in malloc(sizeof(" MU_TOSTRING(H_FABRIC_HANDLE_STRUCT_TYPE(IFABRIC_INTERFACE_NAME)) ")=%zu)", sizeof(H_FABRIC_HANDLE_STRUCT_TYPE(IFABRIC_INTERFACE_NAME)));       \
        /*return as is*/                                                                                                                                                                    \
    }                                                                                                                                                                                       \
    else                                                                                                                                                                                    \
    {                                                                                                                                                                                       \
        result->This = NULL;                                                                                                                                                                \
        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_002: [ H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall create a new instance of IFABRIC_INTERFACE_NAME by calling CREATE_IFABRICINSTANCE_NAME(IFABRIC_INTERFACE_NAME). ]*/ \
        HRESULT hr = CREATE_IFABRICINSTANCE_NAME(IFABRIC_INTERFACE_NAME)(&result->This);                                                                                                    \
        if (FAILED(hr))                                                                                                                                                                     \
        {                                                                                                                                                                                   \
            /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_003: [ If there are any failures then H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME) shall fail and return NULL. ]*/                    \
            LogHRESULTError(hr, "failure in CREATE_IFABRICINSTANCE_NAME(" MU_TOSTRING(IFABRIC_INTERFACE_NAME) ")(&result->This=%p))", &result->This);                                       \
        }                                                                                                                                                                                   \
        else                                                                                                                                                                                \
        {                                                                                                                                                                                   \
            result->nMaxRetries = nMaxRetries;                                                                                                                                              \
            result->msBetweenRetries = msBetweenRetries;                                                                                                                                    \
            goto allok;                                                                                                                                                                     \
        }                                                                                                                                                                                   \
        free(result);                                                                                                                                                                       \
        result = NULL;                                                                                                                                                                      \
    }                                                                                                                                                                                       \
allok:                                                                                                                                                                                      \
    return result;                                                                                                                                                                          \
}                                                                                                                                                                                           \


/*this macro introduces the declaration for _destroy for the IFabric type wrapper*/
#define H_FABRIC_DECLARE_DESTROY(IFABRIC_INTERFACE_NAME)                                                                                                                                    \
    MOCKABLE_FUNCTION(, void, H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME), H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), handle);                                                            \

/*this macro introduces the definition for _destroy of for the IFabric type wrapper*/
#define H_FABRIC_DEFINE_DESTROY(IFABRIC_INTERFACE_NAME)                                                                                                                                     \
void H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle)                                                                                        \
{                                                                                                                                                                                           \
    if (handle == NULL)                                                                                                                                                                     \
    {                                                                                                                                                                                       \
        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_004: [ If handle is NULL then H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME) shall return. ]*/                                             \
        LogError("invalid arguments " MU_TOSTRING(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME)) " handle=%p", handle);                                                                           \
    }                                                                                                                                                                                       \
    else                                                                                                                                                                                    \
    {                                                                                                                                                                                       \
        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_005: [ H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME) shall call ->Release() on the instance of IFABRIC_INTERFACE_NAME. ]*/                \
        (void)handle->This->lpVtbl->Release(handle->This);                                                                                                                                  \
        /*Codes_SRS_H_FABRIC_MACRO_GENERATOR_02_006: [ H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME) shall free the allocated memory. ]*/                                                 \
        free(handle);                                                                                                                                                                       \
    }                                                                                                                                                                                       \
}                                                                                                                                                                                           \



#endif /*H_FABRIC_MACRO_GENERATOR_H*/
