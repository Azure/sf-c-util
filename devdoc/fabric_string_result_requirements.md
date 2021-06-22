`fabric_string_result` requirements
================

## Overview

`fabric_string_result` is a module that encapsulates a string result so that it can be wrapped in the COM interface `IFabricStringResult`.

## Design

`fabric_string_result` copies the string and whenever queried it returns a pointer to the copy.

## Exposed API

```c
    typedef struct FABRIC_STRING_RESULT_TAG* FABRIC_STRING_RESULT_HANDLE;

    MOCKABLE_FUNCTION(, FABRIC_STRING_RESULT_HANDLE, fabric_string_result_create, const wchar_t*, string_result);
    MOCKABLE_FUNCTION(, void, fabric_string_result_destroy, FABRIC_STRING_RESULT_HANDLE, fabric_string_result);
    MOCKABLE_FUNCTION(, LPCWSTR, fabric_string_result_get_String, FABRIC_STRING_RESULT_HANDLE, fabric_string_result);
```

### fabric_string_result_create

```c
MOCKABLE_FUNCTION(, FABRIC_STRING_RESULT_HANDLE, fabric_string_result_create, const wchar_t*, string_result);
```

`fabric_string_result_create` creates a new fabric string result handle.

**SRS_FABRIC_STRING_RESULT_01_008: [** If `string_result` is NULL, `fabric_string_result_create` shall fail and return `NULL`. **]**

**SRS_FABRIC_STRING_RESULT_01_001: [** `fabric_string_result_create` shall allocate a new fabric string result instance and on success return a non-NULL pointer to it. **]**

**SRS_FABRIC_STRING_RESULT_01_002: [** `fabric_string_result_create` shall copy the `string_result` string so it can be returned by `get_String`. **]**

**SRS_FABRIC_STRING_RESULT_01_003: [** If any error occurs, `fabric_string_result_create` shall fail and return NULL. **]**

### fabric_string_result_destroy

```c
MOCKABLE_FUNCTION(, void, fabric_string_result_destroy, FABRIC_STRING_RESULT_HANDLE, fabric_string_result);
```

`fabric_string_result_destroy` frees all resources associated with a fabric string result handle.

**SRS_FABRIC_STRING_RESULT_01_004: [** If `fabric_string_result` is `NULL`, `fabric_string_result_destroy` shall return. **]**

**SRS_FABRIC_STRING_RESULT_01_005: [** Otherwise, `fabric_string_result_destroy` shall free the memory associated with the fabric string result instance. **]**

### fabric_string_result_get_String

```c
MOCKABLE_FUNCTION(, LPCWSTR, fabric_string_result_get_String, FABRIC_STRING_RESULT_HANDLE, fabric_string_result);
```

`fabric_string_result_get_String` get the result string held by `fabric_string_result'.

**SRS_FABRIC_STRING_RESULT_01_006: [** If `fabric_string_result` is `NULL`, `fabric_string_result_get_String` shall fail and return `NULL`. **]**

**SRS_FABRIC_STRING_RESULT_01_007: [** Otherwise, `fabric_string_result_get_String` shall return a pointer to the string copied in `fabric_string_result_create`. **]**
