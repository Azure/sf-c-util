// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"
#include "c_pal/uuid.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"
#include "sf_c_util/servicefabric_enums_to_strings.h"
#include "ifabricclustermanagementclient10sync.h"

#include "servicefabricdox.h"

/* Format specifiers for FABRIC_UPGRADE_DESCRIPTION */
#define PRI_FABRIC_UPGRADE_DESCRIPTION "s(FABRIC_UPGRADE_DESCRIPTION){ .CodeVersion=%ls, .ConfigVersion=%ls, .UpgradeKind=%" PRI_MU_ENUM ", .UpgradePolicyDescription=%p, .Reserved=%p }"

#define FABRIC_UPGRADE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->CodeVersion, \
    ((desc) == NULL) ? L"NULL" : (desc)->ConfigVersion, \
    MU_ENUM_VALUE(FABRIC_UPGRADE_KIND, ((desc) == NULL) ? FABRIC_UPGRADE_KIND_INVALID : (desc)->UpgradeKind), \
    ((desc) == NULL) ? NULL : (desc)->UpgradePolicyDescription, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_UPGRADE_UPDATE_DESCRIPTION */
#define PRI_FABRIC_UPGRADE_UPDATE_DESCRIPTION "s(FABRIC_UPGRADE_UPDATE_DESCRIPTION){ .UpgradeKind=%" PRI_MU_ENUM ", .UpdateFlags=%" PRIu32 ", .UpgradePolicyDescription=%p, .Reserved=%p }"

#define FABRIC_UPGRADE_UPDATE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_UPGRADE_KIND, ((desc) == NULL) ? FABRIC_UPGRADE_KIND_INVALID : (desc)->UpgradeKind), \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->UpdateFlags, \
    ((desc) == NULL) ? NULL : (desc)->UpgradePolicyDescription, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_STOP_NODE_DESCRIPTION */
#define PRI_FABRIC_STOP_NODE_DESCRIPTION "s(FABRIC_STOP_NODE_DESCRIPTION){ .NodeName=%ls, .NodeInstanceId=%" PRIu64 ", .Reserved=%p }"

#define FABRIC_STOP_NODE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? 0 : (uint64_t)(desc)->NodeInstanceId, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_RESTART_NODE_DESCRIPTION */
#define PRI_FABRIC_RESTART_NODE_DESCRIPTION "s(FABRIC_RESTART_NODE_DESCRIPTION){ .NodeName=%ls, .NodeInstanceId=%" PRIu64 ", .Reserved=%p }"

#define FABRIC_RESTART_NODE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? 0 : (uint64_t)(desc)->NodeInstanceId, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_START_NODE_DESCRIPTION */
#define PRI_FABRIC_START_NODE_DESCRIPTION "s(FABRIC_START_NODE_DESCRIPTION){ .NodeName=%ls, .NodeInstanceId=%" PRIu64 ", .IPAddressOrFQDN=%ls, .ClusterConnectionPort=%" PRIu32 ", .Reserved=%p }"

#define FABRIC_START_NODE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? 0 : (uint64_t)(desc)->NodeInstanceId, \
    ((desc) == NULL) ? L"NULL" : (desc)->IPAddressOrFQDN, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->ClusterConnectionPort, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_START_UPGRADE_DESCRIPTION */
#define PRI_FABRIC_START_UPGRADE_DESCRIPTION "s(FABRIC_START_UPGRADE_DESCRIPTION){ .ClusterConfig=%ls, .HealthCheckRetryTimeoutInSeconds=%" PRIu32 ", .HealthCheckWaitDurationInSeconds=%" PRIu32 ", .HealthCheckStableDurationInSeconds=%" PRIu32 ", .UpgradeDomainTimeoutInSeconds=%" PRIu32 ", .UpgradeTimeoutInSeconds=%" PRIu32 ", .MaxPercentUnhealthyApplications=%" PRIu8 ", .MaxPercentUnhealthyNodes=%" PRIu8 ", .MaxPercentDeltaUnhealthyNodes=%" PRIu8 ", .MaxPercentUpgradeDomainDeltaUnhealthyNodes=%" PRIu8 ", .Reserved=%p }"

#define FABRIC_START_UPGRADE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ClusterConfig, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->HealthCheckRetryTimeoutInSeconds, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->HealthCheckWaitDurationInSeconds, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->HealthCheckStableDurationInSeconds, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->UpgradeDomainTimeoutInSeconds, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->UpgradeTimeoutInSeconds, \
    ((desc) == NULL) ? 0 : (uint8_t)(desc)->MaxPercentUnhealthyApplications, \
    ((desc) == NULL) ? 0 : (uint8_t)(desc)->MaxPercentUnhealthyNodes, \
    ((desc) == NULL) ? 0 : (uint8_t)(desc)->MaxPercentDeltaUnhealthyNodes, \
    ((desc) == NULL) ? 0 : (uint8_t)(desc)->MaxPercentUpgradeDomainDeltaUnhealthyNodes, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION */
#define PRI_FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION "s(FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION){ .ClusterManifestVersion=%ls, .Reserved=%p }"

#define FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ClusterManifestVersion, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

HRESULT FCMC10_NodeStateRemoved(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_NodeStateRemoved called with client=%p, nodeName=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, nodeName, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginNodeStateRemoved, &IFabricClusterManagementClient10::EndNodeStateRemoved,
        timeoutMilliseconds,
        nodeName);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_NodeStateRemoved returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_NodeStateRemoved returning");
    }
    return result;
}

HRESULT FCMC10_RecoverPartitions(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_RecoverPartitions called with client=%p, timeoutMilliseconds=%" PRIu32 "",
        client, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverPartitions, &IFabricClusterManagementClient10::EndRecoverPartitions,
        timeoutMilliseconds);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_RecoverPartitions returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_RecoverPartitions returning");
    }
    return result;
}

HRESULT FCMC10_DeactivateNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ FABRIC_NODE_DEACTIVATION_INTENT intent,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_DeactivateNode called with client=%p, nodeName=%ls, intent=%" PRI_MU_ENUM ", timeoutMilliseconds=%" PRIu32 "",
        client, nodeName, MU_ENUM_VALUE(FABRIC_NODE_DEACTIVATION_INTENT, intent), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginDeactivateNode, &IFabricClusterManagementClient10::EndDeactivateNode,
        timeoutMilliseconds,
        nodeName,
        intent);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_DeactivateNode returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_DeactivateNode returning");
    }
    return result;
}

HRESULT FCMC10_ActivateNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_ActivateNode called with client=%p, nodeName=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, nodeName, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginActivateNode, &IFabricClusterManagementClient10::EndActivateNode,
        timeoutMilliseconds,
        nodeName);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_ActivateNode returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_ActivateNode returning");
    }
    return result;
}

HRESULT FCMC10_ProvisionFabric(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR codeFilepath,
    /* [in] */ LPCWSTR clusterManifestFilepath,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_ProvisionFabric called with client=%p, codeFilepath=%ls, clusterManifestFilepath=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, codeFilepath, clusterManifestFilepath, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginProvisionFabric, &IFabricClusterManagementClient10::EndProvisionFabric,
        timeoutMilliseconds,
        codeFilepath,
        clusterManifestFilepath);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_ProvisionFabric returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_ProvisionFabric returning");
    }
    return result;
}

HRESULT FCMC10_UpgradeFabric(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_UPGRADE_DESCRIPTION *upgradeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_UpgradeFabric called with client=%p, upgradeDescription=%" PRI_FABRIC_UPGRADE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_UPGRADE_DESCRIPTION_VALUES_OR_NULL(upgradeDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginUpgradeFabric, &IFabricClusterManagementClient10::EndUpgradeFabric,
        timeoutMilliseconds,
        upgradeDescription);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_UpgradeFabric returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_UpgradeFabric returning");
    }
    return result;
}

HRESULT FCMC10_GetFabricUpgradeProgress(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricUpgradeProgressResult2 **result)
{
    LogInfo("FCMC10_GetFabricUpgradeProgress called with client=%p, timeoutMilliseconds=%" PRIu32 ", result=%p",
        client, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricClusterManagementClient10::BeginGetFabricUpgradeProgress, &IFabricClusterManagementClient10::EndGetFabricUpgradeProgress,
        timeoutMilliseconds,
        result);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_GetFabricUpgradeProgress returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_GetFabricUpgradeProgress returning");
    }
    return hresult;
}

HRESULT FCMC10_MoveNextFabricUpgradeDomain(
    IFabricClusterManagementClient10* client,
    /* [in] */ IFabricUpgradeProgressResult2 *progress,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_MoveNextFabricUpgradeDomain called with client=%p, progress=%p, timeoutMilliseconds=%" PRIu32 "",
        client, progress, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginMoveNextFabricUpgradeDomain, &IFabricClusterManagementClient10::EndMoveNextFabricUpgradeDomain,
        timeoutMilliseconds,
        progress);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_MoveNextFabricUpgradeDomain returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_MoveNextFabricUpgradeDomain returning");
    }
    return result;
}

HRESULT FCMC10_MoveNextFabricUpgradeDomain2(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR nextUpgradeDomain,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_MoveNextFabricUpgradeDomain2 called with client=%p, nextUpgradeDomain=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, nextUpgradeDomain, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginMoveNextFabricUpgradeDomain2, &IFabricClusterManagementClient10::EndMoveNextFabricUpgradeDomain2,
        timeoutMilliseconds,
        nextUpgradeDomain);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_MoveNextFabricUpgradeDomain2 returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_MoveNextFabricUpgradeDomain2 returning");
    }
    return result;
}

HRESULT FCMC10_UnprovisionFabric(
    IFabricClusterManagementClient10* client,
    /* [in] */ LPCWSTR codeVersion,
    /* [in] */ LPCWSTR configVersion,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_UnprovisionFabric called with client=%p, codeVersion=%ls, configVersion=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, codeVersion, configVersion, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginUnprovisionFabric, &IFabricClusterManagementClient10::EndUnprovisionFabric,
        timeoutMilliseconds,
        codeVersion,
        configVersion);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_UnprovisionFabric returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_UnprovisionFabric returning");
    }
    return result;
}

HRESULT FCMC10_GetClusterManifest(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    LogInfo("FCMC10_GetClusterManifest called with client=%p, timeoutMilliseconds=%" PRIu32 ", result=%p",
        client, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricClusterManagementClient10::BeginGetClusterManifest, &IFabricClusterManagementClient10::EndGetClusterManifest,
        timeoutMilliseconds,
        result);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_GetClusterManifest returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_GetClusterManifest returning");
    }
    return hresult;
}

HRESULT FCMC10_RecoverPartition(
    IFabricClusterManagementClient10* client,
    /* [in] */ FABRIC_PARTITION_ID partitionId,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_RecoverPartition called with client=%p, partitionId=%" PRI_GUID ", timeoutMilliseconds=%" PRIu32 "",
        client, GUID_VALUES(partitionId), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverPartition, &IFabricClusterManagementClient10::EndRecoverPartition,
        timeoutMilliseconds,
        partitionId);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_RecoverPartition returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_RecoverPartition returning");
    }
    return result;
}

HRESULT FCMC10_RecoverServicePartitions(
    IFabricClusterManagementClient10* client,
    /* [in] */ FABRIC_URI serviceName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_RecoverServicePartitions called with client=%p, serviceName=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, serviceName, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverServicePartitions, &IFabricClusterManagementClient10::EndRecoverServicePartitions,
        timeoutMilliseconds,
        serviceName);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_RecoverServicePartitions returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_RecoverServicePartitions returning");
    }
    return result;
}

HRESULT FCMC10_RecoverSystemPartitions(
    IFabricClusterManagementClient10* client,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_RecoverSystemPartitions called with client=%p, timeoutMilliseconds=%" PRIu32 "",
        client, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRecoverSystemPartitions, &IFabricClusterManagementClient10::EndRecoverSystemPartitions,
        timeoutMilliseconds);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_RecoverSystemPartitions returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_RecoverSystemPartitions returning");
    }
    return result;
}

HRESULT FCMC10_UpdateFabricUpgrade(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_UPGRADE_UPDATE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_UpdateFabricUpgrade called with client=%p, description=%" PRI_FABRIC_UPGRADE_UPDATE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_UPGRADE_UPDATE_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginUpdateFabricUpgrade, &IFabricClusterManagementClient10::EndUpdateFabricUpgrade,
        timeoutMilliseconds,
        description);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_UpdateFabricUpgrade returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_UpdateFabricUpgrade returning");
    }
    return result;
}

HRESULT FCMC10_StopNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_STOP_NODE_DESCRIPTION *stopNodeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_StopNode called with client=%p, stopNodeDescription=%" PRI_FABRIC_STOP_NODE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_STOP_NODE_DESCRIPTION_VALUES_OR_NULL(stopNodeDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginStopNode, &IFabricClusterManagementClient10::EndStopNode,
        timeoutMilliseconds,
        stopNodeDescription);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_StopNode returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_StopNode returning");
    }
    return result;
}

HRESULT FCMC10_RestartNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_RESTART_NODE_DESCRIPTION *restartNodeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_RestartNode called with client=%p, restartNodeDescription=%" PRI_FABRIC_RESTART_NODE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_RESTART_NODE_DESCRIPTION_VALUES_OR_NULL(restartNodeDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginRestartNode, &IFabricClusterManagementClient10::EndRestartNode,
        timeoutMilliseconds,
        restartNodeDescription);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_RestartNode returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_RestartNode returning");
    }
    return result;
}

HRESULT FCMC10_StartNode(
    IFabricClusterManagementClient10* client,
    /* [in] */ const FABRIC_START_NODE_DESCRIPTION *startNodeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_StartNode called with client=%p, startNodeDescription=%" PRI_FABRIC_START_NODE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_START_NODE_DESCRIPTION_VALUES_OR_NULL(startNodeDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricClusterManagementClient10::BeginStartNode, &IFabricClusterManagementClient10::EndStartNode,
        timeoutMilliseconds,
        startNodeDescription);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_StartNode returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_StartNode returning");
    }
    return result;
}

HRESULT FCMC10_CopyClusterPackage(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR clusterManifestPath,
    /* [in] */ LPCWSTR clusterManifestPathInImageStore,
    /* [in] */ LPCWSTR codePackagePath,
    /* [in] */ LPCWSTR codePackagePathInImageStore)
{
    LogInfo("FCMC10_CopyClusterPackage called with This=%p, imageStoreConnectionString=%ls, clusterManifestPath=%ls, clusterManifestPathInImageStore=%ls, codePackagePath=%ls, codePackagePathInImageStore=%ls",
        This, imageStoreConnectionString, clusterManifestPath, clusterManifestPathInImageStore, codePackagePath, codePackagePathInImageStore);

    HRESULT result = This->CopyClusterPackage(imageStoreConnectionString, clusterManifestPath, clusterManifestPathInImageStore, codePackagePath, codePackagePathInImageStore);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_CopyClusterPackage returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_CopyClusterPackage returning");
    }
    return result;
}

HRESULT FCMC10_RemoveClusterPackage(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR clusterManifestPathInImageStore,
    /* [in] */ LPCWSTR codePackagePathInImageStore)
{
    LogInfo("FCMC10_RemoveClusterPackage called with This=%p, imageStoreConnectionString=%ls, clusterManifestPathInImageStore=%ls, codePackagePathInImageStore=%ls",
        This, imageStoreConnectionString, clusterManifestPathInImageStore, codePackagePathInImageStore);

    HRESULT result = This->RemoveClusterPackage(imageStoreConnectionString, clusterManifestPathInImageStore, codePackagePathInImageStore);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_RemoveClusterPackage returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_RemoveClusterPackage returning");
    }
    return result;
}

HRESULT FCMC10_RollbackFabricUpgrade(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_RollbackFabricUpgrade called with This=%p, timeoutMilliseconds=%" PRIu32 "",
        This, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginRollbackFabricUpgrade, &IFabricClusterManagementClient10::EndRollbackFabricUpgrade,
        timeoutMilliseconds
        );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_RollbackFabricUpgrade returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_RollbackFabricUpgrade returning");
    }
    return result;
}

HRESULT FCMC10_ResetPartitionLoad(
    IFabricClusterManagementClient10 * This,
    /* [in] */ FABRIC_PARTITION_ID partitionId,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_ResetPartitionLoad called with This=%p, partitionId=%" PRI_GUID ", timeoutMilliseconds=%" PRIu32 "",
        This, GUID_VALUES(partitionId), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginResetPartitionLoad, &IFabricClusterManagementClient10::EndResetPartitionLoad,
        timeoutMilliseconds,
        partitionId
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_ResetPartitionLoad returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_ResetPartitionLoad returning");
    }
    return result;
}

HRESULT FCMC10_ToggleVerboseServicePlacementHealthReporting(
    IFabricClusterManagementClient10 * This,
    /* [in] */ BOOLEAN enabled,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_ToggleVerboseServicePlacementHealthReporting called with This=%p, enabled=%" PRI_BOOL ", timeoutMilliseconds=%" PRIu32 "",
        This, MU_BOOL_VALUE(enabled), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginToggleVerboseServicePlacementHealthReporting, &IFabricClusterManagementClient10::EndToggleVerboseServicePlacementHealthReporting,
        timeoutMilliseconds,
        enabled
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_ToggleVerboseServicePlacementHealthReporting returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_ToggleVerboseServicePlacementHealthReporting returning");
    }
    return result;
}

HRESULT FCMC10_UpgradeConfiguration(
    IFabricClusterManagementClient10 * This,
    /* [in] */ const FABRIC_START_UPGRADE_DESCRIPTION *startUpgradeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_UpgradeConfiguration called with This=%p, startUpgradeDescription=%" PRI_FABRIC_START_UPGRADE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_START_UPGRADE_DESCRIPTION_VALUES_OR_NULL(startUpgradeDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginUpgradeConfiguration, &IFabricClusterManagementClient10::EndUpgradeConfiguration,
        timeoutMilliseconds,
        startUpgradeDescription
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_UpgradeConfiguration returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_UpgradeConfiguration returning");
    }
    return result;
}

HRESULT FCMC10_GetClusterConfigurationUpgradeStatus(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricOrchestrationUpgradeStatusResult **result)
{
    LogInfo("FCMC10_GetClusterConfigurationUpgradeStatus called with This=%p, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterConfigurationUpgradeStatus, &IFabricClusterManagementClient10::EndGetClusterConfigurationUpgradeStatus,
        timeoutMilliseconds,
        result);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_GetClusterConfigurationUpgradeStatus returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_GetClusterConfigurationUpgradeStatus returning");
    }
    return hresult;
}

HRESULT FCMC10_GetClusterConfiguration(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    LogInfo("FCMC10_GetClusterConfiguration called with This=%p, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterConfiguration, &IFabricClusterManagementClient10::EndGetClusterConfiguration,
        timeoutMilliseconds,
        result);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_GetClusterConfiguration returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_GetClusterConfiguration returning");
    }
    return hresult;
}

HRESULT FCMC10_GetUpgradesPendingApproval(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_GetUpgradesPendingApproval called with This=%p, timeoutMilliseconds=%" PRIu32 "",
        This, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginGetUpgradesPendingApproval, &IFabricClusterManagementClient10::EndGetUpgradesPendingApproval,
        timeoutMilliseconds
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_GetUpgradesPendingApproval returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_GetUpgradesPendingApproval returning");
    }
    return result;
}

HRESULT FCMC10_StartApprovedUpgrades(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogInfo("FCMC10_StartApprovedUpgrades called with This=%p, timeoutMilliseconds=%" PRIu32 "",
        This, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricClusterManagementClient10::BeginStartApprovedUpgrades, &IFabricClusterManagementClient10::EndStartApprovedUpgrades,
        timeoutMilliseconds
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FCMC10_StartApprovedUpgrades returning");
    }
    else
    {
        LogHRESULTInfo(result, "FCMC10_StartApprovedUpgrades returning");
    }
    return result;
}

HRESULT FCMC10_GetClusterManifest2(
    IFabricClusterManagementClient10 * This,
    /* [in] */ const FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    LogInfo("FCMC10_GetClusterManifest2 called with This=%p, queryDescription=%" PRI_FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_CLUSTER_MANIFEST_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterManifest2, &IFabricClusterManagementClient10::EndGetClusterManifest2,
        timeoutMilliseconds,
        result,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_GetClusterManifest2 returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_GetClusterManifest2 returning");
    }
    return hresult;
}

HRESULT FCMC10_GetUpgradeOrchestrationServiceState(
    IFabricClusterManagementClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    LogInfo("FCMC10_GetUpgradeOrchestrationServiceState called with This=%p, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetUpgradeOrchestrationServiceState, &IFabricClusterManagementClient10::EndGetUpgradeOrchestrationServiceState,
        timeoutMilliseconds,
        result);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_GetUpgradeOrchestrationServiceState returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_GetUpgradeOrchestrationServiceState returning");
    }
    return hresult;
}

HRESULT FCMC10_SetUpgradeOrchestrationServiceState(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR state,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricUpgradeOrchestrationServiceStateResult **result)
{
    LogInfo("FCMC10_SetUpgradeOrchestrationServiceState called with This=%p, state=%ls, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, state, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginSetUpgradeOrchestrationServiceState, &IFabricClusterManagementClient10::EndSetUpgradeOrchestrationServiceState,
        timeoutMilliseconds,
        result,
        state );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_SetUpgradeOrchestrationServiceState returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_SetUpgradeOrchestrationServiceState returning");
    }
    return hresult;
}

HRESULT FCMC10_GetClusterConfiguration2(
    IFabricClusterManagementClient10 * This,
    /* [in] */ LPCWSTR apiVersion,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    LogInfo("FCMC10_GetClusterConfiguration2 called with This=%p, apiVersion=%ls, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, apiVersion, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricClusterManagementClient10::BeginGetClusterConfiguration2, &IFabricClusterManagementClient10::EndGetClusterConfiguration2,
        timeoutMilliseconds,
        result,
        apiVersion);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FCMC10_GetClusterConfiguration2 returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FCMC10_GetClusterConfiguration2 returning");
    }
    return hresult;
}
