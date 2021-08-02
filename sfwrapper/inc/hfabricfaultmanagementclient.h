// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef H_FABRIC_FAULT_MANAGEMENT_CLIENT_H
#define H_FABRIC_FAULT_MANAGEMENT_CLIENT_H

#include "windows.h"

#include "fabricclient.h"

#include "macro_utils/macro_utils.h"

#include "h_fabric_macro_generator.h"

H_FABRIC_DEFINE_TYPEDEF(IFabricFaultManagementClient);

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    H_FABRIC_DECLARE_CREATE(IFabricFaultManagementClient);
    H_FABRIC_DECLARE_DESTROY(IFabricFaultManagementClient);

    /*below are the same API as ifabricfaultmanagementclient exposes, which are the same as the methods of the IFabricFaultManagementClient interface*/
    /*note: the computed outputs (such as IFabricGetNodeListResult are "as they come" from IFabricFaultManagementClient interface*/

    H_FABRIC_DECLARE_API(IFabricFaultManagementClient, FFMC_RestartNode,
        IN_ARGS(
            ARG(const FABRIC_RESTART_NODE_DESCRIPTION2*, description),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricRestartNodeResult**, fabricRestartNodeResult)
        )
    )

    H_FABRIC_DECLARE_API(IFabricFaultManagementClient, FFMC_StartNode,
        IN_ARGS(
            ARG(const FABRIC_START_NODE_DESCRIPTION2*, description),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStartNodeResult**, fabricStartNodeResult)
        )
    )

    H_FABRIC_DECLARE_API(IFabricFaultManagementClient, FFMC_StopNode,
        IN_ARGS(
            ARG(const FABRIC_STOP_NODE_DESCRIPTION2*, description),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStopNodeResult**, fabricStopNodeResult)
        )
    )

    H_FABRIC_DECLARE_API(IFabricFaultManagementClient, FFMC_RestartDeployedCodePackage,
        IN_ARGS(
            ARG(const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2*, description),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricRestartDeployedCodePackageResult**, fabricRestartDeployedCodePackageResult)
        )
    )

    H_FABRIC_DECLARE_API(IFabricFaultManagementClient, FFMC_MovePrimary,
        IN_ARGS(
            ARG(const FABRIC_MOVE_PRIMARY_DESCRIPTION2*, description),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricMovePrimaryResult**, fabricMovePrimaryResult)
        )
    )

    H_FABRIC_DECLARE_API(IFabricFaultManagementClient, FFMC_MoveSecondary,
        IN_ARGS(
            ARG(const FABRIC_MOVE_SECONDARY_DESCRIPTION2*, description),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricMoveSecondaryResult**, fabricMoveSecondaryResult)
        )
    )

#ifdef __cplusplus
}
#endif

#endif /*H_FABRIC_FAULT_MANAGEMENT_CLIENT_H*/
