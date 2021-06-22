`fabric_async_op_sync_wrapper` requirements
================

## Overview

`fabric_async_op_sync_wrapper` is a module that implements a synchronous wrapper over the Begin/End APIs that are part of a Fabric asynchronous operation.

## Exposed API

```c
#define DECLARE_FABRIC_ASYNC_OPERATION_SYNC(interface_name, operation_name, ...) \
    ...

#define DEFINE_FABRIC_ASYNC_OPERATION_SYNC(interface_name, operation_name, ...) \
    ...
```

### DECLARE_FABRIC_ASYNC_OPERATION_SYNC

```c
#define DECLARE_FABRIC_ASYNC_OPERATION_SYNC(interface_name, operation_name, ...)
```

`DECLARE_FABRIC_ASYNC_OPERATION_SYNC` expands to code declaring a wrapper function that executes the asynchronous operation synchronously.

`...` is made of:

- `BEGIN_ARGS(...)`, which is a list of tuples containing the argument type and name for each of the begin arguments.

- `END_ARGS(...)`, which is a list of tuples containing the argument type and name for each of the end arguments.

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_001: [** `DECLARE_FABRIC_ASYNC_OPERATION_SYNC` shall declare a function with the following prototype: **]**

```c
HRESULT {interface_name}_{operation_name}_execute(interface_name* com_object, {begin_args}, {end_args});
```

### DEFINE_FABRIC_ASYNC_OPERATION_SYNC

```c
#define DEFINE_FABRIC_ASYNC_OPERATION_SYNC(interface_name, operation_name, ...)
```

`DEFINE_FABRIC_ASYNC_OPERATION_SYNC` expands to code implementing a wrapper function that executes the asynchronous operation synchronously.

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_002: [** `DEFINE_FABRIC_ASYNC_OPERATION_SYNC` shall implement a function with the following prototype: **]**

```c
HRESULT {interface_name}_{operation_name}_execute(interface_name* com_object, {begin_args}, {end_args_pointers});
```

Note: {end_args_pointers} is a list pointers to each of the arguments defined with the `END_ARGS` macro.

For example if the end arguments are: `END_ARGS(int, a, double b)`, {end_args_pointers} will be `int* a, double* b`.

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_003: [** If `com_object` is NULL, `_execute` shall fail and return `E_INVALIDARG`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_020: [** If any of the end arg pointers is `NULL`, `_execute` shall fail and return `E_INVALIDARG`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_004: [** `_execute` shall create a async operation callback object by calling `fabric_async_op_cb_create`, passing as arguments the wrapper complete callback. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_005: [** A COM wrapper shall be created for the async operation callback object. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_006: [** `_execute` shall call `Begin{operation_name}` on `com_object`, passing as arguments the begin arguments and the async operation callback COM object. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_007: [** If `fabric_operation_context` has not completed synchronously, `_execute` shall wait to be signalled by the `_sync_wrapper_cb` function. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_008: [** `_execute` shall call `End{operation_name}` on `com_object`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_009: [** `_execute` shall release the asynchronous operation context obtained from `Begin{operation_name}`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_010: [** On success, `_execute` shall return `S_OK`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_011: [** If `Begin{operation_name}` fails, `_execute` shall return the error returned by `Begin{operation_name}`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_012: [** If `End{operation_name}` fails, `_execute` shall return the error returned by `End{operation_name}`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_013: [** If any other error occurs, `_execute` shall fail and return `E_FAIL`. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_014: [** `DEFINE_FABRIC_ASYNC_OPERATION_SYNC` shall define the wrapper completion callback: **]**

```c
static void {interface_name}_{operation_name}_sync_wrapper_cb(void* context, IFabricAsyncOperationContext* fabric_async_operation_context);
```

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_015: [** If `context` is `NULL`, `_sync_wrapper_cb` shall terminate the process. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_016: [** If `fabric_async_operation_context` is `NULL`, `_sync_wrapper_cb` shall terminate the process. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_017: [** Otherwise, `_sync_wrapper_cb` shall check whether the async operation has completed synchronously. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_018: [** If the async operation has completed synchronously, `_sync_wrapper_cb` shall return. **]**

**SRS_FABRIC_ASYNC_OP_SYNC_WRAPPER_01_019: [** If the async operation has not completed synchronously `_sync_wrapper_cb` shall signal to unblock `_execute`. **]**
