`hresult_to_string` requirements
============

## Overview

`hresult_to_string` is a module that produces the string representation of an HRESULT code. It will look into SYSTEM, ServiceFabric and all the loaded modules for a matching message. 
The produced string needs to be free'd by the caller.

As a matter of usability it will also expose a macro (`LogHRESULTError`) that prints on the console the same content as if LogError would have been called + the meaning of the HRESULT. `LogHRESULTError` uses char*.

## Exposed API

```c

typedef void* (*malloc_t)(size_t);

typedef void (*free_t)(void* ptr);

char* hresult_to_string(malloc_t the_malloc, free_t the_free, HRESULT hresult);
wchar_t* hresult_to_wstring(HRESULT hresult);

#define LogHRESULTError(hr, FORMAT, ...)

```

### hresult_to_string
```c
char* hresult_to_string(malloc_t the_malloc, free_t the_free, HRESULT hresult);
```

`hresult_to_string` produces a human readable `char*` representation of `hresult` using `the_malloc` for allocating memory and `the_free` for freeing the allocated memory. 

**SRS_HRESULT_TO_STRING_02_018: [** If `the_malloc` is `NULL` then `hresult_to_string` shall fail and return `NULL`. **]**

**SRS_HRESULT_TO_STRING_02_017: [** If `the_free` is `NULL` then `hresult_to_string` shall fail and return `NULL`. **]**

**SRS_HRESULT_TO_STRING_02_001: [** `hresult_to_string` shall allocate memory for a "big enough" string representation. **]**

**SRS_HRESULT_TO_STRING_02_002: [** `hresult_to_string` shall call `FormatMessageA` with `dwFlags` set to `FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS`. **]**

**SRS_HRESULT_TO_STRING_02_003: [** If `FormatMessageA` succeeds then `hresult_to_string` shall return the value as given by `FormatMessageA`. **]**

**SRS_HRESULT_TO_STRING_02_004: [** Otherwise, `hresult_to_string` shall look in all Service Fabric code for a match and return that match. **]**

**SRS_HRESULT_TO_STRING_02_005: [** If no Service Fabric codes match `hresult` then `hresult_to_string` shall look in all the loaded modules by the current process. **]**

**SRS_HRESULT_TO_STRING_02_006: [** If a module can decode `hresult` then that value shall be returned. **]**

**SRS_HRESULT_TO_STRING_02_007: [** Otherwise `NULL` shall be returned. **]**

**SRS_HRESULT_TO_STRING_02_008: [** If there are any failures then `hresult_to_string` shall return `NULL`. **]**

### hresult_to_wstring
```c
wchar_t* hresult_to_wstring(HRESULT hresult);
```

`hresult_to_wstring` produces a human readable `wchar_t*` representation of `hresult`.

**SRS_HRESULT_TO_STRING_02_009: [** `hresult_to_wstring` shall allocate memory for a "big enough" string representation. **]**

**SRS_HRESULT_TO_STRING_02_010: [** `hresult_to_wstring` shall call `FormatMessageW` with `dwFlags` set to `FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS`. **]**

**SRS_HRESULT_TO_STRING_02_011: [** If `FormatMessageW` succeeds then `hresult_to_wstring` shall return the value as given by `FormatMessageW`. **]**

**SRS_HRESULT_TO_STRING_02_012: [** Otherwise, `hresult_to_wstring` shall look in all Service Fabric code for a match and return that match. **]**

**SRS_HRESULT_TO_STRING_02_013: [** If no Service Fabric codes match `hresult` then `hresult_to_wstring` shall look in all the loaded modules by the current process. **]**

**SRS_HRESULT_TO_STRING_02_014: [** If a module can decode `hresult` then that value shall be returned. **]**

**SRS_HRESULT_TO_STRING_02_015: [** Otherwise `NULL` shall be returned. **]**

**SRS_HRESULT_TO_STRING_02_016: [** If there are any failures then `hresult_to_wstring` shall return `NULL`. **]**


### LogHRESULTError
```c
#define LogHRESULTError(hr, FORMAT, ...)
```

`LogHRESULTError` will behave as if `LogError(FORMAT, ...)` would have been called and it will append " HRESULT=<<human readable string>>" as obtained from `hresult_to_string`.

