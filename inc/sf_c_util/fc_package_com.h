// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef FC_PACKAGE_COM_H
#define FC_PACKAGE_COM_H

#include "windows.h"
#include "fabriccommon.h"
#include "unknwn.h"

#include "com_wrapper/com_wrapper.h"

#include "fc_package.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FC_PACKAGE_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IFabricConfigurationPackage, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION*, get_Description ),\
        COM_WRAPPER_FUNCTION_WRAPPER(LPCWSTR, get_Path),                                                \
        COM_WRAPPER_FUNCTION_WRAPPER(const FABRIC_CONFIGURATION_SETTINGS*, get_Settings),               \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetSection,                                               \
            /* [in] */ LPCWSTR, sectionName,                                                            \
            /* [retval][out] */ const FABRIC_CONFIGURATION_SECTION**, bufferedValue),                   \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, GetValue,                                                 \
            /* [in] */ LPCWSTR, sectionName,                                                            \
            /* [in] */ LPCWSTR, parameterName,                                                          \
            /* [out] */ BOOLEAN*, isEncrypted,                                                          \
            /* [retval][out] */ LPCWSTR*, bufferedValue),                                               \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, DecryptValue,                                             \
            /* [in] */ LPCWSTR, encryptedValue,                                                         \
            /* [retval][out] */ IFabricStringResult**, decryptedValue)                                  \
    )

    DECLARE_COM_WRAPPER_OBJECT(FC_PACKAGE_HANDLE, FC_PACKAGE_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* FC_PACKAGE_COM_H */
