`fabric_op_completed_sync_ctx` requirements
================

## Overview

`fabric_op_completed_sync_ctx` is a module that implements the IFabricAsyncOperationContext contract as completing always synchronously.

Note: This unit contains APIs that can be wrapped using `com_wrapper` to produce a wrapper that implements the `IFabricAsyncOperationContext` interface.

## Exposed API

```c
    typedef struct FABRIC_OP_COMPLETED_SYNC_CTX_TAG* FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE;

    MOCKABLE_FUNCTION(, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx_create, IFabricAsyncOperationCallback*, callback);
    MOCKABLE_FUNCTION(, void, fabric_op_completed_sync_ctx_destroy, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
    MOCKABLE_FUNCTION(, BOOLEAN, fabric_op_completed_sync_ctx_IsCompleted, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
    MOCKABLE_FUNCTION(, BOOLEAN, fabric_op_completed_sync_ctx_CompletedSynchronously, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
    MOCKABLE_FUNCTION(, HRESULT, fabric_op_completed_sync_ctx_get_Callback, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx, IFabricAsyncOperationCallback**, callback);
    MOCKABLE_FUNCTION(, HRESULT, fabric_op_completed_sync_ctx_Cancel, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
```

### fabric_op_completed_sync_ctx_create

```c
MOCKABLE_FUNCTION(, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx_create, IFabricAsyncOperationCallback*, callback);
```

`fabric_op_completed_sync_ctx_create` creates a new fabric asynchronous operation context.

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_001: [** If `callback` is NULL, `fabric_op_completed_sync_ctx_create` shall fail and return `NULL`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_002: [** Otherwise `fabric_op_completed_sync_ctx_create` shall allocate a new fabric async operation context and on success return a non-NULL pointer to it. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_003: [** `fabric_op_completed_sync_ctx_create` shall increment the reference count for `callback` and store it. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_004: [** If any error occurs, `fabric_op_completed_sync_ctx_create` shall fail and return NULL. **]**

### fabric_op_completed_sync_ctx_destroy

```c
MOCKABLE_FUNCTION(, void, fabric_op_completed_sync_ctx_destroy, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
```

`fabric_op_completed_sync_ctx_destroy` frees a context for a fabric asynchronous operation that completed synchronously.

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_005: [** If `fabric_op_completed_sync_ctx` is `NULL`, `fabric_op_completed_sync_ctx_destroy` shall return. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_006: [** Otherwise, `fabric_op_completed_sync_ctx_destroy` shall decrement the reference count for the callback passed to `fabric_op_completed_sync_ctx_create`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_007: [** `fabric_op_completed_sync_ctx_destroy` shall free the memory associated with `fabric_op_completed_sync_ctx`. **]**

### fabric_op_completed_sync_ctx_IsCompleted

```c
MOCKABLE_FUNCTION(, BOOLEAN, fabric_op_completed_sync_ctx_IsCompleted, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
```

`fabric_op_completed_sync_ctx_IsCompleted` returns whether the operation completed.

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_008: [** If `fabric_op_completed_sync_ctx` is `NULL`, `fabric_op_completed_sync_ctx_IsCompleted` shall return `FALSE`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_009: [** Otherwise `fabric_op_completed_sync_ctx_IsCompleted` shall always return `TRUE`. **]**

### fabric_op_completed_sync_ctx_CompletedSynchronously

```c
MOCKABLE_FUNCTION(, BOOLEAN, fabric_op_completed_sync_ctx_CompletedSynchronously, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
```

`fabric_op_completed_sync_ctx_CompletedSynchronously` returns whether the operation completed synchronously.

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_010: [** If `fabric_op_completed_sync_ctx` is `NULL`, `fabric_op_completed_sync_ctx_CompletedSynchronously` shall return `FALSE`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_011: [** Otherwise `fabric_op_completed_sync_ctx_CompletedSynchronously` shall return `TRUE`. **]**

### async_operation_ctx_sync_get_Callback

```c
MOCKABLE_FUNCTION(, HRESULT, fabric_op_completed_sync_ctx_get_Callback, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx, IFabricAsyncOperationCallback**, callback);
```

`fabric_op_completed_sync_ctx_get_Callback` returns the callback associated with the fabric asynchronous operation.

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_012: [** If `fabric_op_completed_sync_ctx` is NULL, `fabric_op_completed_sync_ctx_get_Callback` shall return `E_FAIL`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_013: [** If `callback` is `NULL`, `fabric_op_completed_sync_ctx_get_Callback` shall return `E_POINTER`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_014: [** Otherwise `fabric_op_completed_sync_ctx_get_Callback` shall increment the ref count for the callback passed as argument to `fabric_op_completed_sync_ctx_create`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_015: [** `fabric_op_completed_sync_ctx_get_Callback` shall return in `callback` the callback passed as argument to `fabric_op_completed_sync_ctx_create`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_016: [** On success, `fabric_op_completed_sync_ctx_get_Callback` shall return `S_OK`. **]**

### fabric_op_completed_sync_ctx_Cancel

```c
MOCKABLE_FUNCTION(, HRESULT, fabric_op_completed_sync_ctx_Cancel, FABRIC_OP_COMPLETED_SYNC_CTX_HANDLE, fabric_op_completed_sync_ctx);
```

`fabric_op_completed_sync_ctx_Cancel` implements the cancel behavior for a fabric asynchronous operation that completed synchronously (Cancel cannot be performed for this case).

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_017: [** If `fabric_op_completed_sync_ctx` is `NULL`, `fabric_op_completed_sync_ctx_Cancel` shall return `E_FAIL`. **]**

**SRS_FABRIC_OP_COMPLETED_SYNC_CTX_01_018: [** Otherwise `fabric_op_completed_sync_ctx` shall return `E_FAIL` as a completed operation cannot be cancelled. **]**
