// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"

#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"
#include "mycallback.h"
#include "servicefabricdox.h"

#include "ifabricfaultmanagementclientsync.h"

HRESULT FFMC_RestartNode(IFabricFaultManagementClient* client,
    const FABRIC_RESTART_NODE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricRestartNodeResult **fabricRestartNodeResult)
{
    return ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginRestartNode, &IFabricFaultManagementClient::EndRestartNode,
        timeoutMilliseconds,
        fabricRestartNodeResult,
        description);
}

HRESULT FFMC_StartNode(IFabricFaultManagementClient* client,
    const FABRIC_START_NODE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricStartNodeResult **fabricStartNodeResult)
{
    return ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginStartNode, &IFabricFaultManagementClient::EndStartNode,
        timeoutMilliseconds,
        fabricStartNodeResult,
        description);
}

HRESULT FFMC_StopNode(IFabricFaultManagementClient* client,
    const FABRIC_STOP_NODE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricStopNodeResult **fabricStopNodeResult)
{
    return ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginStopNode, &IFabricFaultManagementClient::EndStopNode,
        timeoutMilliseconds,
        fabricStopNodeResult,
        description);
}

HRESULT FFMC_RestartDeployedCodePackage(IFabricFaultManagementClient* client,
    const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricRestartDeployedCodePackageResult **fabricRestartDeployedCodePackageResult)
{
    return ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginRestartDeployedCodePackage, &IFabricFaultManagementClient::EndRestartDeployedCodePackage,
        timeoutMilliseconds,
        fabricRestartDeployedCodePackageResult,
        description);
}

HRESULT FFMC_MovePrimary(IFabricFaultManagementClient* client,
    const FABRIC_MOVE_PRIMARY_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricMovePrimaryResult **fabricMovePrimaryResult)
{
    return ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginMovePrimary, &IFabricFaultManagementClient::EndMovePrimary,
        timeoutMilliseconds,
        fabricMovePrimaryResult,
        description);
}

HRESULT FFMC_MoveSecondary(IFabricFaultManagementClient* client,
    const FABRIC_MOVE_SECONDARY_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricMoveSecondaryResult **fabricMoveSecondaryResult)
{
    return ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginMoveSecondary, &IFabricFaultManagementClient::EndMoveSecondary,
        timeoutMilliseconds,
        fabricMoveSecondaryResult,
        description);
}
