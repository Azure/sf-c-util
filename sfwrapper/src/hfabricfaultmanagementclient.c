// Copyright (C) Microsoft Corporation. All rights reserved.

#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

#include "windows.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "h_fabric_macro_generator.h"
#include "ifabricfaultmanagementclientsync.h"

#include "hfabricfaultmanagementclient.h"

H_FABRIC_DEFINE_STRUCT_TYPE(IFabricFaultManagementClient);

H_FABRIC_DEFINE_CREATE(IFabricFaultManagementClient);
H_FABRIC_DEFINE_DESTROY(IFabricFaultManagementClient);

H_FABRIC_DEFINE_API(IFabricFaultManagementClient, FFMC_RestartNode,
    IN_ARGS(
        ARG(const FABRIC_RESTART_NODE_DESCRIPTION2*, description),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricRestartNodeResult**, fabricRestartNodeResult)
    )
)

H_FABRIC_DEFINE_API(IFabricFaultManagementClient, FFMC_StartNode,
    IN_ARGS(
        ARG(const FABRIC_START_NODE_DESCRIPTION2*, description),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricStartNodeResult**, fabricStartNodeResult)
    )
)

H_FABRIC_DEFINE_API(IFabricFaultManagementClient, FFMC_StopNode,
    IN_ARGS(
        ARG(const FABRIC_STOP_NODE_DESCRIPTION2*, description),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricStopNodeResult**, fabricStopNodeResult)
    )
)

H_FABRIC_DEFINE_API(IFabricFaultManagementClient, FFMC_RestartDeployedCodePackage,
    IN_ARGS(
        ARG(const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2*, description),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricRestartDeployedCodePackageResult**, fabricRestartDeployedCodePackageResult)
    )
)

H_FABRIC_DEFINE_API(IFabricFaultManagementClient, FFMC_MovePrimary,
    IN_ARGS(
        ARG(const FABRIC_MOVE_PRIMARY_DESCRIPTION2*, description),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricMovePrimaryResult**, fabricMovePrimaryResult)
    )
)

H_FABRIC_DEFINE_API(IFabricFaultManagementClient, FFMC_MoveSecondary,
    IN_ARGS(
        ARG(const FABRIC_MOVE_SECONDARY_DESCRIPTION2*, description),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricMoveSecondaryResult**, fabricMoveSecondaryResult)
    )
)
