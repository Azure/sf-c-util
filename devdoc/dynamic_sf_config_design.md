# Dynamic Service Fabric Configuration - Detailed Design

## Overview

This document describes the design for adding dynamic configuration support to the Service Fabric configuration system in `sf-c-util`. The goal is to allow services to:

1. Be notified when Service Fabric configuration packages change
2. Re-read configuration values atomically using a reader-writer lock pattern
3. Take actions based on configuration changes

## Problem Statement

Currently, the `sf_service_config` macros generate configuration structures that are read once at service startup. There is no mechanism to:
- Detect when configuration changes at runtime
- Re-read configuration values safely while the service is running
- Protect concurrent access to configuration values during updates

## Solution Components

The solution consists of two main components:

### 1. Enhanced `sf_service_config` - SRW Lock and Re-read Support

Extend the existing `sf_service_config` macros to add:
- SRW (Slim Reader-Writer) lock protection for thread-safe read/write access
- A refresh function to re-read configuration values from Service Fabric

### 2. `configuration_package_change_handler` - Configuration Change Handler

A new module that implements `IFabricConfigurationPackageChangeHandler` COM interface and provides a simplified callback mechanism when configuration packages change.

---

## Component 1: Enhanced `sf_service_config`

### Purpose

Extend the existing `sf_service_config` to support:
- Multiple concurrent readers (using shared lock)
- Exclusive writer access during configuration updates (using exclusive lock)
- A refresh function to re-read all configuration values

### Changes to Internal Structure

The existing generated struct will be extended to include an SRW lock:

```c
typedef struct MU_C2(name, _TAG)
{
    SRW_LOCK_LL lock;                                    // NEW: SRW lock for thread-safe access
    IFabricCodePackageActivationContext* activation_context;
    const wchar_t* sf_config_name_string;
    const wchar_t* sf_parameters_section_name_string;
    // ... existing config fields ...
} name;
```

### New Macro for Refresh Function

```c
#define SF_SERVICE_CONFIG_REFRESH(name) MU_C2(name, _configuration_refresh)
```

### API Additions

#### New Declaration Macro (or extend existing)

```c
#define DECLARE_SF_SERVICE_CONFIG_REFRESH(name, ...) \
    MOCKABLE_FUNCTION(, int, SF_SERVICE_CONFIG_REFRESH(name), THANDLE(SF_SERVICE_CONFIG(name)), handle);
```

This can be added to the existing `DECLARE_SF_SERVICE_CONFIG` macro or used separately.

### Modified Getter Functions

The existing getter functions will be modified to acquire/release shared locks:

```c
SF_SERVICE_CONFIG_RETURN_TYPE(field_type) SF_SERVICE_CONFIG_GETTER(name, field_name)(
    THANDLE(SF_SERVICE_CONFIG(name)) handle)
{
    SF_SERVICE_CONFIG_RETURN_TYPE(field_type) result;
    if (handle == NULL)
    {
        LogError("Invalid args");
        result = SF_SERVICE_CONFIG_GETTER_ERROR(field_type);
    }
    else
    {
        srw_lock_ll_acquire_shared(&handle->lock);       // NEW
        result = handle->field_name;                      // (for THANDLE types, THANDLE_INITIALIZE)
        srw_lock_ll_release_shared(&handle->lock);       // NEW
    }
    return result;
}
```

### New Refresh Function

```c
int SF_SERVICE_CONFIG_REFRESH(name)(THANDLE(SF_SERVICE_CONFIG(name)) handle);
```

**Behavior:**
1. Validate handle is not NULL
2. Re-read all configuration values into temporary variables using `configuration_reader_get_*` functions
3. If any required configuration fails to read, return error (keeps old values)
4. Acquire exclusive lock using `srw_lock_ll_acquire_exclusive`
5. Clean up old field values (free strings, release THANDLEs)
6. Copy new values into the struct fields
7. Release exclusive lock using `srw_lock_ll_release_exclusive`
8. Return 0 on success, non-zero on failure

### Modified Create Function

The create function will be modified to initialize the SRW lock:

```c
THANDLE(SF_SERVICE_CONFIG(name)) SF_SERVICE_CONFIG_CREATE(name)(
    IFabricCodePackageActivationContext* activation_context)
{
    // ... existing validation and allocation ...
    
    if (srw_lock_ll_init(&temp_config_obj->lock) != 0)   // NEW
    {
        LogError("srw_lock_ll_init failed");
        // ... cleanup and return NULL ...
    }
    
    // ... existing config reading ...
}
```

### Modified Dispose Function

The dispose function will be modified to deinitialize the SRW lock:

```c
static void MU_C2A(SF_SERVICE_CONFIG(name), _dispose)(SF_SERVICE_CONFIG(name)* handle)
{
    // ... existing cleanup of fields ...
    
    srw_lock_ll_deinit(&handle->lock);                   // NEW
    
    (void)handle->activation_context->lpVtbl->Release(handle->activation_context);
}
```

---

## Component 2: Configuration Package Change Handler Architecture

### Overview

The configuration package change handler functionality is split into three separate units for clean separation of concerns and improved testability:

1. **`fabric_configuration_package_change_handler`** - Core logic that handles the three Service Fabric callback types (OnPackageAdded, OnPackageRemoved, OnPackageModified) and aggregates them into a single user callback
2. **`fabric_configuration_package_change_handler_com`** - COM wrapper implementation that exposes the core logic as an `IFabricConfigurationPackageChangeHandler` COM interface
3. **`configuration_package_change_handler`** - High-level convenience module that creates the handler, COM wrapper, and registers with Service Fabric

### Purpose

This architecture provides:
- Clean separation between business logic and COM infrastructure
- Easy unit testing of core logic without COM dependencies
- Reusable COM wrapper that can be tested independently
- Simple high-level API for users who want one-call setup

---

### Unit 1: `fabric_configuration_package_change_handler` (Core Logic)

#### Purpose
Implements the core callback aggregation logic. Takes the three Service Fabric callback types and converts them to a single unified callback.

#### API Design

```c
// fabric_configuration_package_change_handler.h

typedef struct FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG* FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE;

// Callback type for configuration changes
typedef void (*ON_CONFIGURATION_CHANGED)(
    void* context,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previous_config_package,  // NULL for Added
    IFabricConfigurationPackage* new_config_package        // NULL for Removed
);

MOCKABLE_FUNCTION(, FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE,
    fabric_configuration_package_change_handler_create,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);

MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_destroy,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);

// Called by COM wrapper when Service Fabric notifies of package changes
MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_on_package_added,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, configPackage);

MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_on_package_removed,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, configPackage);

MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_on_package_modified,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, previousConfigPackage,
    IFabricConfigurationPackage*, configPackage);
```

#### Internal Structure

```c
typedef struct FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG
{
    ON_CONFIGURATION_CHANGED on_configuration_changed;
    void* context;
} FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER;
```

#### Behavior

- **Create**: Allocates handler, stores callback and context
- **Destroy**: Frees handler
- **on_package_added**: Calls user callback with `previous = NULL`, `new = configPackage`
- **on_package_removed**: Calls user callback with `previous = configPackage`, `new = NULL`
- **on_package_modified**: Calls user callback with both packages

---

### Unit 2: `fabric_configuration_package_change_handler_com` (COM Wrapper)

#### Purpose
Provides the COM wrapper definition that exposes `FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE` as `IFabricConfigurationPackageChangeHandler`.

#### API Design

```c
// fabric_configuration_package_change_handler_com.h

#include "fabric_configuration_package_change_handler.h"
#include "com_wrapper/com_wrapper.h"

#define FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricConfigurationPackageChangeHandler, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_configuration_package_change_handler_on_package_added, \
            IFabricCodePackageActivationContext*, source, \
            IFabricConfigurationPackage*, configPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_configuration_package_change_handler_on_package_removed, \
            IFabricCodePackageActivationContext*, source, \
            IFabricConfigurationPackage*, configPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_configuration_package_change_handler_on_package_modified, \
            IFabricCodePackageActivationContext*, source, \
            IFabricConfigurationPackage*, previousConfigPackage, \
            IFabricConfigurationPackage*, configPackage) \
    )

DECLARE_COM_WRAPPER_OBJECT(FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE_INTERFACES);
```

#### Source File

```c
// fabric_configuration_package_change_handler_com.c
#include "fabric_configuration_package_change_handler_com.h"

DEFINE_COM_WRAPPER_OBJECT(FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE_INTERFACES);
```

---

### Unit 3: `configuration_package_change_handler` (High-Level API)

#### Purpose
Provides a simple one-call interface that creates the core handler, wraps it with COM, and registers with Service Fabric.

#### API Design

```c
// configuration_package_change_handler.h

typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG* CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE;

MOCKABLE_FUNCTION(, CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE,
    configuration_package_change_handler_create,
    IFabricCodePackageActivationContext*, activation_context,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);

MOCKABLE_FUNCTION(, void, configuration_package_change_handler_destroy,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);
```

#### Internal Structure

```c
typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG
{
    IFabricCodePackageActivationContext* activation_context;
    LONGLONG callback_handle;
    IFabricConfigurationPackageChangeHandler* com_handler;
} CONFIGURATION_PACKAGE_CHANGE_HANDLER;
```

#### Behavior

##### Create Function
1. Validate all parameters
2. Allocate handler structure
3. Create `FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE` using `fabric_configuration_package_change_handler_create`
4. Create COM wrapper using `COM_WRAPPER_CREATE`
5. Register with Service Fabric using `activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler`
6. Store callback handle and COM handler reference
7. AddRef on activation_context
8. Return handle on success, NULL on failure with cleanup

##### Destroy Function
1. Unregister using `activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler`
2. Release COM handler (which will destroy the core handler)
3. Release activation_context
4. Free handler structure

---

## File Structure

### Modified Header Files
- `inc/sf_c_util/sf_service_config.h` - Add SRW lock, refresh function, and lock-protected getters

### New Header Files
- `inc/sf_c_util/fabric_configuration_package_change_handler.h` - Core handler API (Unit 1)
- `inc/sf_c_util/fabric_configuration_package_change_handler_com.h` - COM wrapper definition (Unit 2)
- `inc/sf_c_util/configuration_package_change_handler.h` - High-level convenience API (Unit 3)

### New Source Files
- `src/fabric_configuration_package_change_handler.c` - Core handler implementation (Unit 1)
- `src/fabric_configuration_package_change_handler_com.c` - COM wrapper implementation (Unit 2)
- `src/configuration_package_change_handler.c` - High-level API implementation (Unit 3)

### Modified Requirement Documents
- `devdoc/sf_service_config_requirements.md` - Add requirements for SRW lock and refresh

### New Requirement Documents
- `devdoc/fabric_configuration_package_change_handler_requirements.md` - Core handler requirements
- `devdoc/configuration_package_change_handler_requirements.md` - High-level API requirements

### New Test Files
- `tests/fabric_configuration_package_change_handler_ut/` - Unit tests for core handler (Unit 1)
- `tests/configuration_package_change_handler_ut/` - Unit tests for high-level API (Unit 3)

---

## Usage Example

```c
// In header file
#define MY_CONFIG_PARAMS \
    CONFIG_REQUIRED(uint32_t, timeout_ms), \
    CONFIG_OPTIONAL(bool, feature_enabled)

#define MY_CONFIGURATION_PARAMETER_NAME_timeout_ms L"TimeoutMs"
#define MY_CONFIGURATION_PARAMETER_NAME_feature_enabled L"FeatureEnabled"

DECLARE_SF_SERVICE_CONFIG(MY, MY_CONFIG_PARAMS)

// In source file
DEFINE_SF_SERVICE_CONFIG(MY, L"Config", L"MySection", MY_CONFIG_PARAMS)

// Usage
static void on_config_changed(
    void* context,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previous,
    IFabricConfigurationPackage* current)
{
    THANDLE(SF_SERVICE_CONFIG(MY))* config = context;
    
    // Refresh the configuration (thread-safe with SRW lock)
    if (SF_SERVICE_CONFIG_REFRESH(MY)(*config) == 0)
    {
        LogInfo("Configuration refreshed successfully");
    }
    else
    {
        LogError("Failed to refresh configuration");
    }
}

int service_init(IFabricCodePackageActivationContext* activation_context)
{
    // Create configuration (now includes SRW lock internally)
    g_config = SF_SERVICE_CONFIG_CREATE(MY)(activation_context);
    if (g_config == NULL)
    {
        return MU_FAILURE;
    }
    
    // Register for configuration changes
    g_change_handler = configuration_package_change_handler_create(
        activation_context,
        on_config_changed,
        &g_config);
    if (g_change_handler == NULL)
    {
        THANDLE_ASSIGN(SF_SERVICE_CONFIG(MY))(&g_config, NULL);
        return MU_FAILURE;
    }
    
    // Read configuration values (thread-safe with shared lock acquired internally)
    uint32_t timeout = SF_SERVICE_CONFIG_GETTER(MY, timeout_ms)(g_config);
    bool feature = SF_SERVICE_CONFIG_GETTER(MY, feature_enabled)(g_config);
    
    return 0;
}
```

---

## Requirements IDs

All new requirements will use the MrBot ID (88) as specified:
- `SRS_SF_SERVICE_CONFIG_88_xxx` - New requirements for SRW lock and refresh in existing module
- `SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_xxx` - New module

---

## Dependencies

### Existing Dependencies
- `c_pal/srw_lock_ll.h` - SRW lock low-level API
- `c_pal/thandle.h` - THANDLE pattern
- `com_wrapper/com_wrapper.h` - COM wrapper framework
- `sf_c_util/sf_service_config.h` - Base configuration macros
- `fabricruntime.h` - Service Fabric runtime interfaces

### No New External Dependencies
All required functionality is already available in the existing dependencies.

---

## Thread Safety Considerations

### `sf_service_config` (Enhanced)
- **Getters**: Multiple threads can read concurrently (shared lock acquired internally)
- **Refresh**: Single thread can update while readers wait (exclusive lock)
- **Lock Ordering**: Always acquire SRW lock first if combining with other locks
- **Backward Compatibility**: Existing users automatically get thread-safe access

### `configuration_package_change_handler`
- Callbacks from Service Fabric may come from any thread
- Users must ensure their callback implementation is thread-safe
- The handler itself is thread-safe for creation and destruction

### Lifetime Management
- Configuration change handler holds a reference to the activation context
- SRW lock configuration holds references to both activation context and current config
- Users must destroy the change handler before destroying the configuration
- THANDLEs provide automatic cleanup when references reach zero

---

## Testing Strategy

### Unit Tests

#### Enhanced `sf_service_config_ut`
- Create succeeds with valid activation context
- Create fails with NULL activation context  
- Create initializes SRW lock
- Getters return correct values
- Getters fail gracefully with NULL handle
- Getters acquire/release shared lock (mock verification)
- Refresh succeeds and updates values
- Refresh acquires exclusive lock (mock verification)
- Refresh fails gracefully when underlying read fails
- Dispose cleans up all resources including SRW lock

#### `configuration_package_change_handler_ut`
- Create succeeds with valid parameters
- Create fails with NULL activation context
- Create fails with NULL callback
- On package added calls user callback with correct parameters
- On package removed calls user callback with correct parameters
- On package modified calls user callback with correct parameters
- Destroy unregisters from Service Fabric
- Destroy releases activation context

### Integration Tests

- End-to-end test with real Service Fabric activation context
- Verify configuration refresh works with actual SF configuration
- Verify change notifications are received when configuration changes

---

## Implementation Order

1. **Phase 1**: Enhance `sf_service_config` macros with SRW lock and refresh function
   - Add SRW lock to struct, modify getters, add refresh function
   - Update existing unit tests

2. **Phase 2**: Implement `configuration_package_change_handler` and tests
   - Enables dynamic configuration update notification

3. **Phase 3**: Integration testing
   - Verify the complete workflow works with Service Fabric

---

## Open Questions / Future Considerations

1. **Configuration Package Filtering**: Should the change handler filter to only notify for specific configuration package names? (Current design notifies for all packages)

2. **Retry Logic**: Should the refresh function have built-in retry logic on transient failures?

3. **Validation Callback**: Should there be a way to validate new configuration before applying it?

4. **Rollback**: Should there be automatic rollback capability if new configuration is invalid?

These can be addressed in future iterations based on usage experience.
