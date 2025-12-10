// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef CONFIGURATION_PACKAGE_CHANGE_HANDLER_H
#define CONFIGURATION_PACKAGE_CHANGE_HANDLER_H

#include "windows.h"

#include "fabricruntime.h"

#include "umock_c/umock_c_prod.h"

#include "sf_c_util/fabric_configuration_package_change_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CONFIGURATION_PACKAGE_CHANGE_HANDLER_TAG* CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE;

MOCKABLE_FUNCTION(, CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, configuration_package_change_handler_create,
    IFabricCodePackageActivationContext*, activation_context,
    ON_CONFIGURATION_CHANGED, on_configuration_changed,
    void*, context);

MOCKABLE_FUNCTION(, void, configuration_package_change_handler_destroy,
    CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, handle);

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_PACKAGE_CHANGE_HANDLER_H */

