// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef H_FABRIC_APPLICATION_MANAGEMENT_CLIENT10_H
#define H_FABRIC_APPLICATION_MANAGEMENT_CLIENT10_H

#include "windows.h"

#include "fabricclient.h"

#include "macro_utils/macro_utils.h"

#include "h_fabric_macro_generator.h"

H_FABRIC_DEFINE_TYPEDEF(IFabricApplicationManagementClient10);

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    H_FABRIC_DECLARE_CREATE(IFabricApplicationManagementClient10);
    H_FABRIC_DECLARE_DESTROY(IFabricApplicationManagementClient10);

    /*below are the same API as ifabricapplicationclient10sync exposes, which are the same as the methods of the IFabricApplicationManagementClient10 interface*/
    /*note: the computed outputs (such as IFabricGetNodeListResult are "as they come" from IFabricApplicationManagementClient10 interface*/

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_ProvisionApplicationType,
        IN_ARGS(
            ARG(LPCWSTR, applicationBuildPath),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_CreateApplication,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_UpgradeApplication,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_UPGRADE_DESCRIPTION*, upgradeDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_GetApplicationUpgradeProgress,
        IN_ARGS(
            ARG(FABRIC_URI, applicationName),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricApplicationUpgradeProgressResult2**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_MoveNextApplicationUpgradeDomain,
        IN_ARGS(
            ARG(IFabricApplicationUpgradeProgressResult2*, progress),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_DeleteApplication,
        IN_ARGS(
            ARG(FABRIC_URI, applicationName),
            ARG(DWORD, timeoutMilliseconds)
            )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_UnprovisionApplicationType,
        IN_ARGS(
            ARG(LPCWSTR, applicationTypeName),
            ARG(LPCWSTR, applicationTypeVersion),
            ARG(DWORD, timeoutMilliseconds)
            )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_GetApplicationManifest,
        IN_ARGS(
            ARG(LPCWSTR, applicationTypeName),
            ARG(LPCWSTR, applicationTypeVersion),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStringResult**, result)
            )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_MoveNextApplicationUpgradeDomain2,
        IN_ARGS(
            ARG(LPCWSTR, applicationName),
            ARG(LPCWSTR, nextUpgradeDomain),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_UpdateApplicationUpgrade,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_RestartDeployedCodePackage,
        IN_ARGS(
            ARG(const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION*, restartCodePackageDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_CopyApplicationPackage,
        IN_ARGS(
            ARG(LPCWSTR, imageStoreConnectionString),
            ARG(LPCWSTR, applicationPackagePath),
            ARG(LPCWSTR, applicationPackagePathInImageStore)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_RemoveApplicationPackage,
        IN_ARGS(
            ARG(LPCWSTR, imageStoreConnectionString),
            ARG(LPCWSTR, applicationPackagePathInImageStore)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_DeployServicePackageToNode,
        IN_ARGS(
            ARG(LPCWSTR, applicationTypeName),
            ARG(LPCWSTR, applicationTypeVersion),
            ARG(LPCWSTR, serviceManifestName),
            ARG(const FABRIC_PACKAGE_SHARING_POLICY_LIST*, sharingPolicy),
            ARG(LPCWSTR, nodeName),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_RollbackApplicationUpgrade,
        IN_ARGS(
            ARG(FABRIC_URI, applicationName),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_UpdateApplication,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_UPDATE_DESCRIPTION*, applicationUpdateDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_DeleteApplication2,
        IN_ARGS(
            ARG(const FABRIC_DELETE_APPLICATION_DESCRIPTION*, deleteDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_ProvisionApplicationType2,
        IN_ARGS(
            ARG(const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_UnprovisionApplicationType2,
        IN_ARGS(
            ARG(const FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricApplicationManagementClient10, FAMC10_ProvisionApplicationType3,
        IN_ARGS(
            ARG(const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

#ifdef __cplusplus
}
#endif

#endif /*H_FABRIC_APPLICATION_MANAGEMENT_CLIENT10_H*/
