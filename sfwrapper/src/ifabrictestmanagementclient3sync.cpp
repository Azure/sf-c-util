// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"

#include "c_pal/uuid.h"

#include "fabricclient.h"

#include "sf_c_util/servicefabric_enums_to_strings.h"

#include "servicefabricdox.h"

#include "ifabrictestmanagementclient3sync.h"

// Format specifier for FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION
#define PRI_FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION "s(FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION){ .OperationId=%" PRI_GUID ", .PartitionSelector=%p, .DataLossMode=%" PRI_MU_ENUM ", .Reserved=%p }"
#define FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->OperationId), \
    ((desc) == NULL) ? NULL : (desc)->PartitionSelector, \
    MU_ENUM_VALUE(FABRIC_DATA_LOSS_MODE, ((desc) == NULL) ? FABRIC_DATA_LOSS_MODE_INVALID : (desc)->DataLossMode), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifier for FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION
#define PRI_FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION "s(FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION){ .OperationId=%" PRI_GUID ", .PartitionSelector=%p, .QuorumLossMode=%" PRI_MU_ENUM ", .QuorumLossDurationInMilliSeconds=%" PRId32 ", .Reserved=%p }"
#define FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->OperationId), \
    ((desc) == NULL) ? NULL : (desc)->PartitionSelector, \
    MU_ENUM_VALUE(FABRIC_QUORUM_LOSS_MODE, ((desc) == NULL) ? FABRIC_QUORUM_LOSS_MODE_INVALID : (desc)->QuorumLossMode), \
    ((desc) == NULL) ? 0 : (int32_t)(desc)->QuorumLossDurationInMilliSeconds, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifier for FABRIC_START_PARTITION_RESTART_DESCRIPTION
#define PRI_FABRIC_START_PARTITION_RESTART_DESCRIPTION "s(FABRIC_START_PARTITION_RESTART_DESCRIPTION){ .OperationId=%" PRI_GUID ", .PartitionSelector=%p, .RestartPartitionMode=%" PRI_MU_ENUM ", .Reserved=%p }"
#define FABRIC_START_PARTITION_RESTART_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->OperationId), \
    ((desc) == NULL) ? NULL : (desc)->PartitionSelector, \
    MU_ENUM_VALUE(FABRIC_RESTART_PARTITION_MODE, ((desc) == NULL) ? FABRIC_RESTART_PARTITION_MODE_INVALID : (desc)->RestartPartitionMode), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifier for FABRIC_TEST_COMMAND_LIST_DESCRIPTION
#define PRI_FABRIC_TEST_COMMAND_LIST_DESCRIPTION "s(FABRIC_TEST_COMMAND_LIST_DESCRIPTION){ .TestCommandStateFilter=%" PRIu32 ", .TestCommandTypeFilter=%" PRIu32 ", .Reserved=%p }"
#define FABRIC_TEST_COMMAND_LIST_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->TestCommandStateFilter, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->TestCommandTypeFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifier for FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION
#define PRI_FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION "s(FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION){ .OperationId=%" PRI_GUID ", .Force=%" PRI_BOOL ", .Reserved=%p }"
#define FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->OperationId), \
    MU_BOOL_VALUE(((desc) == NULL) ? FALSE : (desc)->Force), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifier for FABRIC_START_CHAOS_DESCRIPTION
#define PRI_FABRIC_START_CHAOS_DESCRIPTION "s(FABRIC_START_CHAOS_DESCRIPTION){ .ChaosParameters=%p, .Reserved=%p }"
#define FABRIC_START_CHAOS_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? NULL : (desc)->ChaosParameters, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifier for FABRIC_GET_CHAOS_REPORT_DESCRIPTION
#define PRI_FABRIC_GET_CHAOS_REPORT_DESCRIPTION "s(FABRIC_GET_CHAOS_REPORT_DESCRIPTION){ .Filter=%p, .ContinuationToken=%ls, .Reserved=%p }"
#define FABRIC_GET_CHAOS_REPORT_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? NULL : (desc)->Filter, \
    ((desc) == NULL) ? L"NULL" : (desc)->ContinuationToken, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifier for FABRIC_NODE_TRANSITION_DESCRIPTION
#define PRI_FABRIC_NODE_TRANSITION_DESCRIPTION "s(FABRIC_NODE_TRANSITION_DESCRIPTION){ .NodeTransitionType=%" PRI_MU_ENUM ", .Value=%p }"
#define FABRIC_NODE_TRANSITION_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_NODE_TRANSITION_TYPE, ((desc) == NULL) ? FABRIC_NODE_TRANSITION_TYPE_INVALID : (desc)->NodeTransitionType), \
    ((desc) == NULL) ? NULL : (desc)->Value

HRESULT FTMC3_StartPartitionDataLoss(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION* invokeDataLossDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FTMC3_StartPartitionDataLoss called with client=%p, invokeDataLossDescription=%" PRI_FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION_VALUES_OR_NULL(invokeDataLossDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartPartitionDataLoss, &IFabricTestManagementClient3::EndStartPartitionDataLoss,
        timeoutMilliseconds,
        invokeDataLossDescription);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FTMC3_StartPartitionDataLoss failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FTMC3_StartPartitionDataLoss succeeded");
    }
    return result;
}

HRESULT FTMC3_GetPartitionDataLossProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricPartitionDataLossProgressResult** result)
{
    LogVerbose("FTMC3_GetPartitionDataLossProgress called with client=%p, operationId=%" PRI_GUID ", timeoutMilliseconds=%" PRIu32 "",
        client, GUID_VALUES(operationId), timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetPartitionDataLossProgress, &IFabricTestManagementClient3::EndGetPartitionDataLossProgress,
        timeoutMilliseconds,
        result,
        operationId);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FTMC3_GetPartitionDataLossProgress failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FTMC3_GetPartitionDataLossProgress succeeded with result=%p", *result);
    }
    return hresult;
}

HRESULT FTMC3_StartPartitionQuorumLoss(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION* invokeQuorumLossDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FTMC3_StartPartitionQuorumLoss called with client=%p, invokeQuorumLossDescription=%" PRI_FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION_VALUES_OR_NULL(invokeQuorumLossDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartPartitionQuorumLoss, &IFabricTestManagementClient3::EndStartPartitionQuorumLoss,
        timeoutMilliseconds,
        invokeQuorumLossDescription);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FTMC3_StartPartitionQuorumLoss failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FTMC3_StartPartitionQuorumLoss succeeded");
    }
    return result;
}

HRESULT FTMC3_GetPartitionQuorumLossProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricPartitionQuorumLossProgressResult** result)
{
    LogVerbose("FTMC3_GetPartitionQuorumLossProgress called with client=%p, operationId=%" PRI_GUID ", timeoutMilliseconds=%" PRIu32 "",
        client, GUID_VALUES(operationId), timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetPartitionQuorumLossProgress, &IFabricTestManagementClient3::EndGetPartitionQuorumLossProgress,
        timeoutMilliseconds,
        result,
        operationId);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FTMC3_GetPartitionQuorumLossProgress failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FTMC3_GetPartitionQuorumLossProgress succeeded with result=%p", *result);
    }
    return hresult;
}

HRESULT FTMC3_StartPartitionRestart(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_PARTITION_RESTART_DESCRIPTION* restartPartitionDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FTMC3_StartPartitionRestart called with client=%p, restartPartitionDescription=%" PRI_FABRIC_START_PARTITION_RESTART_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_START_PARTITION_RESTART_DESCRIPTION_VALUES_OR_NULL(restartPartitionDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartPartitionRestart, &IFabricTestManagementClient3::EndStartPartitionRestart,
        timeoutMilliseconds,
        restartPartitionDescription);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FTMC3_StartPartitionRestart failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FTMC3_StartPartitionRestart succeeded");
    }
    return result;
}

HRESULT FTMC3_GetPartitionRestartProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricPartitionRestartProgressResult** result)
{
    LogVerbose("FTMC3_GetPartitionRestartProgress called with client=%p, operationId=%" PRI_GUID ", timeoutMilliseconds=%" PRIu32 "",
        client, GUID_VALUES(operationId), timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetPartitionRestartProgress, &IFabricTestManagementClient3::EndGetPartitionRestartProgress,
        timeoutMilliseconds,
        result,
        operationId);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FTMC3_GetPartitionRestartProgress failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FTMC3_GetPartitionRestartProgress succeeded with result=%p", *result);
    }
    return hresult;
}

HRESULT FTMC3_GetTestCommandStatusList(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_TEST_COMMAND_LIST_DESCRIPTION* operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricTestCommandStatusResult** result)
{
    LogVerbose("FTMC3_GetTestCommandStatusList called with client=%p, operationId=%" PRI_FABRIC_TEST_COMMAND_LIST_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_TEST_COMMAND_LIST_DESCRIPTION_VALUES_OR_NULL(operationId), timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetTestCommandStatusList, &IFabricTestManagementClient3::EndGetTestCommandStatusList,
        timeoutMilliseconds,
        result,
        operationId);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FTMC3_GetTestCommandStatusList failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FTMC3_GetTestCommandStatusList succeeded with result=%p", *result);
    }
    return hresult;
}

HRESULT FTMC3_CancelTestCommand(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION* invokeDataLossDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FTMC3_CancelTestCommand called with client=%p, invokeDataLossDescription=%" PRI_FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION_VALUES_OR_NULL(invokeDataLossDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginCancelTestCommand, &IFabricTestManagementClient3::EndCancelTestCommand,
        timeoutMilliseconds,
        invokeDataLossDescription);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FTMC3_CancelTestCommand failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FTMC3_CancelTestCommand succeeded");
    }
    return result;
}

HRESULT FTMC3_StartChaos(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_START_CHAOS_DESCRIPTION* restartPartitionDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FTMC3_StartChaos called with client=%p, restartPartitionDescription=%" PRI_FABRIC_START_CHAOS_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_START_CHAOS_DESCRIPTION_VALUES_OR_NULL(restartPartitionDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartChaos, &IFabricTestManagementClient3::EndStartChaos,
        timeoutMilliseconds,
        restartPartitionDescription);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FTMC3_StartChaos failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FTMC3_StartChaos succeeded");
    }
    return result;
}

HRESULT FTMC3_StopChaos(IFabricTestManagementClient3* client,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FTMC3_StopChaos called with client=%p, timeoutMilliseconds=%" PRIu32 "",
        client, timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStopChaos, &IFabricTestManagementClient3::EndStopChaos,
        timeoutMilliseconds);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FTMC3_StopChaos failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FTMC3_StopChaos succeeded");
    }
    return result;
}

HRESULT FTMC3_GetChaosReport(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_GET_CHAOS_REPORT_DESCRIPTION* getChaosReportDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricChaosReportResult** result)
{
    LogVerbose("FTMC3_GetChaosReport called with client=%p, getChaosReportDescription=%" PRI_FABRIC_GET_CHAOS_REPORT_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_GET_CHAOS_REPORT_DESCRIPTION_VALUES_OR_NULL(getChaosReportDescription), timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetChaosReport, &IFabricTestManagementClient3::EndGetChaosReport,
        timeoutMilliseconds,
        result,
        getChaosReportDescription);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FTMC3_GetChaosReport failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FTMC3_GetChaosReport succeeded with result=%p", *result);
    }
    return hresult;
}

HRESULT FTMC3_StartNodeTransition(IFabricTestManagementClient3* client,
    /* [in] */ const FABRIC_NODE_TRANSITION_DESCRIPTION* description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FTMC3_StartNodeTransition called with client=%p, description=%" PRI_FABRIC_NODE_TRANSITION_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_NODE_TRANSITION_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricTestManagementClient3::BeginStartNodeTransition, &IFabricTestManagementClient3::EndStartNodeTransition,
        timeoutMilliseconds,
        description);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FTMC3_StartNodeTransition failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FTMC3_StartNodeTransition succeeded");
    }
    return result;
}

HRESULT FTMC3_GetNodeTransitionProgress(IFabricTestManagementClient3* client,
    /* [in] */ FABRIC_TEST_COMMAND_OPERATION_ID operationId,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricNodeTransitionProgressResult** result)
{
    LogVerbose("FTMC3_GetNodeTransitionProgress called with client=%p, operationId=%" PRI_GUID ", timeoutMilliseconds=%" PRIu32 "",
        client, GUID_VALUES(operationId), timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricTestManagementClient3::BeginGetNodeTransitionProgress, &IFabricTestManagementClient3::EndGetNodeTransitionProgress,
        timeoutMilliseconds,
        result,
        operationId);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FTMC3_GetNodeTransitionProgress failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FTMC3_GetNodeTransitionProgress succeeded with result=%p", *result);
    }
    return hresult;
}
