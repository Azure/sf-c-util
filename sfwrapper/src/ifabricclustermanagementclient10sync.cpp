// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "ifabricclustermanagementclient10sync.h"

#include "servicefabricdox.h"

HRESULT FCMC10_NodeStateRemoved(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginNodeStateRemoved, &IFabricClusterManagementClient10::EndNodeStateRemoved,
        timeoutMilliseconds,
        nodeName);
}

HRESULT FCMC10_RecoverPartitions(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverPartitions, &IFabricClusterManagementClient10::EndRecoverPartitions,
        timeoutMilliseconds);
}

HRESULT FCMC10_DeactivateNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ FABRIC_NODE_DEACTIVATION_INTENT intent,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginDeactivateNode, &IFabricClusterManagementClient10::EndDeactivateNode,
        timeoutMilliseconds,
        nodeName,
        intent);
}

HRESULT FCMC10_ActivateNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginActivateNode, &IFabricClusterManagementClient10::EndActivateNode,
        timeoutMilliseconds,
        nodeName);
}

HRESULT FCMC10_ProvisionFabric(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR codeFilepath,
    /* [in] */ LPCWSTR clusterManifestFilepath,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginProvisionFabric, &IFabricClusterManagementClient10::EndProvisionFabric,
        timeoutMilliseconds,
        codeFilepath,
        clusterManifestFilepath);
}

HRESULT FCMC10_UpgradeFabric(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_UPGRADE_DESCRIPTION *upgradeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginUpgradeFabric, &IFabricClusterManagementClient10::EndUpgradeFabric,
        timeoutMilliseconds,
        upgradeDescription);
}

HRESULT FCMC10_GetFabricUpgradeProgress(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricUpgradeProgressResult2 **result)
{
    return ServiceFabric_DoX(client, &IFabricClusterManagementClient10::BeginGetFabricUpgradeProgress, &IFabricClusterManagementClient10::EndGetFabricUpgradeProgress,
        timeoutMilliseconds,
        result);
}

HRESULT FCMC10_MoveNextFabricUpgradeDomain(
    IFabricClusterManagementClient10* client,
    /* [in] */ IFabricUpgradeProgressResult2 *progress,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginMoveNextFabricUpgradeDomain, &IFabricClusterManagementClient10::EndMoveNextFabricUpgradeDomain,
        timeoutMilliseconds,
        progress);
}

HRESULT FCMC10_MoveNextFabricUpgradeDomain2(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nextUpgradeDomain,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginMoveNextFabricUpgradeDomain2, &IFabricClusterManagementClient10::EndMoveNextFabricUpgradeDomain2,
        timeoutMilliseconds,
        nextUpgradeDomain);
}

HRESULT FCMC10_UnprovisionFabric(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR codeVersion,
    /* [in] */ LPCWSTR configVersion,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginUnprovisionFabric, &IFabricClusterManagementClient10::EndUnprovisionFabric,
        timeoutMilliseconds,
        codeVersion,
        configVersion);
}

HRESULT FCMC10_GetClusterManifest(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    return ServiceFabric_DoX(client, &IFabricClusterManagementClient10::BeginGetClusterManifest, &IFabricClusterManagementClient10::EndGetClusterManifest,
        timeoutMilliseconds,
        result);
}

HRESULT FCMC10_RecoverPartition(
    IFabricClusterManagementClient10* client,
    /* [in] */ FABRIC_PARTITION_ID partitionId,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverPartition, &IFabricClusterManagementClient10::EndRecoverPartition,
        timeoutMilliseconds,
        partitionId);
}

HRESULT FCMC10_RecoverServicePartitions(
    IFabricClusterManagementClient10* client,
    /* [in] */ FABRIC_URI serviceName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverServicePartitions, &IFabricClusterManagementClient10::EndRecoverServicePartitions,
        timeoutMilliseconds,
        serviceName);
}

HRESULT FCMC10_RecoverSystemPartitions(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverSystemPartitions, &IFabricClusterManagementClient10::EndRecoverSystemPartitions,
        timeoutMilliseconds);
}

HRESULT FCMC10_UpdateFabricUpgrade(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_UPGRADE_UPDATE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginUpdateFabricUpgrade, &IFabricClusterManagementClient10::EndUpdateFabricUpgrade,
        timeoutMilliseconds,
        description);
}

HRESULT FCMC10_StopNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_STOP_NODE_DESCRIPTION *stopNodeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginStopNode, &IFabricClusterManagementClient10::EndStopNode,
        timeoutMilliseconds,
        stopNodeDescription);
}

HRESULT FCMC10_RestartNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_RESTART_NODE_DESCRIPTION *restartNodeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRestartNode, &IFabricClusterManagementClient10::EndRestartNode,
        timeoutMilliseconds,
        restartNodeDescription);
}

HRESULT FCMC10_StartNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_START_NODE_DESCRIPTION *startNodeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginStartNode, &IFabricClusterManagementClient10::EndStartNode,
        timeoutMilliseconds,
        startNodeDescription);
}

HRESULT FCMC10_CopyClusterPackage(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR clusterManifestPath,
    /* [in] */ LPCWSTR clusterManifestPathInImageStore,
    /* [in] */ LPCWSTR codePackagePath,
    /* [in] */ LPCWSTR codePackagePathInImageStore)
{
    return This->CopyClusterPackage(imageStoreConnectionString, clusterManifestPath, clusterManifestPathInImageStore, codePackagePath, codePackagePathInImageStore);
}

HRESULT FCMC10_RemoveClusterPackage(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR clusterManifestPathInImageStore,
    /* [in] */ LPCWSTR codePackagePathInImageStore)
{
    return This->RemoveClusterPackage(imageStoreConnectionString, clusterManifestPathInImageStore, codePackagePathInImageStore);
}

HRESULT FCMC10_RollbackFabricUpgrade(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginRollbackFabricUpgrade, &IFabricClusterManagementClient10::EndRollbackFabricUpgrade,
        timeoutMilliseconds
        );
}

HRESULT FCMC10_ResetPartitionLoad(
    IFabricClusterManagementClient10 * This,
    /* [in] */ FABRIC_PARTITION_ID partitionId,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginResetPartitionLoad, &IFabricClusterManagementClient10::EndResetPartitionLoad,
        timeoutMilliseconds,
        partitionId
    );
}

HRESULT FCMC10_ToggleVerboseServicePlacementHealthReporting(
    IFabricClusterManagementClient10 * This,
    /* [in] */ BOOLEAN enabled,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginToggleVerboseServicePlacementHealthReporting, &IFabricClusterManagementClient10::EndToggleVerboseServicePlacementHealthReporting,
        timeoutMilliseconds,
        enabled
    );
}

HRESULT FCMC10_UpgradeConfiguration(
    IFabricClusterManagementClient10 * This,
    /* [in] */ const FABRIC_START_UPGRADE_DESCRIPTION *startUpgradeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginUpgradeConfiguration, &IFabricClusterManagementClient10::EndUpgradeConfiguration,
        timeoutMilliseconds,
        startUpgradeDescription
    );
}

HRESULT FCMC10_GetClusterConfigurationUpgradeStatus(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricOrchestrationUpgradeStatusResult **result)
{
    return ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterConfigurationUpgradeStatus, &IFabricClusterManagementClient10::EndGetClusterConfigurationUpgradeStatus,
        timeoutMilliseconds,
        result);
}

HRESULT FCMC10_GetClusterConfiguration(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    return ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterConfiguration, &IFabricClusterManagementClient10::EndGetClusterConfiguration,
        timeoutMilliseconds,
        result);
}

HRESULT FCMC10_GetUpgradesPendingApproval(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginGetUpgradesPendingApproval, &IFabricClusterManagementClient10::EndGetUpgradesPendingApproval,
        timeoutMilliseconds
    );
}

HRESULT FCMC10_StartApprovedUpgrades(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginStartApprovedUpgrades, &IFabricClusterManagementClient10::EndStartApprovedUpgrades,
        timeoutMilliseconds
    );
}

HRESULT FCMC10_GetClusterManifest2(
    IFabricClusterManagementClient10 * This,
    /* [in] */ const FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    return ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterManifest2, &IFabricClusterManagementClient10::EndGetClusterManifest2,
        timeoutMilliseconds,
        result,
        queryDescription);
}

HRESULT FCMC10_GetUpgradeOrchestrationServiceState(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    return ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetUpgradeOrchestrationServiceState, &IFabricClusterManagementClient10::EndGetUpgradeOrchestrationServiceState,
        timeoutMilliseconds,
        result);
}

HRESULT FCMC10_SetUpgradeOrchestrationServiceState(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR state,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricUpgradeOrchestrationServiceStateResult **result)
{
    return ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginSetUpgradeOrchestrationServiceState, &IFabricClusterManagementClient10::EndSetUpgradeOrchestrationServiceState,
        timeoutMilliseconds,
        result,
        state );
}

HRESULT FCMC10_GetClusterConfiguration2(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR apiVersion,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    return ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterConfiguration2, &IFabricClusterManagementClient10::EndGetClusterConfiguration2,
        timeoutMilliseconds,
        result,
        apiVersion);
}
