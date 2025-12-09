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

## Component 2: `configuration_package_change_handler`

### Purpose

Provides a simplified interface to receive notifications when Service Fabric configuration packages change. This component:
- Implements `IFabricConfigurationPackageChangeHandler` COM interface
- Aggregates the three callback types (Added, Removed, Modified) into a single user callback
- Manages registration/unregistration with Service Fabric

### API Design

```c
// configuration_package_change_handler.h

typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG* CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE;

// Callback type for configuration changes
typedef void (*ON_CONFIGURATION_CHANGED)(
    void* context,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previous_config_package,  // NULL for Added
    IFabricConfigurationPackage* new_config_package        // NULL for Removed
);

MOCKABLE_FUNCTION(, CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, 
    configuration_package_change_handler_create,
    IFabricCodePackageActivationContext*, activation_context,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);

MOCKABLE_FUNCTION(, void, configuration_package_change_handler_destroy,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);
```

### Internal Structure

```c
typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG
{
    IFabricCodePackageActivationContext* activation_context;
    LONGLONG callback_handle;
    ON_CONFIGURATION_CHANGED on_configuration_changed;
    void* context;
} CONFIGURATION_PACKAGE_CHANGE_HANDLER;
```

### COM Interface Implementation

The module will implement `IFabricConfigurationPackageChangeHandler` using the `com-wrapper` pattern:

```c
// configuration_package_change_handler_com.h

#define CONFIGURATION_PACKAGE_CHANGE_HANDLER_COM_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricConfigurationPackageChangeHandler, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, configuration_package_change_handler_on_package_added, \
            IFabricCodePackageActivationContext*, source, \
            IFabricConfigurationPackage*, configPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, configuration_package_change_handler_on_package_removed, \
            IFabricCodePackageActivationContext*, source, \
            IFabricConfigurationPackage*, configPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, configuration_package_change_handler_on_package_modified, \
            IFabricCodePackageActivationContext*, source, \
            IFabricConfigurationPackage*, previousConfigPackage, \
            IFabricConfigurationPackage*, configPackage) \
    )

DECLARE_COM_WRAPPER_OBJECT(CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, CONFIGURATION_PACKAGE_CHANGE_HANDLER_COM_INTERFACES);
```

### Behavior

#### Create Function
1. Validate all parameters
2. Allocate handler structure
3. Store activation_context (AddRef)
4. Store callback and context
5. Create COM wrapper
6. Register with Service Fabric using `IFabricCodePackageActivationContext::RegisterConfigurationPackageChangeHandler`
7. Store returned callback handle
8. Return handle on success, NULL on failure

#### COM Callback Functions
Each callback (OnPackageAdded, OnPackageRemoved, OnPackageModified) will:
1. Call the user's `ON_CONFIGURATION_CHANGED` callback with appropriate parameters:
   - **Added**: `previous_config_package = NULL`, `new_config_package = configPackage`
   - **Removed**: `previous_config_package = configPackage`, `new_config_package = NULL`
   - **Modified**: `previous_config_package = previousConfigPackage`, `new_config_package = configPackage`

#### Destroy Function
1. Unregister using `IFabricCodePackageActivationContext::UnregisterConfigurationPackageChangeHandler`
2. Release activation_context
3. Free handler structure

---

## File Structure

### Modified Header Files
- `inc/sf_c_util/sf_service_config.h` - Add SRW lock, refresh function, and lock-protected getters

### New Header Files
- `inc/sf_c_util/configuration_package_change_handler.h` - Change handler API
- `inc/sf_c_util/configuration_package_change_handler_com.h` - COM wrapper definition

### New Source Files
- `src/configuration_package_change_handler.c` - Change handler implementation
- `src/configuration_package_change_handler_com.c` - COM wrapper implementation

### Modified Requirement Documents
- `devdoc/sf_service_config_requirements.md` - Add requirements for SRW lock and refresh

### New Requirement Documents
- `devdoc/configuration_package_change_handler_requirements.md`

### New Test Files
- `tests/configuration_package_change_handler_ut/` - Unit tests for change handler

### Modified Test Files
- `tests/sf_service_config_ut/` - Add tests for refresh function and lock behavior

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
