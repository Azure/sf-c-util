# configuration_package_change_handler requirements

## Overview

`configuration_package_change_handler` provides a simplified interface to receive notifications when Service Fabric configuration packages change. This module:
- Implements `IFabricConfigurationPackageChangeHandler` COM interface
- Aggregates the three callback types (Added, Removed, Modified) into a single user callback
- Manages registration/unregistration with Service Fabric

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

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [** `configuration_package_change_handler_create` shall store the `on_configuration_changed` callback and `context`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [** `configuration_package_change_handler_create` shall create a COM wrapper for `IFabricConfigurationPackageChangeHandler` using `COM_WRAPPER_CREATE`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [** `configuration_package_change_handler_create` shall register the COM handler with Service Fabric by calling `activation_context->lpVtbl->RegisterConfigurationPackageChangeHandler`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [** `configuration_package_change_handler_create` shall store the callback handle returned by `RegisterConfigurationPackageChangeHandler`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [** `configuration_package_change_handler_create` shall call `AddRef` on `activation_context` to take a reference. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [** `configuration_package_change_handler_create` shall succeed and return the handler. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [** If any error occurs, `configuration_package_change_handler_create` shall fail and return `NULL`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [** If `RegisterConfigurationPackageChangeHandler` fails, `configuration_package_change_handler_create` shall release the COM wrapper and free resources. **]**

## configuration_package_change_handler_destroy

```c
MOCKABLE_FUNCTION(, void, configuration_package_change_handler_destroy,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);
```

`configuration_package_change_handler_destroy` unregisters from Service Fabric notifications and frees all resources.

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_013: [** If `handle` is `NULL`, `configuration_package_change_handler_destroy` shall return. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_014: [** `configuration_package_change_handler_destroy` shall unregister from Service Fabric by calling `activation_context->lpVtbl->UnregisterConfigurationPackageChangeHandler` with the stored callback handle. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_015: [** `configuration_package_change_handler_destroy` shall call `Release` on the stored `activation_context`. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_016: [** `configuration_package_change_handler_destroy` shall free the handler structure. **]**

## configuration_package_change_handler_on_package_added

```c
void configuration_package_change_handler_on_package_added(
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* configPackage);
```

`configuration_package_change_handler_on_package_added` is called by Service Fabric when a configuration package is added.

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_017: [** If `handle` is `NULL`, `configuration_package_change_handler_on_package_added` shall return. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_018: [** `configuration_package_change_handler_on_package_added` shall call the `on_configuration_changed` callback with `previous_config_package` set to `NULL` and `new_config_package` set to `configPackage`. **]**

## configuration_package_change_handler_on_package_removed

```c
void configuration_package_change_handler_on_package_removed(
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* configPackage);
```

`configuration_package_change_handler_on_package_removed` is called by Service Fabric when a configuration package is removed.

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_019: [** If `handle` is `NULL`, `configuration_package_change_handler_on_package_removed` shall return. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_020: [** `configuration_package_change_handler_on_package_removed` shall call the `on_configuration_changed` callback with `previous_config_package` set to `configPackage` and `new_config_package` set to `NULL`. **]**

## configuration_package_change_handler_on_package_modified

```c
void configuration_package_change_handler_on_package_modified(
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE handle,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previousConfigPackage,
    IFabricConfigurationPackage* configPackage);
```

`configuration_package_change_handler_on_package_modified` is called by Service Fabric when a configuration package is modified.

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_021: [** If `handle` is `NULL`, `configuration_package_change_handler_on_package_modified` shall return. **]**

**SRS_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_022: [** `configuration_package_change_handler_on_package_modified` shall call the `on_configuration_changed` callback with `previous_config_package` set to `previousConfigPackage` and `new_config_package` set to `configPackage`. **]**
