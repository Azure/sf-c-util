// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef FC_PACKAGE_H
#define FC_PACKAGE_H


#include "windows.h"
#include "fabriccommon.h"
#include "fabricruntime.h"
#include "fabrictypes.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct FC_PACKAGE_TAG* FC_PACKAGE_HANDLE;

    /*a FABRIC_CONFIGURATION_PACKAGE is serialized to argc/argv as --configurationPackageName "name" followed by 1x FABRIC_CONFIGURATION_SECTION_LIST*/

    MOCKABLE_INTERFACE(fc_package,
        FUNCTION(, FC_PACKAGE_HANDLE, fc_package_create, int, argc, char**, argv, int*, argc_consumed),
        FUNCTION(, void, fc_package_destroy, FC_PACKAGE_HANDLE, fc_package_handle),
        FUNCTION(, const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION*, get_Description, FC_PACKAGE_HANDLE, fc_package_handle),
        FUNCTION(, LPCWSTR, get_Path, FC_PACKAGE_HANDLE, fc_package_handle),
        FUNCTION(, const FABRIC_CONFIGURATION_SETTINGS*, get_Settings, FC_PACKAGE_HANDLE, fc_package_handle),
        FUNCTION(, HRESULT, GetSection, FC_PACKAGE_HANDLE, fc_package_handle,
            /* [in] */ LPCWSTR, sectionName,
            /* [retval][out] */ const FABRIC_CONFIGURATION_SECTION**, bufferedValue),
        FUNCTION(, HRESULT, GetValue, FC_PACKAGE_HANDLE, fc_package_handle,
            /* [in] */ LPCWSTR, sectionName,
            /* [in] */ LPCWSTR, parameterName,
            /* [out] */ BOOLEAN*, isEncrypted,
            /* [retval][out] */ LPCWSTR*, bufferedValue),
        FUNCTION(, HRESULT, DecryptValue, FC_PACKAGE_HANDLE, fc_package_handle,
            /* [in] */ LPCWSTR, encryptedValue,
            /* [retval][out] */ IFabricStringResult**, decryptedValue)
    )

    /* FABRIC_CONFIGURATION_PACKAGE => argc/argv */
    MOCKABLE_FUNCTION(, int, IFabricConfigurationPackage_to_ARGC_ARGV, IFabricConfigurationPackage*, iFabricConfigurationPackage, int*, argc, char***, argv);

    /*argc/argv = > IFabricConfigurationPackage * sort of "factory" :). Handled by fc_create above in MOCKABLE_INTERFACE(fc_package,... */
    /*freeing a previously produced IFabricConfigurationPackage* => done by COM means, it ends up eventually calling fc_package_destroy */

#ifdef __cplusplus
}
#endif

#endif /* FC_PACKAGE_H */
