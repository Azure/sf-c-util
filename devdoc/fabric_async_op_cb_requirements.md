`fabric_async_op_cb` requirements
================

## Overview

`fabric_async_op_cb` is a module that implements the callback needed for implementing a wrapper for the fabric asynchronous operations.

Note: This unit contains APIs that can be wrapped using `com_wrapper` to produce a wrapper that implements the `IFabricAsyncOperationCallback` interface.

## Exposed API

```c
    typedef void (*USER_INVOKE_CB)(void* user_context, IFabricAsyncOperationContext* fabric_async_operation_context);
    typedef struct FABRIC_ASYNC_OP_CB_TAG* FABRIC_ASYNC_OP_CB_HANDLE;

    MOCKABLE_FUNCTION(, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb_create, USER_INVOKE_CB, user_invoke_cb, void*, user_invoke_cb_context);
    MOCKABLE_FUNCTION(, void, fabric_async_op_cb_destroy, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb);
    MOCKABLE_FUNCTION(, void, fabric_async_op_cb_Invoke, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb, IFabricAsyncOperationContext*, context);
```

### fabric_async_op_cb_create

```c
MOCKABLE_FUNCTION(, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb_create, USER_INVOKE_CB, user_invoke_cb, void*, user_invoke_cb_context);
```

`fabric_async_op_cb_create` allocates a new fabric asynchronous operation callback instance.

**SRS_FABRIC_ASYNC_OP_CB_01_001: [** If `user_invoke_cb` is `NULL`, `fabric_async_op_cb_create` shall fail and return `NULL`. **]**

**SRS_FABRIC_ASYNC_OP_CB_01_002: [** `user_invoke_cb_context` shall be allowed to be `NULL`. **]**

**SRS_FABRIC_ASYNC_OP_CB_01_003: [** Otherwise, `fabric_async_op_cb_create` shall allocate a new callback instance and on success return a non-`NULL` pointer to it. **]**

**SRS_FABRIC_ASYNC_OP_CB_01_004: [** If any error occurs, `fabric_async_op_cb_create` shall fail and return `NULL`. **]**

### fabric_async_op_cb_destroy

```c
MOCKABLE_FUNCTION(, void, fabric_async_op_cb_destroy, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb);
```

`fabric_async_op_cb_destroy` frees the resources associated with `fabric_async_op_cb`.

**SRS_FABRIC_ASYNC_OP_CB_01_005: [** If `fabric_async_op_cb` is `NULL`, `fabric_async_op_cb_destroy` shall return. **]**

**SRS_FABRIC_ASYNC_OP_CB_01_006: [** Otherwise, `fabric_async_op_cb_destroy` shall free the memory allocated in `fabric_async_op_cb_create`. **]**

### fabric_async_op_cb_Invoke

```c
MOCKABLE_FUNCTION(, void, fabric_async_op_cb_Invoke, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb, IFabricAsyncOperationContext*, context);
```

`fabric_async_op_cb_Invoke` invokes the user callback passed to `fabric_async_op_cb_create`.

**SRS_FABRIC_ASYNC_OP_CB_01_007: [** If `fabric_async_op_cb` is `NULL`, `fabric_async_op_cb_Invoke` shall return. **]**

**SRS_FABRIC_ASYNC_OP_CB_01_008: [** Otherwise `fabric_async_op_cb_Invoke` shall call the callback `user_invoke_cb` and pass as arguments `user_invoke_cb_context` and `context`. **]**
