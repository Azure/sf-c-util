// Copyright (C) Microsoft Corporation. All rights reserved.

#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

#include "windows.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "h_fabric_macro_generator.h"
#include "ifabrictestmanagementclient3sync.h"

#include "hfabrictestmanagementclient3.h"

H_FABRIC_DEFINE_STRUCT_TYPE(IFabricTestManagementClient3);

H_FABRIC_DEFINE_CREATE(IFabricTestManagementClient3);
H_FABRIC_DEFINE_DESTROY(IFabricTestManagementClient3);

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_StartPartitionDataLoss,
    IN_ARGS(
        ARG(const FABRIC_START_PARTITION_DATA_LOSS_DESCRIPTION*, invokeDataLossDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_GetPartitionDataLossProgress,
    IN_ARGS(
        ARG(FABRIC_TEST_COMMAND_OPERATION_ID, operationId),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricPartitionDataLossProgressResult**, result)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_StartPartitionQuorumLoss,
    IN_ARGS(
        ARG(const FABRIC_START_PARTITION_QUORUM_LOSS_DESCRIPTION*, invokeQuorumLossDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_GetPartitionQuorumLossProgress,
    IN_ARGS(
        ARG(FABRIC_TEST_COMMAND_OPERATION_ID, operationId),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricPartitionQuorumLossProgressResult**, result)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_StartPartitionRestart,
    IN_ARGS(
        ARG(const FABRIC_START_PARTITION_RESTART_DESCRIPTION*, restartPartitionDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_GetPartitionRestartProgress,
    IN_ARGS(
        ARG(FABRIC_TEST_COMMAND_OPERATION_ID, operationId),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricPartitionRestartProgressResult**, result)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_GetTestCommandStatusList,
    IN_ARGS(
        ARG(const FABRIC_TEST_COMMAND_LIST_DESCRIPTION*, operationId),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricTestCommandStatusResult**, result)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_CancelTestCommand,
    IN_ARGS(
        ARG(const FABRIC_CANCEL_TEST_COMMAND_DESCRIPTION*, invokeDataLossDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_StartChaos,
    IN_ARGS(
        ARG(const FABRIC_START_CHAOS_DESCRIPTION*, restartPartitionDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_StopChaos,
    IN_ARGS(
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_GetChaosReport,
    IN_ARGS(
        ARG(const FABRIC_GET_CHAOS_REPORT_DESCRIPTION*, getChaosReportDescription),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricChaosReportResult**, result)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_StartNodeTransition,
    IN_ARGS(
        ARG(const FABRIC_NODE_TRANSITION_DESCRIPTION*, description),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricTestManagementClient3, FTMC3_GetNodeTransitionProgress,
    IN_ARGS(
        ARG(FABRIC_TEST_COMMAND_OPERATION_ID, operationId),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricNodeTransitionProgressResult**, result)
    )
)
