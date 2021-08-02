// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef H_FABRIC_CLUSTER_MANAGEMENT_CLIENT10_H
#define H_FABRIC_CLUSTER_MANAGEMENT_CLIENT10_H

#include "windows.h"

#include "fabricclient.h"

#include "macro_utils/macro_utils.h"

#include "h_fabric_macro_generator.h"

H_FABRIC_DEFINE_TYPEDEF(IFabricClusterManagementClient10);

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    H_FABRIC_DECLARE_CREATE(IFabricClusterManagementClient10);
    H_FABRIC_DECLARE_DESTROY(IFabricClusterManagementClient10);

    /*below are the same API as ifabricqueryclient10sync exposes, which are the same as the methods of the IFabricClusterManagementClient10 interface*/
    /*note: the computed outputs (such as IFabricGetNodeListResult are "as they come" from IFabricClusterManagementClient10 interface*/

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_NodeStateRemoved,
        IN_ARGS(
            ARG(LPCWSTR, nodeName),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_RecoverPartitions,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_DeactivateNode,
        IN_ARGS(
            ARG(LPCWSTR, nodeName),
            ARG(FABRIC_NODE_DEACTIVATION_INTENT, intent),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_ActivateNode,
        IN_ARGS(
            ARG(LPCWSTR, nodeName),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_ProvisionFabric,
        IN_ARGS(
            ARG(LPCWSTR, codeFilepath),
            ARG(LPCWSTR, clusterManifestFilepath),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_UpgradeFabric,
        IN_ARGS(
            ARG(const FABRIC_UPGRADE_DESCRIPTION*, upgradeDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetFabricUpgradeProgress,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricUpgradeProgressResult2**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_MoveNextFabricUpgradeDomain,
        IN_ARGS(
            ARG(IFabricUpgradeProgressResult2*, progress),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_MoveNextFabricUpgradeDomain2,
        IN_ARGS(
            ARG(LPCWSTR, nextUpgradeDomain),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_UnprovisionFabric,
        IN_ARGS(
            ARG(LPCWSTR, codeVersion),
            ARG(LPCWSTR, configVersion),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetClusterManifest,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStringResult**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_RecoverPartition,
        IN_ARGS(
            ARG(FABRIC_PARTITION_ID, partitionId),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_RecoverServicePartitions,
        IN_ARGS(
            ARG(FABRIC_URI, serviceName),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_RecoverSystemPartitions,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_UpdateFabricUpgrade,
        IN_ARGS(
            ARG(const FABRIC_UPGRADE_UPDATE_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_StopNode,
        IN_ARGS(
            ARG(const FABRIC_STOP_NODE_DESCRIPTION*, stopNodeDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_RestartNode,
        IN_ARGS(
            ARG(const FABRIC_RESTART_NODE_DESCRIPTION*, restartNodeDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_StartNode,
        IN_ARGS(
            ARG(const FABRIC_START_NODE_DESCRIPTION*, startNodeDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_CopyClusterPackage,
        IN_ARGS(
            ARG(LPCWSTR, imageStoreConnectionString),
            ARG(LPCWSTR, clusterManifestPath),
            ARG(LPCWSTR, clusterManifestPathInImageStore),
            ARG(LPCWSTR, codePackagePath),
            ARG(LPCWSTR, codePackagePathInImageStore)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_RemoveClusterPackage,
        IN_ARGS(
            ARG(LPCWSTR, imageStoreConnectionString),
            ARG(LPCWSTR, clusterManifestPathInImageStore),
            ARG(LPCWSTR, codePackagePathInImageStore)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_RollbackFabricUpgrade,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_ResetPartitionLoad,
        IN_ARGS(
            ARG(FABRIC_PARTITION_ID, partitionId),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_ToggleVerboseServicePlacementHealthReporting,
        IN_ARGS(
            ARG(BOOLEAN, enabled),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_UpgradeConfiguration,
        IN_ARGS(
            ARG(const FABRIC_START_UPGRADE_DESCRIPTION*, startUpgradeDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetClusterConfigurationUpgradeStatus,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricOrchestrationUpgradeStatusResult**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetClusterConfiguration,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStringResult**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetUpgradesPendingApproval,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds)
        )
    )


    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_StartApprovedUpgrades,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetClusterManifest2,
        IN_ARGS(
            ARG(const FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStringResult**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetUpgradeOrchestrationServiceState,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStringResult**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_SetUpgradeOrchestrationServiceState,
        IN_ARGS(
            ARG(LPCWSTR, state),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricUpgradeOrchestrationServiceStateResult**, result)
        )
    )

    H_FABRIC_DECLARE_API(IFabricClusterManagementClient10, FCMC10_GetClusterConfiguration2,
        IN_ARGS(
            ARG(LPCWSTR, apiVersion),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStringResult**, result)
        )
    )

#ifdef __cplusplus
}
#endif

#endif /*H_FABRIC_CLUSTER_MANAGEMENT_CLIENT10_H*/
