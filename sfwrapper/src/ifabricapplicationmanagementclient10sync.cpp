// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "ifabricapplicationmanagementclient10sync.h"

#include "servicefabricdox.h"

HRESULT FAMC10_ProvisionApplicationType(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationBuildPath,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginProvisionApplicationType, &IFabricApplicationManagementClient10::EndProvisionApplicationType,
        timeoutMilliseconds,
        applicationBuildPath);
}

HRESULT FAMC10_CreateApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginCreateApplication, &IFabricApplicationManagementClient10::EndCreateApplication,
        timeoutMilliseconds,
        description);
}

HRESULT FAMC10_UpgradeApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_UPGRADE_DESCRIPTION *upgradeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUpgradeApplication, &IFabricApplicationManagementClient10::EndUpgradeApplication,
        timeoutMilliseconds,
        upgradeDescription);
}

HRESULT FAMC10_GetApplicationUpgradeProgress(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricApplicationUpgradeProgressResult2 **result)
{
    return ServiceFabric_DoX(This, &IFabricApplicationManagementClient10::BeginGetApplicationUpgradeProgress, &IFabricApplicationManagementClient10::EndGetApplicationUpgradeProgress,
        timeoutMilliseconds,
        result,
        applicationName
        );
}

HRESULT FAMC10_MoveNextApplicationUpgradeDomain(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ IFabricApplicationUpgradeProgressResult2 *progress,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginMoveNextApplicationUpgradeDomain, &IFabricApplicationManagementClient10::EndMoveNextApplicationUpgradeDomain,
        timeoutMilliseconds,
        progress);
}

HRESULT FAMC10_DeleteApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginDeleteApplication, &IFabricApplicationManagementClient10::EndDeleteApplication,
        timeoutMilliseconds,
        applicationName);
}

HRESULT FAMC10_UnprovisionApplicationType(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUnprovisionApplicationType, &IFabricApplicationManagementClient10::EndUnprovisionApplicationType,
        timeoutMilliseconds,
        applicationTypeName,
        applicationTypeVersion
    );
}

HRESULT FAMC10_GetApplicationManifest(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    return ServiceFabric_DoX(This, &IFabricApplicationManagementClient10::BeginGetApplicationManifest, &IFabricApplicationManagementClient10::EndGetApplicationManifest,
        timeoutMilliseconds,
        result,
        applicationTypeName,
        applicationTypeVersion
    );
}

HRESULT FAMC10_MoveNextApplicationUpgradeDomain2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ LPCWSTR nextUpgradeDomain,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginMoveNextApplicationUpgradeDomain2, &IFabricApplicationManagementClient10::EndMoveNextApplicationUpgradeDomain2,
        timeoutMilliseconds,
        applicationName,
        nextUpgradeDomain
    );
}

HRESULT FAMC10_UpdateApplicationUpgrade(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUpdateApplicationUpgrade, &IFabricApplicationManagementClient10::EndUpdateApplicationUpgrade,
        timeoutMilliseconds,
        description
    );
}

HRESULT FAMC10_RestartDeployedCodePackage(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION *restartCodePackageDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginRestartDeployedCodePackage, &IFabricApplicationManagementClient10::EndRestartDeployedCodePackage,
        timeoutMilliseconds,
        restartCodePackageDescription
    );
}

HRESULT FAMC10_CopyApplicationPackage(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR applicationPackagePath,
    /* [in] */ LPCWSTR applicationPackagePathInImageStore)
{
    return This->CopyApplicationPackage(imageStoreConnectionString, applicationPackagePath, applicationPackagePathInImageStore);
}

HRESULT FAMC10_RemoveApplicationPackage(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR applicationPackagePathInImageStore)
{
    return This->RemoveApplicationPackage(imageStoreConnectionString, applicationPackagePathInImageStore);
}

HRESULT FAMC10_DeployServicePackageToNode(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ LPCWSTR serviceManifestName,
    /* [in] */ const FABRIC_PACKAGE_SHARING_POLICY_LIST *sharingPolicy,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginDeployServicePackageToNode, &IFabricApplicationManagementClient10::EndDeployServicePackageToNode,
        timeoutMilliseconds,
        applicationTypeName,
        applicationTypeVersion,
        serviceManifestName,
        sharingPolicy,
        nodeName
    );
}

HRESULT FAMC10_RollbackApplicationUpgrade(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginRollbackApplicationUpgrade, &IFabricApplicationManagementClient10::EndRollbackApplicationUpgrade,
        timeoutMilliseconds,
        applicationName
    );
}

HRESULT FAMC10_UpdateApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_UPDATE_DESCRIPTION *applicationUpdateDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUpdateApplication, &IFabricApplicationManagementClient10::EndUpdateApplication,
        timeoutMilliseconds,
        applicationUpdateDescription
    );
}

HRESULT FAMC10_DeleteApplication2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_DELETE_APPLICATION_DESCRIPTION *deleteDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginDeleteApplication2, &IFabricApplicationManagementClient10::EndDeleteApplication2,
        timeoutMilliseconds,
        deleteDescription
    );
}

HRESULT FAMC10_ProvisionApplicationType2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginProvisionApplicationType2, &IFabricApplicationManagementClient10::EndProvisionApplicationType2,
        timeoutMilliseconds,
        description
    );
}

HRESULT FAMC10_UnprovisionApplicationType2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUnprovisionApplicationType2, &IFabricApplicationManagementClient10::EndUnprovisionApplicationType2,
        timeoutMilliseconds,
        description
    );
}

HRESULT FAMC10_ProvisionApplicationType3(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginProvisionApplicationType3, &IFabricApplicationManagementClient10::EndProvisionApplicationType3,
        timeoutMilliseconds,
        description
    );
}
