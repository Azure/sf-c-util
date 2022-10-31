// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_FABRIC_CONFIGURATION_PACKAGE_ARGC_ARGV_H
#define REAL_FABRIC_CONFIGURATION_PACKAGE_ARGC_ARGV_H

#include <stdint.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_FABRIC_CONFIGURATION_PCKAGE_ARGC_ARGV_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        fc_package_create, \
        fc_package_destroy,   \
        get_Path, \
        GetSection, \
        GetValue, \
        DecryptValue \
)

#include "sf_c_util/fc_package.h"

FC_PACKAGE_HANDLE real_fc_package_create(int argc, char** argv, int* argc_consumed);
void real_fc_package_destroy(FC_PACKAGE_HANDLE fc_package_handle);
const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* real_get_Description(FC_PACKAGE_HANDLE fc_package_handle);
LPCWSTR real_get_Path(FC_PACKAGE_HANDLE fc_package_handle);
const FABRIC_CONFIGURATION_SETTINGS* real_get_Settings(FC_PACKAGE_HANDLE fc_package_handle);
HRESULT real_GetSection(FC_PACKAGE_HANDLE fc_package_handle,
    /* [in] */ LPCWSTR sectionName,
    /* [retval][out] */ const FABRIC_CONFIGURATION_SECTION** bufferedValue);
HRESULT real_GetValue(FC_PACKAGE_HANDLE fc_package_handle,
    /* [in] */ LPCWSTR sectionName,
    /* [in] */ LPCWSTR parameterName,
    /* [out] */ BOOLEAN* isEncrypted,
    /* [retval][out] */ LPCWSTR* bufferedValue);
HRESULT real_DecryptValue(FC_PACKAGE_HANDLE fc_package_handle,
    /* [in] */ LPCWSTR encryptedValue,
    /* [retval][out] */ IFabricStringResult** decryptedValue);

#endif //REAL_FABRIC_CONFIGURATION_PACKAGE_ARGC_ARGV_H
