// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef IFABRICCLUSTERMANAGEMENTCLIENT10SYNC_H
#define IFABRICCLUSTERMANAGEMENTCLIENT10SYNC_H

#include "windows.h"

#include "fabricclient.h"

#include "sf_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

    HRESULT FCMC10_NodeStateRemoved(
        IFabricClusterManagementClient10* client,
        /* [in] */ LPCWSTR nodeName,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_RecoverPartitions(
        IFabricClusterManagementClient10* client,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_DeactivateNode(
        IFabricClusterManagementClient10* client,
        /* [in] */ LPCWSTR nodeName,
        /* [in] */ FABRIC_NODE_DEACTIVATION_INTENT intent,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_ActivateNode(
        IFabricClusterManagementClient10* client,
        /* [in] */ LPCWSTR nodeName,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_ProvisionFabric(
        IFabricClusterManagementClient10* client,
        /* [in] */ LPCWSTR codeFilepath,
        /* [in] */ LPCWSTR clusterManifestFilepath,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_UpgradeFabric(
        IFabricClusterManagementClient10* client,
        /* [in] */ const FABRIC_UPGRADE_DESCRIPTION *upgradeDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_GetFabricUpgradeProgress(
        IFabricClusterManagementClient10* client,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricUpgradeProgressResult2 **result);

    HRESULT FCMC10_MoveNextFabricUpgradeDomain(
        IFabricClusterManagementClient10* client,
        /* [in] */ IFabricUpgradeProgressResult2 *progress,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_MoveNextFabricUpgradeDomain2(
        IFabricClusterManagementClient10* client,
        /* [in] */ LPCWSTR nextUpgradeDomain,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_UnprovisionFabric(
        IFabricClusterManagementClient10* client,
        /* [in] */ LPCWSTR codeVersion,
        /* [in] */ LPCWSTR configVersion,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_GetClusterManifest(
        IFabricClusterManagementClient10* client,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricStringResult **result);

    HRESULT FCMC10_RecoverPartition(
        IFabricClusterManagementClient10* client,
        /* [in] */ FABRIC_PARTITION_ID partitionId,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_RecoverServicePartitions(
        IFabricClusterManagementClient10* client,
        /* [in] */ FABRIC_URI serviceName,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_RecoverSystemPartitions(
        IFabricClusterManagementClient10* client,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_UpdateFabricUpgrade(
        IFabricClusterManagementClient10* client,
        /* [in] */ const FABRIC_UPGRADE_UPDATE_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_StopNode(
        IFabricClusterManagementClient10* client,
        /* [in] */ const FABRIC_STOP_NODE_DESCRIPTION *stopNodeDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_RestartNode(
        IFabricClusterManagementClient10* client,
        /* [in] */ const FABRIC_RESTART_NODE_DESCRIPTION *restartNodeDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_StartNode(
        IFabricClusterManagementClient10* client,
        /* [in] */ const FABRIC_START_NODE_DESCRIPTION *startNodeDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_CopyClusterPackage(
        IFabricClusterManagementClient10 * This,
        /* [in] */ LPCWSTR imageStoreConnectionString,
        /* [in] */ LPCWSTR clusterManifestPath,
        /* [in] */ LPCWSTR clusterManifestPathInImageStore,
        /* [in] */ LPCWSTR codePackagePath,
        /* [in] */ LPCWSTR codePackagePathInImageStore);

    HRESULT FCMC10_RemoveClusterPackage(
        IFabricClusterManagementClient10 * This,
        /* [in] */ LPCWSTR imageStoreConnectionString,
        /* [in] */ LPCWSTR clusterManifestPathInImageStore,
        /* [in] */ LPCWSTR codePackagePathInImageStore);

    HRESULT FCMC10_RollbackFabricUpgrade(
        IFabricClusterManagementClient10 * This,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_ResetPartitionLoad(
        IFabricClusterManagementClient10 * This,
        /* [in] */ FABRIC_PARTITION_ID partitionId,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_ToggleVerboseServicePlacementHealthReporting(
        IFabricClusterManagementClient10 * This,
        /* [in] */ BOOLEAN enabled,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_UpgradeConfiguration(
        IFabricClusterManagementClient10 * This,
        /* [in] */ const FABRIC_START_UPGRADE_DESCRIPTION *startUpgradeDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_GetClusterConfigurationUpgradeStatus(
        IFabricClusterManagementClient10 * This,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricOrchestrationUpgradeStatusResult **result);

    HRESULT FCMC10_GetClusterConfiguration(
        IFabricClusterManagementClient10 * This,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricStringResult **result);

    HRESULT FCMC10_GetUpgradesPendingApproval(
        IFabricClusterManagementClient10 * This,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_StartApprovedUpgrades(
        IFabricClusterManagementClient10 * This,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FCMC10_GetClusterManifest2(
        IFabricClusterManagementClient10 * This,
        /* [in] */ const FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricStringResult **result);

    HRESULT FCMC10_GetUpgradeOrchestrationServiceState(
        IFabricClusterManagementClient10 * This,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricStringResult **result);

    HRESULT FCMC10_SetUpgradeOrchestrationServiceState(
        IFabricClusterManagementClient10 * This,
        /* [in] */ LPCWSTR state,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricUpgradeOrchestrationServiceStateResult **result);

    HRESULT FCMC10_GetClusterConfiguration2(
        IFabricClusterManagementClient10 * This,
        /* [in] */ LPCWSTR apiVersion,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricStringResult **result);

    DECLARE_CREATE_IFABRICINSTANCE(IFabricClusterManagementClient10);

#ifdef __cplusplus
}
#endif

#endif /*IFABRICCLUSTERMANAGEMENTCLIENT10SYNC_H*/
