// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_H
#define FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_H

#include "windows.h"

#include "fabricruntime.h"

#include "umock_c/umock_c_prod.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_H */
