````instructions
# sf-c-util AI Coding Instructions

## Project Overview
sf-c-util provides C wrappers for Microsoft Azure Service Fabric COM APIs. It bridges C handle-based code with Service Fabric's COM interfaces, offering configuration readers, async operation callbacks, and string result implementations.

## Core Architecture
**COM Interface Bridging**: The fundamental pattern uses `com-wrapper` to expose Service Fabric COM interfaces through C handles:
```c
// 1. Define C handle API
MOCKABLE_FUNCTION(, FABRIC_ASYNC_OP_CB_HANDLE, fabric_async_op_cb_create, USER_INVOKE_CB, callback, void*, context);

// 2. Define COM wrapper in *_com.h
#define FABRIC_ASYNC_OP_CB_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IFabricAsyncOperationCallback, \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_async_op_cb_Invoke, IFabricAsyncOperationContext*, context))
DECLARE_COM_WRAPPER_OBJECT(FABRIC_ASYNC_OP_CB_HANDLE, FABRIC_ASYNC_OP_CB_HANDLE_INTERFACES);
```

**Configuration Reading**: All readers follow identical patterns across types (uint32_t, wchar_t*, THANDLE(RC_STRING), bool):
```c
int configuration_reader_get_TYPE(IFabricCodePackageActivationContext* activation_context,
    const wchar_t* config_package_name, const wchar_t* section_name, 
    const wchar_t* parameter_name, TYPE* value);
```

## Essential Usage Patterns

### Service Fabric Error Handling
```c
#include "sf_c_util/hresult_to_string.h"
// Always check HRESULTs and convert to strings for logging
HRESULT hr = sf_operation();
if (FAILED(hr)) {
    char* error_str = hresult_to_string(hr);
    LogError("SF operation failed: %s", error_str);
    free(error_str);
}
```

### COM Wrapper Usage
```c
// Create C handle, then get COM interface
FABRIC_STRING_RESULT_HANDLE handle = fabric_string_result_create(L"result");
IFabricStringResult* com_obj = COM_WRAPPER_CREATE(FABRIC_STRING_RESULT_HANDLE, IFabricStringResult, handle, fabric_string_result_destroy);
// COM object takes ownership of handle and manages lifecycle
```

### Configuration Reading Best Practices
```c
// Prefer THANDLE(RC_STRING) for automatic memory management
THANDLE(RC_STRING) config_value = NULL;
if (configuration_reader_get_thandle_rc_string(activation_context, L"Config", L"Section", L"Param", &config_value) == 0) {
    const char* value_str = rc_string_c_str(config_value);
    // Use value_str...
    THANDLE_ASSIGN(RC_STRING)(&config_value, NULL); // cleanup
}

// For direct wchar_t*, caller must free
wchar_t* wide_string = NULL;
if (configuration_reader_get_wchar_string(activation_context, L"Config", L"Section", L"Param", &wide_string) == 0) {
    // Use wide_string...
    free(wide_string); // Required cleanup
}
```

## Testing Approach
Mock Service Fabric COM interfaces using umock-c patterns:
```c
#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#undef ENABLE_MOCKS

// Mock Service Fabric COM calls
STRICT_EXPECTED_CALL(activation_context_GetConfigurationPackage(IGNORED_ARG, IGNORED_ARG)).SetReturn(S_OK);
```

## SRS Requirements Traceability
Follow standard Azure C library SRS patterns for all modules:
```c
// Requirements: **SRS_MODULE_42_001: [ If param is NULL then function shall fail ]**
// Implementation: /*Codes_SRS_MODULE_42_001: [ If param is NULL then function shall fail ]*/
// Tests: // Tests_SRS_MODULE_42_001: [ If param is NULL then function shall fail ]
```

## Key Components
- **`inc/sf_c_util/`**: Public C APIs for Service Fabric integration
- **`src/`**: Implementation following Azure C library standards
- **`sfwrapper/`**: Generated sync wrappers for async Service Fabric management APIs
- **`deps/servicefabric/`**: Service Fabric SDK headers and IDL files

## External Dependencies
**CRITICAL**: Follow coding standards in `deps/c-build-tools/.github/general_coding_instructions.md`.
Leverage patterns from:
- `deps/c-build-tools/.github/copilot-instructions.md` (CMake functions, pipeline templates, quality gates)
- `deps/c-pal/.github/copilot-instructions.md` (THANDLE, reference counting, platform abstraction)
- `deps/c-util/.github/copilot-instructions.md` (RC_STRING, async operations, collections)
- `deps/c-logging/.github/copilot-instructions.md` (structured logging, ETW support)
- `deps/com-wrapper/.github/copilot-instructions.md` (COM interface wrapping)
- `deps/macro-utils-c/.github/copilot-instructions.md` (metaprogramming, code generation)
- `deps/umock-c/.github/copilot-instructions.md` (unit test mocking patterns)
- `deps/ctest/.github/copilot-instructions.md` (test execution framework)
- `deps/c-testrunnerswitcher/.github/copilot-instructions.md` (test runner abstraction)

This library enables C applications to use Service Fabric APIs through familiar C patterns while maintaining COM interface compatibility.

````
