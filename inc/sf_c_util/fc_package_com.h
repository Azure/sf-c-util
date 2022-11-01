// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef FC_PACKAGE_COM_H
#define FC_PACKAGE_COM_H

#include "windows.h"
#include "fabriccommon.h"
#include "unknwn.h"

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/fc_package.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FC_PACKAGE_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricConfigurationPackage, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION*, IFabricConfigurationPackage_get_Description ),\
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, IFabricConfigurationPackage_get_Path),                                                \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_CONFIGURATION_SETTINGS*, IFabricConfigurationPackage_get_Settings),               \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, IFabricConfigurationPackage_GetSection,                                               \
            /* [in] */ LPCWSTR, sectionName,                                                                                        \
            /* [retval][out] */ const FABRIC_CONFIGURATION_SECTION**, bufferedValue),                                               \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, IFabricConfigurationPackage_GetValue,                                                 \
            /* [in] */ LPCWSTR, sectionName,                                                                                        \
            /* [in] */ LPCWSTR, parameterName,                                                                                      \
            /* [out] */ BOOLEAN*, isEncrypted,                                                                                      \
            /* [retval][out] */ LPCWSTR*, bufferedValue),                                                                           \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, IFabricConfigurationPackage_DecryptValue,                                             \
            /* [in] */ LPCWSTR, encryptedValue,                                                                                     \
            /* [retval][out] */ IFabricStringResult**, decryptedValue)                                                              \
    )

    DECLARE_COM_WRAPPER_OBJECT(FC_PACKAGE_HANDLE, FC_PACKAGE_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FC_PACKAGE_COM_H */
