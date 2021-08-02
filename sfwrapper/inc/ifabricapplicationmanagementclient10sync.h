// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef IFABRICAPPLICATIONMANAGEMENTCLIENT10SYNC_H
#define IFABRICAPPLICATIONMANAGEMENTCLIENT10SYNC_H

#include "windows.h"

#include "fabricclient.h"

#include "sf_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

    HRESULT FAMC10_ProvisionApplicationType(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ LPCWSTR applicationBuildPath,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_CreateApplication(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_APPLICATION_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_UpgradeApplication(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_APPLICATION_UPGRADE_DESCRIPTION *upgradeDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_GetApplicationUpgradeProgress(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ FABRIC_URI applicationName,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricApplicationUpgradeProgressResult2 **result);

    HRESULT FAMC10_MoveNextApplicationUpgradeDomain(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ IFabricApplicationUpgradeProgressResult2 *progress,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_DeleteApplication(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ FABRIC_URI applicationName,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_UnprovisionApplicationType(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ LPCWSTR applicationTypeName,
        /* [in] */ LPCWSTR applicationTypeVersion,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_GetApplicationManifest(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ LPCWSTR applicationTypeName,
        /* [in] */ LPCWSTR applicationTypeVersion,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricStringResult **result);

    HRESULT FAMC10_MoveNextApplicationUpgradeDomain2(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ FABRIC_URI applicationName,
        /* [in] */ LPCWSTR nextUpgradeDomain,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_UpdateApplicationUpgrade(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_RestartDeployedCodePackage(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION *restartCodePackageDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_CopyApplicationPackage(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ LPCWSTR imageStoreConnectionString,
        /* [in] */ LPCWSTR applicationPackagePath,
        /* [in] */ LPCWSTR applicationPackagePathInImageStore);

    HRESULT FAMC10_RemoveApplicationPackage(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ LPCWSTR imageStoreConnectionString,
        /* [in] */ LPCWSTR applicationPackagePathInImageStore);

    HRESULT FAMC10_DeployServicePackageToNode(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ LPCWSTR applicationTypeName,
        /* [in] */ LPCWSTR applicationTypeVersion,
        /* [in] */ LPCWSTR serviceManifestName,
        /* [in] */ const FABRIC_PACKAGE_SHARING_POLICY_LIST *sharingPolicy,
        /* [in] */ LPCWSTR nodeName,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_RollbackApplicationUpgrade(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ FABRIC_URI applicationName,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_UpdateApplication(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_APPLICATION_UPDATE_DESCRIPTION *applicationUpdateDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_DeleteApplication2(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_DELETE_APPLICATION_DESCRIPTION *deleteDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_ProvisionApplicationType2(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_UnprovisionApplicationType2(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FAMC10_ProvisionApplicationType3(
        IFabricApplicationManagementClient10 * This,
        /* [in] */ const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE *description,
        /* [in] */ DWORD timeoutMilliseconds);

    DECLARE_CREATE_IFABRICINSTANCE(IFabricApplicationManagementClient10)

#ifdef __cplusplus
}
#endif

#endif /*IFABRICAPPLICATIONMANAGEMENTCLIENT10SYNC_H*/
