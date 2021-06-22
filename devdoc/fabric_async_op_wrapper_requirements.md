`fabric_async_op_wrapper` requirements
================

## Overview

`fabric_async_op_wrapper` is a module that implements a wrapper over the Begin/End APIs that are part of a Fabric asynchronous operation.

## Exposed API

```c
#define DECLARE_FABRIC_ASYNC_OPERATION(interface_name, operation_name, ...) \
    ...

#define DEFINE_FABRIC_ASYNC_OPERATION(interface_name, operation_name, ...) \
    ...
```

### DECLARE_FABRIC_ASYNC_OPERATION

```c
#define DECLARE_FABRIC_ASYNC_OPERATION(interface_name, operation_name, ...)
```

`DECLARE_FABRIC_ASYNC_OPERATION` expands to code declaring a wrapper function that executes the asynchronous operation and the associated completion callback.

`...` is made of:

- `BEGIN_ARGS(...)`, which is a list of tuples containing the argument type and name for each of the begin arguments.

- `END_ARGS(...)`, which is a list of tuples containing the argument type and name for each of the end arguments.

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_001: [** `DECLARE_FABRIC_ASYNC_OPERATION` shall declare a function with the following prototype: **]**

```c
HRESULT {interface_name}_{operation_name}_execute_async(interface_name* com_object, {begin_args}, {interface_name}_{operation_name}_COMPLETE_CB on_complete, void* on_complete_context);
```

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_002: [** `DECLARE_FABRIC_ASYNC_OPERATION` shall declare the async operation completion callback with the prototype: **]**

```c
typedef void {interface_name}_{operation_name}_COMPLETE_CB(void* context, HRESULT async_operation_result, {end_args});
```

### DEFINE_FABRIC_ASYNC_OPERATION

```c
#define DEFINE_FABRIC_ASYNC_OPERATION(interface_name, operation_name, ...)
```

`DEFINE_FABRIC_ASYNC_OPERATION` expands to code implementing a wrapper function that executes the asynchronous operation and the associated completion callback.

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_025: [** `DEFINE_FABRIC_ASYNC_OPERATION` shall implement a function with the following prototype: **]**

```c
HRESULT {interface_name}_{operation_name}_execute_async(interface_name* com_object, {begin_args}, {interface_name}_{operation_name}_COMPLETE_CB on_complete, void* on_complete_context);
```

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_003: [** If `com_object` is NULL, `_execute_async` shall fail and return `E_INVALIDARG`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_004: [** If `on_complete` is NULL, `_execute_async` shall fail and return `E_INVALIDARG`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_005: [** `on_complete_context` shall be allowed to be `NULL`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_006: [** `_execute_async` shall allocate a context used to store `on_complete` and `on_complete_context`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_027: [** `_execute_async` shall increment the reference count for `com_object`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_007: [** `_execute_async` shall create a async operation callback object by calling `fabric_async_op_cb_create`, passing as arguments the wrapper complete callback and the context with `on_complete` and `on_complete_context`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_008: [** A COM wrapper shall be created for the async operation callback object. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_009: [** `_execute_async` shall call `Begin{operation_name}` on `com_object`, passing as arguments the begin arguments and the async operation callback COM object. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_010: [** If `fabric_operation_context` has completed synchronously: **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_011: [** `_execute_async` shall call `End{operation_name}` on `com_object`. **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_012: [** `_execute_async` shall call the `on_complete` and pass as arguments `on_complete_context`, `S_OK` and the end argument values obtained from `End{operation_name}`. **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_028: [** `_execute_async` shall decrement the reference count for `com_object`. **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_029: [** `_execute_async` shall free the context used to store `on_complete` and `on_complete_context`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_013: [** `_execute_async` shall release the asynchronous operation context obtained from `Begin{operation_name}`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_026: [** On success, `_execute_async` shall return `S_OK`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_014: [** If `Begin{operation_name}` fails, `_execute_async` shall return the error returned by `Begin{operation_name}`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_015: [** If `End{operation_name}` fails, `_execute_async` shall return the error returned by `End{operation_name}`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_016: [** If any other error occurs, `_execute_async` shall fail and return `E_FAIL`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_017: [** `DEFINE_FABRIC_ASYNC_OPERATION` shall define the wrapper completion callback: **]**

```c
static void {interface_name}_{operation_name}_wrapper_cb(void* context, IFabricAsyncOperationContext* fabric_async_operation_context);
```

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_018: [** If `context` is `NULL`, `_wrapper_cb` shall return. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_019: [** Otherwise, `_wrapper_cb` shall check whether the async operation has completed synchronously. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_020: [** If the async operation has completed synchronously, `_wrapper_cb` shall return. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_021: [** If the async operation has not completed synchronously: **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_022: [** `_wrapper_cb` shall call `End{operation_name}` on the `com_object` passed to `_execute_async`. **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_023: [** `_wrapper_cb` shall call the `on_complete` and pass as arguments `on_complete_context`, `S_OK` and the end argument values obtained from `End{operation_name}`. **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_030: [** `_wrapper_cb` shall release the com object passed as argument to `_execute_async`. **]**

- **SRS_FABRIC_ASYNC_OP_WRAPPER_01_031: [** `_wrapper_cb` shall free the context created in `_execute_async`. **]**

**SRS_FABRIC_ASYNC_OP_WRAPPER_01_024: [** If the `End{operation_name}` fails, `_wrapper_cb` shall call the `on_complete` and pass as arguments `on_complete_context` and the result of the `End{operation_name}` call. **]**
