// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_COM_H
#define FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_COM_H

#include "windows.h"
#include "fabricruntime.h"
#include "unknwn.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/fabric_configuration_package_change_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricConfigurationPackageChangeHandler, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_configuration_package_change_handler_on_package_added, IFabricCodePackageActivationContext*, source, IFabricConfigurationPackage*, configPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_configuration_package_change_handler_on_package_removed, IFabricCodePackageActivationContext*, source, IFabricConfigurationPackage*, configPackage), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, fabric_configuration_package_change_handler_on_package_modified, IFabricCodePackageActivationContext*, source, IFabricConfigurationPackage*, previousConfigPackage, IFabricConfigurationPackage*, configPackage) \
    )

DECLARE_COM_WRAPPER_OBJECT(FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE, FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_COM_H */
