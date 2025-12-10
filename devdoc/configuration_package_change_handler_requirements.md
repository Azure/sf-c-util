# configuration_package_change_handler requirements

## Overview

`configuration_package_change_handler` is the high-level API module that provides a simplified interface to receive notifications when Service Fabric configuration packages change. This module:
- Creates the underlying `fabric_configuration_package_change_handler`
- Creates the COM wrapper using `fabric_configuration_package_change_handler_com`
- Registers the handler with Service Fabric via the activation context
- Manages the lifecycle and cleanup of all components

This module orchestrates the other two components:
- `fabric_configuration_package_change_handler` - core handler logic
- `fabric_configuration_package_change_handler_com` - COM wrapper definitions

## Exposed API

```c
typedef void (*ON_CONFIGURATION_CHANGED)(
    void* context,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previous_config_package,
    IFabricConfigurationPackage* new_config_package);

typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG* CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE;

MOCKABLE_FUNCTION(, CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, configuration_package_change_handler_create,
    IFabricCodePackageActivationContext*, activation_context,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);

MOCKABLE_FUNCTION(, void, configuration_package_change_handler_destroy,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);
```

## configuration_package_change_handler_create

```c
MOCKABLE_FUNCTION(, CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, configuration_package_change_handler_create,
    IFabricCodePackageActivationContext*, activation_context,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);
```

`configuration_package_change_handler_create` creates a handler that registers with Service Fabric to receive configuration package change notifications.

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_001: [** If `activation_context` is `NULL`, `configuration_package_change_handler_create` shall fail and return `NULL`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_002: [** If `on_configuration_changed` is `NULL`, `configuration_package_change_handler_create` shall fail and return `NULL`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_003: [** `context` is allowed to be `NULL`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_004: [** `configuration_package_change_handler_create` shall allocate memory for the handler structure. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [** `configuration_package_change_handler_create` shall call `fabric_configuration_package_change_handler_create` with the `on_configuration_changed` callback and `context` to create the core handler. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [** If `fabric_configuration_package_change_handler_create` fails, `configuration_package_change_handler_create` shall free the allocated memory and return `NULL`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [** `configuration_package_change_handler_create` shall create a COM wrapper for `IFabricConfigurationPackageChangeHandler` using `COM_WRAPPER_CREATE` with the `FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [** If `COM_WRAPPER_CREATE` fails, `configuration_package_change_handler_create` shall destroy the fabric handler, free the allocated memory, and return `NULL`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [** `configuration_package_change_handler_create` shall call `AddRef` on `activation_context` to take a reference. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [** `configuration_package_change_handler_create` shall register the COM handler with Service Fabric by calling `activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [** If `RegisterConfigurationPackageChangeHandler` fails, `configuration_package_change_handler_create` shall release the COM wrapper, call `Release` on the activation context, and free all resources. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [** `configuration_package_change_handler_create` shall store the callback handle returned by `RegisterConfigurationPackageChangeHandler`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_013: [** `configuration_package_change_handler_create` shall succeed and return the handler. **]**

## configuration_package_change_handler_destroy

```c
MOCKABLE_FUNCTION(, void, configuration_package_change_handler_destroy,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);
```

`configuration_package_change_handler_destroy` unregisters from Service Fabric notifications and frees all resources.

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_014: [** If `handle` is `NULL`, `configuration_package_change_handler_destroy` shall return. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_015: [** `configuration_package_change_handler_destroy` shall unregister from Service Fabric by calling `activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler` with the stored callback handle. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_016: [** `configuration_package_change_handler_destroy` shall call `Release` on the COM wrapper (which will trigger destruction of the underlying fabric handler). **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_017: [** `configuration_package_change_handler_destroy` shall call `Release` on the stored `activation_context`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_018: [** `configuration_package_change_handler_destroy` shall free the handler structure. **]**
