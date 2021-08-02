// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef IFABRICTESTMANAGEMENTCLIENT3SYNC_H
#define IFABRICTESTMANAGEMENTCLIENT3SYNC_H

#include "windows.h"

#include "fabricclient.h"

#include "sf_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

    HRESULT FTMC3_StartPartitionDataLoss(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION* invokeDataLossDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FTMC3_GetPartitionDataLossProgress(IFabricTestManagementClient3* client,
        /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricPartitionDataLossProgressResult** result);

    HRESULT FTMC3_StartPartitionQuorumLoss(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION* invokeQuorumLossDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FTMC3_GetPartitionQuorumLossProgress(IFabricTestManagementClient3* client,
        /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricPartitionQuorumLossProgressResult** result);

    HRESULT FTMC3_StartPartitionRestart(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_START_PARTITION_RESTART_DESCRIPTION* restartPartitionDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FTMC3_GetPartitionRestartProgress(IFabricTestManagementClient3* client,
        /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricPartitionRestartProgressResult** result);

    HRESULT FTMC3_GetTestCommandStatusList(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_TEST_COMMAND_LIST_DESCRIPTION* operationId,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricTestCommandStatusResult** result);

    HRESULT FTMC3_CancelTestCommand(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION* invokeDataLossDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FTMC3_StartChaos(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_START_CHAOS_DESCRIPTION* restartPartitionDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FTMC3_StopChaos(IFabricTestManagementClient3* client,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FTMC3_GetChaosReport(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_GET_CHAOS_REPORT_DESCRIPTION* getChaosReportDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricChaosReportResult** result);
    
    HRESULT FTMC3_StartNodeTransition(IFabricTestManagementClient3* client,
        /* [in] */ const FABRIC_NODE_TRANSITION_DESCRIPTION* description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FTMC3_GetNodeTransitionProgress(IFabricTestManagementClient3* client,
        /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricNodeTransitionProgressResult** result);

    DECLARE_CREATE_IFABRICINSTANCE(IFabricTestManagementClient3);

#ifdef __cplusplus
}
#endif


#endif /*IFABRICTESTMANAGEMENTCLIENT3SYNC_H*/

