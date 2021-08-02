// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef IFABRICFAULTMANAGEMENTCLIENTSYNC_H
#define IFABRICFAULTMANAGEMENTCLIENTSYNC_H

#include "windows.h"

#include "fabricclient.h"

#include "sf_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

    HRESULT FFMC_RestartNode(IFabricFaultManagementClient* client,
        const FABRIC_RESTART_NODE_DESCRIPTION2 *description,
        DWORD timeoutMilliseconds,
        IFabricRestartNodeResult **fabricRestartNodeResult);

    HRESULT FFMC_StartNode(IFabricFaultManagementClient* client,
        const FABRIC_START_NODE_DESCRIPTION2 *description,
        DWORD timeoutMilliseconds,
        IFabricStartNodeResult **fabricStartNodeResult);

    HRESULT FFMC_StopNode(IFabricFaultManagementClient* client,
        const FABRIC_STOP_NODE_DESCRIPTION2 *description,
        DWORD timeoutMilliseconds,
        IFabricStopNodeResult **fabricStopNodeResult);

    HRESULT FFMC_RestartDeployedCodePackage(IFabricFaultManagementClient* client,
        const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2 *description,
        DWORD timeoutMilliseconds,
        IFabricRestartDeployedCodePackageResult **fabricRestartDeployedCodePackageResult);

    HRESULT FFMC_MovePrimary(IFabricFaultManagementClient* client,
        const FABRIC_MOVE_PRIMARY_DESCRIPTION2 *description,
        DWORD timeoutMilliseconds,
        IFabricMovePrimaryResult **fabricMovePrimaryResult);

    HRESULT FFMC_MoveSecondary(IFabricFaultManagementClient* client,
        const FABRIC_MOVE_SECONDARY_DESCRIPTION2 *description,
        DWORD timeoutMilliseconds,
        IFabricMoveSecondaryResult **fabricMoveSecondaryResult);

    DECLARE_CREATE_IFABRICINSTANCE(IFabricFaultManagementClient);

#ifdef __cplusplus
}
#endif


#endif /*IFABRICFAULTMANAGEMENTCLIENTSYNC_H*/

