// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "fabricclient.h"

#include "servicefabricdox.h"

#include "ifabrictestmanagementclient3sync.h"

HRESULT FTMC3_StartPartitionDataLoss(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION* invokeDataLossDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartPartitionDataLoss, &IFabricTestManagementClient3::EndStartPartitionDataLoss,
        timeoutMilliseconds,
        invokeDataLossDescription);
}

HRESULT FTMC3_GetPartitionDataLossProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricPartitionDataLossProgressResult** result)
{
    return ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetPartitionDataLossProgress, &IFabricTestManagementClient3::EndGetPartitionDataLossProgress,
        timeoutMilliseconds,
        result,
        operationId);
}

HRESULT FTMC3_StartPartitionQuorumLoss(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION* invokeQuorumLossDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartPartitionQuorumLoss, &IFabricTestManagementClient3::EndStartPartitionQuorumLoss,
        timeoutMilliseconds,
        invokeQuorumLossDescription);
}

HRESULT FTMC3_GetPartitionQuorumLossProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricPartitionQuorumLossProgressResult** result)
{
    return ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetPartitionQuorumLossProgress, &IFabricTestManagementClient3::EndGetPartitionQuorumLossProgress,
        timeoutMilliseconds,
        result,
        operationId);
}

HRESULT FTMC3_StartPartitionRestart(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_PARTITION_RESTART_DESCRIPTION* restartPartitionDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartPartitionRestart, &IFabricTestManagementClient3::EndStartPartitionRestart,
        timeoutMilliseconds,
        restartPartitionDescription);
}

HRESULT FTMC3_GetPartitionRestartProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricPartitionRestartProgressResult** result)
{
    return ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetPartitionRestartProgress, &IFabricTestManagementClient3::EndGetPartitionRestartProgress,
        timeoutMilliseconds,
        result,
        operationId);
}

HRESULT FTMC3_GetTestCommandStatusList(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_TEST_COMMAND_LIST_DESCRIPTION* operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricTestCommandStatusResult** result)
{
    return ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetTestCommandStatusList, &IFabricTestManagementClient3::EndGetTestCommandStatusList,
        timeoutMilliseconds,
        result,
        operationId);
}

HRESULT FTMC3_CancelTestCommand(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION* invokeDataLossDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginCancelTestCommand, &IFabricTestManagementClient3::EndCancelTestCommand,
        timeoutMilliseconds,
        invokeDataLossDescription);
}

HRESULT FTMC3_StartChaos(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_CHAOS_DESCRIPTION* restartPartitionDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartChaos, &IFabricTestManagementClient3::EndStartChaos,
        timeoutMilliseconds,
        restartPartitionDescription);
}

HRESULT FTMC3_StopChaos(IFabricTestManagementClient3* client,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStopChaos, &IFabricTestManagementClient3::EndStopChaos,
        timeoutMilliseconds);
}

HRESULT FTMC3_GetChaosReport(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_GET_CHAOS_REPORT_DESCRIPTION* getChaosReportDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricChaosReportResult** result)
{
    return ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetChaosReport, &IFabricTestManagementClient3::EndGetChaosReport,
        timeoutMilliseconds,
        result,
        getChaosReportDescription);
}

HRESULT FTMC3_StartNodeTransition(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_NODE_TRANSITION_DESCRIPTION* description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartNodeTransition, &IFabricTestManagementClient3::EndStartNodeTransition,
        timeoutMilliseconds,
        description);
}

HRESULT FTMC3_GetNodeTransitionProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricNodeTransitionProgressResult** result)
{
    return ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetNodeTransitionProgress, &IFabricTestManagementClient3::EndGetNodeTransitionProgress,
        timeoutMilliseconds,
        result,
        operationId);
}
