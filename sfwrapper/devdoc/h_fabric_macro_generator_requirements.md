`h_fabric_macro_generator` requirements
============

## Overview

`h_fabric_macro_generator` is a header wraps an existing `IFabricZZZsync` and adds retry and timeout functionalities to it. The below diagram shows the stack of modules built on top of what service fabric provides. The diagram refers to `IFabricQueryClient10` from `fabricclient.h` for examplification reasons. 
```c

Layer name:                      API example:
                                 +=====================================+
h_fabric_macro_generator (C)     |  GetNodeList(HANDLE)                |
                                 +=====================================+
IFabricQueryClient10sync (C)     |  GetNodeList(IFabricQueryClient10)  |
                                 +=====================================+
Service Fabric DoX (C++)         |  ServiceFabric_DoX                  |
                                 +=====================================+
Service Fabric (IDL)             |  IFabricQueryClient10               |
                                 +=====================================+
```

## Exposed API

```c
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

/*the below macro, in_args is a single preprocessor token in the form IN_ARGS(ARG(type1, name1), ARG(type2, name2),...)*/
#define H_FABRIC_DECLARE_API(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args) \
    MOCKABLE_FUNCTION(, HRESULT, H_FABRIC_API(IFABRIC_METHOD_NAME), H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), handle ARGS_MOCKABLE_FUNCTION_DECLARATION(in_args))

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries and timeout.*/
#define H_FABRIC_DEFINE_API(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args)                                                                                                           \
HRESULT H_FABRIC_API(IFABRIC_METHOD_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle                                                                                                    \
    ARGS_C_DECLARATION(in_args)                                                                                                                                                             \

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries and timeout.*/
#define H_FABRIC_DEFINE_API_WITH_RESULTS(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args, permanent_failures)                                                                          \
HRESULT H_FABRIC_API(IFABRIC_METHOD_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle                                                                                                    \
    ARGS_C_DECLARATION(in_args)                                                                                                                                                             \

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries.*/
#define H_FABRIC_DEFINE_API_NO_SF_TIMEOUT(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args)                                                                                             \
HRESULT H_FABRIC_API(IFABRIC_METHOD_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle                                                                                                    \
    ARGS_C_DECLARATION(in_args)                                                                                                                                                             \

/*this macro expands to a full function definition. The name of the function is H_FABRIC_API(IFABRIC_METHOD_NAME) and it contains the code to run with retries.*/
#define H_FABRIC_DEFINE_API_NO_SF_TIMEOUT_WITH_RESULTS(IFABRIC_INTERFACE_NAME, IFABRIC_METHOD_NAME, in_args, permanent_failures)                                                            \
HRESULT H_FABRIC_API(IFABRIC_METHOD_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle                                                                                                    \
    ARGS_C_DECLARATION(in_args)                                                                                                                                                             \

/*this macro introduces the declaration for _create for the IFabric type wrapper*/                                                                                                          \
#define H_FABRIC_DECLARE_CREATE(IFABRIC_INTERFACE_NAME)                                                                                                                                     \
    H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)(uint32_t nMaxRetries, uint32_t msBetweenRetries);                                                \

/*this macro introduces the definition for _create of for the IFabric type wrapper*/
#define H_FABRIC_DEFINE_CREATE(IFABRIC_INTERFACE_NAME)                                                                                                                                      \
H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)(uint32_t nMaxRetries, uint32_t msBetweenRetries)                                                     \

/*this macro introduces the declaration for _destroy for the IFabric type wrapper*/
#define H_FABRIC_DECLARE_DESTROY(IFABRIC_INTERFACE_NAME)                                                                                                                                    \
    void H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle);                                                                                   \

/*this macro introduces the definition for _destroy of for the IFabric type wrapper*/
#define H_FABRIC_DEFINE_DESTROY(IFABRIC_INTERFACE_NAME)                                                                                                                                     \
void H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)(H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME) handle)                                                                                        \

```

Example usage (based on `IFabricQueryClient10`):

In the header file:

```c
H_FABRIC_DEFINE_TYPEDEF(IFabricQueryClient10);

H_FABRIC_DECLARE_CREATE(IFabricQueryClient10);
H_FABRIC_DECLARE_DESTROY(IFabricQueryClient10);

H_FABRIC_DECLARE_API(IFabricQueryClient10, GetNodeList,
    IN_ARGS(
        ARG(const FABRIC_NODE_QUERY_DESCRIPTION*, queryDescription),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricGetNodeListResult**, fabricGetNodeListResult)
    )
)

H_FABRIC_DECLARE_API(IFabricQueryClient10, SomeOtherApi,
    IN_ARGS(
        ARG(const FABRIC_NODE_QUERY_DESCRIPTION*, queryDescription),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricGetNodeListResult**, fabricGetNodeListResult)
    )
)

```

In the C file:

```c
H_FABRIC_DEFINE_STRUCT_TYPE(IFabricQueryClient10);

H_FABRIC_DEFINE_CREATE(IFabricQueryClient10);
H_FABRIC_DEFINE_DESTROY(IFabricQueryClient10);

H_FABRIC_DEFINE_API(IFabricQueryClient10, GetNodeList,
    IN_ARGS(
        ARG(const FABRIC_NODE_QUERY_DESCRIPTION*, queryDescription),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricGetNodeListResult**, fabricGetNodeListResult)
    )
)

H_FABRIC_DEFINE_API_WITH_RESULTS(IFabricQueryClient10, SomeOtherApi,
    IN_ARGS(
        ARG(const FABRIC_NODE_QUERY_DESCRIPTION*, queryDescription),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricGetNodeListResult**, fabricGetNodeListResult)
    ),
    RESULTS(FABRIC_E_APPLICATION_NOT_FOUND)
)

```

### H_FABRIC_DECLARE_CREATE / H_FABRIC_DEFINE_CREATE
```c
MOCKABLE_FUNCTION(, H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME), uint32_t, nMaxRetries, uint32_t, msBetweenRetries);
```

`H_FABRIC_DECLARE_CREATE / H_FABRIC_DEFINE_CREATE` declare / define a function that returns `H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME)` which can be used with the other APIs. The name of the function is `H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)`. `nMaxRetries` limits the number of calls to the underlying layer to `nMaxRetries`.


**SRS_H_FABRIC_MACRO_GENERATOR_02_001: [** `H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)` shall allocate memory to hold `nMaxRetries`, `msBetweenRetries` and an instance of `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_002: [** `H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)` shall create a new instance of `IFABRIC_INTERFACE_NAME` by calling `CREATE_IFABRICINSTANCE_NAME(IFABRIC_INTERFACE_NAME)`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_003: [** If there are any failures then `H_FABRIC_HANDLE_CREATE(IFABRIC_INTERFACE_NAME)` shall fail and return `NULL`. **]**


### H_FABRIC_DECLARE_DESTROY / H_FABRIC_DEFINE_DESTROY
```c
MOCKABLE_FUNCTION(, void, H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME), H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), handle);
```

`H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)` declare/define a function that frees all resources used by `H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME)`.

**SRS_H_FABRIC_MACRO_GENERATOR_02_004: [** If `handle` is `NULL` then `H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)` shall return. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_005: [** `H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)` shall call ->Release() on the instance of `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_006: [** `H_FABRIC_HANDLE_DESTROY(IFABRIC_INTERFACE_NAME)` shall free the allocated memory. **]**


### H_FABRIC_DECLARE_API / H_FABRIC_DEFINE_API / H_FABRIC_DEFINE_API_WITH_RESULTS
```
MOCKABLE_FUNCTION(, HRESULT, H_FABRIC_API(IFABRIC_METHOD_NAME), H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), handle ARGS_MOCKABLE_FUNCTION_DECLARATION(in_args))
```

`H_FABRIC_DECLARE_API / H_FABRIC_DEFINE_API` declare / define an API with retry and timeout over an existing _sync API. `in_args` are the same as the IFabricZZZsync's API layer arguments. One of them is called `timeoutMilliseconds` and has as meaning "total time spend executing".

The macro `H_FABRIC_DEFINE_API_WITH_RESULTS` adds the optional parameter `permanent_failures`. `permanent_failures` are any failure codes that may be returned by the API that indicate the call should not be retried.

**SRS_H_FABRIC_MACRO_GENERATOR_02_007: [** If `handle` is `NULL` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall fail and return `NULL`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_008: [** `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall record the start time of the request by calling `timer_global_get_elapsed_ms`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_009: [** `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall call `IFABRIC_METHOD_NAME` on the instance of `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_010: [** If the call succeeds then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall succeed and return. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_011: [** If the result is `FABRIC_E_OBJECT_CLOSED`, `FABRIC_E_GATEWAY_NOT_REACHABLE`, `FABRIC_E_TIMEOUT` or `E_ABORT` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall create a new instance of `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_012: [** If creating the new instance of `IFABRIC_INTERFACE_NAME` fails then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall retry using the existing `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_013: [** Otherwise `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall release the existing instance and shall retry using new newly created `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_42_001: [** If the result is any value from `permanent_failures` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall return. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_014: [** If the result is any other value except `FABRIC_E_OBJECT_CLOSED`, `FABRIC_E_GATEWAY_NOT_REACHABLE` or `E_ABORT` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall retry. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_015: [** `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall sleep `msBetweenRetries`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_016: [** If the total time spend retrying exceeds `timeoutMilliseconds` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall return the last error code. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_02_017: [** If the total number of retries exceeds `nMaxRetries` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall return the last error code. **]**

### H_FABRIC_DEFINE_API_NO_SF_TIMEOUT / H_FABRIC_DEFINE_API_NO_SF_TIMEOUT_WITH_RESULTS
```
MOCKABLE_FUNCTION(, HRESULT, H_FABRIC_API(IFABRIC_METHOD_NAME), H_FABRIC_HANDLE(IFABRIC_INTERFACE_NAME), handle ARGS_MOCKABLE_FUNCTION_DECLARATION(in_args))
```

`H_FABRIC_DEFINE_API_NO_SF_TIMEOUT` defines an API over an existing _sync API that has no timeout argument that can be passed to Service Fabric. `in_args` are the same as the IFabricZZZsync's API layer arguments. 

The macro `H_FABRIC_DEFINE_API_NO_SF_TIMEOUT_WITH_RESULTS` adds the optional parameter `permanent_failures`. `permanent_failures` are any failure codes that may be returned by the API that indicate the call should not be retried.

**SRS_H_FABRIC_MACRO_GENERATOR_01_001: [** If `handle` is `NULL` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall fail and return `NULL`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_002: [** `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall call `IFABRIC_METHOD_NAME` on the instance of `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_003: [** If the call succeeds then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall succeed and return. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_004: [** If the result is `FABRIC_E_OBJECT_CLOSED`, `FABRIC_E_GATEWAY_NOT_REACHABLE` or `E_ABORT` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall create a new instance of `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_005: [** If creating the new instance of `IFABRIC_INTERFACE_NAME` fails then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall retry using the existing `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_006: [** Otherwise `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall release the existing instance and shall retry using new newly created `IFABRIC_INTERFACE_NAME`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_42_002: [** If the result is any value from `permanent_failures` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall return. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_007: [** If the result is any other value except `FABRIC_E_OBJECT_CLOSED`, `FABRIC_E_GATEWAY_NOT_REACHABLE` or `E_ABORT` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall retry. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_008: [** `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall sleep `msBetweenRetries`. **]**

**SRS_H_FABRIC_MACRO_GENERATOR_01_009: [** If the total number of retries exceeds `nMaxRetries` then `H_FABRIC_API(IFABRIC_METHOD_NAME)` shall return the last error code. **]**

