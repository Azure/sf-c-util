# fabric_configuration_package_change_handler requirements

## Overview

`fabric_configuration_package_change_handler` provides the core logic for handling Service Fabric configuration package change notifications. This module:
- Aggregates the three Service Fabric callback types (Added, Removed, Modified) into a single user callback
- Is designed to be wrapped by a COM interface via the companion `_com` module
- Does not manage Service Fabric registration - that is done by the higher-level `configuration_package_change_handler` module

## Exposed API

```c
typedef void (*ON_CONFIGURATION_CHANGED)(
    void* context,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previous_config_package,
    IFabricConfigurationPackage* new_config_package);

typedef struct FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG* FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE;

MOCKABLE_FUNCTION(, FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, fabric_configuration_package_change_handler_create,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);

MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_destroy,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);

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

## fabric_configuration_package_change_handler_create

```c
MOCKABLE_FUNCTION(, FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, fabric_configuration_package_change_handler_create,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);
```

`fabric_configuration_package_change_handler_create` creates a handler that stores the user callback and context.

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_001: [** If `on_configuration_changed` is `NULL`, `fabric_configuration_package_change_handler_create` shall fail and return `NULL`. **]**

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_002: [** `context` is allowed to be `NULL`. **]**

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_003: [** `fabric_configuration_package_change_handler_create` shall allocate memory for the handler structure. **]**

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_004: [** If any error occurs, `fabric_configuration_package_change_handler_create` shall fail and return `NULL`. **]**

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_005: [** `fabric_configuration_package_change_handler_create` shall store the `on_configuration_changed` callback and `context`. **]**

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_006: [** `fabric_configuration_package_change_handler_create` shall succeed and return the handler. **]**

## fabric_configuration_package_change_handler_destroy

```c
MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_destroy,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);
```

`fabric_configuration_package_change_handler_destroy` frees all resources associated with the handler.

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_007: [** If `handle` is `NULL`, `fabric_configuration_package_change_handler_destroy` shall return. **]**

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_008: [** `fabric_configuration_package_change_handler_destroy` shall free the handler structure. **]**

## fabric_configuration_package_change_handler_on_package_added

```c
MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_on_package_added,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, configPackage);
```

`fabric_configuration_package_change_handler_on_package_added` is called when a configuration package is added. This notification is ignored.

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_009: [** `fabric_configuration_package_change_handler_on_package_added` shall do nothing. **]**

## fabric_configuration_package_change_handler_on_package_removed

```c
MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_on_package_removed,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, configPackage);
```

`fabric_configuration_package_change_handler_on_package_removed` is called when a configuration package is removed. This notification is ignored.

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_010: [** `fabric_configuration_package_change_handler_on_package_removed` shall do nothing. **]**

## fabric_configuration_package_change_handler_on_package_modified

```c
MOCKABLE_FUNCTION(, void, fabric_configuration_package_change_handler_on_package_modified,
    FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, previousConfigPackage,
    IFabricConfigurationPackage*, configPackage);
```

`fabric_configuration_package_change_handler_on_package_modified` is called when a configuration package is modified.

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_011: [** If `handle` is `NULL`, `fabric_configuration_package_change_handler_on_package_modified` shall return. **]**

**SRS_FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_88_012: [** `fabric_configuration_package_change_handler_on_package_modified` shall call the `on_configuration_changed` callback with `previous_config_package` set to `previousConfigPackage` and `new_config_package` set to `configPackage`. **]**
