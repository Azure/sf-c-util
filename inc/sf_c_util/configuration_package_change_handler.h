// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef CONFIGURATION_PACKAGE_CHANGE_HANDLER_H
#define CONFIGURATION_PACKAGE_CHANGE_HANDLER_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "windows.h"

#include "fabricruntime.h"

#include "umock_c/umock_c_prod.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback invoked when configuration package changes are detected.
 *
 * This callback is called when the configuration package is added, removed, or modified.
 * The callback receives the source activation context and the configuration packages.
 *
 * @param context                   User-provided context passed during handler creation.
 * @param source                    The activation context that raised the event.
 * @param previous_config_package   The previous configuration package (NULL for Added events).
 * @param new_config_package        The new configuration package (NULL for Removed events).
 */
typedef void (*ON_CONFIGURATION_CHANGED)(
    void* context,
    IFabricCodePackageActivationContext* source,
    IFabricConfigurationPackage* previous_config_package,
    IFabricConfigurationPackage* new_config_package);

/**
 * @brief Handle to a configuration package change handler.
 *
 * This handler registers with Service Fabric to receive notifications when configuration
 * packages change. When a change is detected (package added, removed, or modified), the
 * user-provided callback is invoked.
 */
typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG* CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE;

/**
 * @brief Creates a configuration package change handler.
 *
 * Creates a handler that will receive notifications from Service Fabric when a
 * configuration package changes (added, removed, or modified).
 *
 * @param activation_context        The Service Fabric activation context to register with.
 * @param on_configuration_changed  Callback function to invoke when configuration changes.
 * @param context                   User-provided context passed to the callback.
 *
 * @return A handle to the configuration package change handler on success, or NULL on failure.
 */
MOCKABLE_FUNCTION(, CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, configuration_package_change_handler_create,
    IFabricCodePackageActivationContext*, activation_context,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);

/**
 * @brief Destroys a configuration package change handler.
 *
 * Unregisters from Service Fabric notifications and frees all resources associated
 * with the handler.
 *
 * @param handle    The handle to destroy.
 */
MOCKABLE_FUNCTION(, void, configuration_package_change_handler_destroy,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);

/**
 * @brief Called when a configuration package is added.
 *
 * This function is called by the COM wrapper when Service Fabric notifies that
 * a new configuration package has been added. It invokes the user callback with
 * previous_config_package set to NULL.
 *
 * @param handle            The configuration package change handler.
 * @param source            The activation context that raised the event.
 * @param configPackage     The configuration package that was added.
 */
MOCKABLE_FUNCTION(, void, configuration_package_change_handler_on_package_added,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, configPackage);

/**
 * @brief Called when a configuration package is removed.
 *
 * This function is called by the COM wrapper when Service Fabric notifies that
 * a configuration package has been removed. It invokes the user callback with
 * new_config_package set to NULL.
 *
 * @param handle            The configuration package change handler.
 * @param source            The activation context that raised the event.
 * @param configPackage     The configuration package that was removed.
 */
MOCKABLE_FUNCTION(, void, configuration_package_change_handler_on_package_removed,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, configPackage);

/**
 * @brief Called when a configuration package is modified.
 *
 * This function is called by the COM wrapper when Service Fabric notifies that
 * a configuration package has been modified. It invokes the user callback with
 * both the previous and new configuration packages.
 *
 * @param handle                    The configuration package change handler.
 * @param source                    The activation context that raised the event.
 * @param previousConfigPackage     The previous configuration package.
 * @param configPackage             The new configuration package.
 */
MOCKABLE_FUNCTION(, void, configuration_package_change_handler_on_package_modified,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle,
    IFabricCodePackageActivationContext*, source,
    IFabricConfigurationPackage*, previousConfigPackage,
    IFabricConfigurationPackage*, configPackage);

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_PACKAGE_CHANGE_HANDLER_H */
