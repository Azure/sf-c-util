// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"

#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"
#include "sf_c_util/servicefabric_enums_to_strings.h"
#include "mycallback.h"
#include "servicefabricdox.h"

#include "ifabricfaultmanagementclientsync.h"

/* Format specifiers for FABRIC_RESTART_NODE_DESCRIPTION2 */
#define PRI_FABRIC_RESTART_NODE_DESCRIPTION2 "s(FABRIC_RESTART_NODE_DESCRIPTION2){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_RESTART_NODE_DESCRIPTION2_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_RESTART_NODE_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_RESTART_NODE_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

/* Format specifiers for FABRIC_START_NODE_DESCRIPTION2 */
#define PRI_FABRIC_START_NODE_DESCRIPTION2 "s(FABRIC_START_NODE_DESCRIPTION2){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_START_NODE_DESCRIPTION2_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_START_NODE_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_START_NODE_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

/* Format specifiers for FABRIC_STOP_NODE_DESCRIPTION2 */
#define PRI_FABRIC_STOP_NODE_DESCRIPTION2 "s(FABRIC_STOP_NODE_DESCRIPTION2){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_STOP_NODE_DESCRIPTION2_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_STOP_NODE_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_STOP_NODE_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

/* Format specifiers for FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2 */
#define PRI_FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2 "s(FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

/* Format specifiers for FABRIC_MOVE_PRIMARY_DESCRIPTION2 */
#define PRI_FABRIC_MOVE_PRIMARY_DESCRIPTION2 "s(FABRIC_MOVE_PRIMARY_DESCRIPTION2){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_MOVE_PRIMARY_DESCRIPTION2_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_MOVE_PRIMARY_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_MOVE_PRIMARY_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

/* Format specifiers for FABRIC_MOVE_SECONDARY_DESCRIPTION2 */
#define PRI_FABRIC_MOVE_SECONDARY_DESCRIPTION2 "s(FABRIC_MOVE_SECONDARY_DESCRIPTION2){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_MOVE_SECONDARY_DESCRIPTION2_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_MOVE_SECONDARY_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_MOVE_SECONDARY_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

HRESULT FFMC_RestartNode(IFabricFaultManagementClient* client,
    const FABRIC_RESTART_NODE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricRestartNodeResult **fabricRestartNodeResult)
{
    LogInfo("FFMC_RestartNode called with client=%p, description=%" PRI_FABRIC_RESTART_NODE_DESCRIPTION2 ", timeoutMilliseconds=%" PRIu32 ", fabricRestartNodeResult=%p",
        client, FABRIC_RESTART_NODE_DESCRIPTION2_VALUES_OR_NULL(description), timeoutMilliseconds, fabricRestartNodeResult);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginRestartNode, &IFabricFaultManagementClient::EndRestartNode,
        timeoutMilliseconds,
        fabricRestartNodeResult,
        description);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FFMC_RestartNode returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FFMC_RestartNode returning");
    }
    return hresult;
}

HRESULT FFMC_StartNode(IFabricFaultManagementClient* client,
    const FABRIC_START_NODE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricStartNodeResult **fabricStartNodeResult)
{
    LogInfo("FFMC_StartNode called with client=%p, description=%" PRI_FABRIC_START_NODE_DESCRIPTION2 ", timeoutMilliseconds=%" PRIu32 ", fabricStartNodeResult=%p",
        client, FABRIC_START_NODE_DESCRIPTION2_VALUES_OR_NULL(description), timeoutMilliseconds, fabricStartNodeResult);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginStartNode, &IFabricFaultManagementClient::EndStartNode,
        timeoutMilliseconds,
        fabricStartNodeResult,
        description);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FFMC_StartNode returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FFMC_StartNode returning");
    }
    return hresult;
}

HRESULT FFMC_StopNode(IFabricFaultManagementClient* client,
    const FABRIC_STOP_NODE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricStopNodeResult **fabricStopNodeResult)
{
    LogInfo("FFMC_StopNode called with client=%p, description=%" PRI_FABRIC_STOP_NODE_DESCRIPTION2 ", timeoutMilliseconds=%" PRIu32 ", fabricStopNodeResult=%p",
        client, FABRIC_STOP_NODE_DESCRIPTION2_VALUES_OR_NULL(description), timeoutMilliseconds, fabricStopNodeResult);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginStopNode, &IFabricFaultManagementClient::EndStopNode,
        timeoutMilliseconds,
        fabricStopNodeResult,
        description);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FFMC_StopNode returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FFMC_StopNode returning");
    }
    return hresult;
}

HRESULT FFMC_RestartDeployedCodePackage(IFabricFaultManagementClient* client,
    const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricRestartDeployedCodePackageResult **fabricRestartDeployedCodePackageResult)
{
    LogInfo("FFMC_RestartDeployedCodePackage called with client=%p, description=%" PRI_FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2 ", timeoutMilliseconds=%" PRIu32 ", fabricRestartDeployedCodePackageResult=%p",
        client, FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION2_VALUES_OR_NULL(description), timeoutMilliseconds, fabricRestartDeployedCodePackageResult);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginRestartDeployedCodePackage, &IFabricFaultManagementClient::EndRestartDeployedCodePackage,
        timeoutMilliseconds,
        fabricRestartDeployedCodePackageResult,
        description);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FFMC_RestartDeployedCodePackage returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FFMC_RestartDeployedCodePackage returning");
    }
    return hresult;
}

HRESULT FFMC_MovePrimary(IFabricFaultManagementClient* client,
    const FABRIC_MOVE_PRIMARY_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricMovePrimaryResult **fabricMovePrimaryResult)
{
    LogInfo("FFMC_MovePrimary called with client=%p, description=%" PRI_FABRIC_MOVE_PRIMARY_DESCRIPTION2 ", timeoutMilliseconds=%" PRIu32 ", fabricMovePrimaryResult=%p",
        client, FABRIC_MOVE_PRIMARY_DESCRIPTION2_VALUES_OR_NULL(description), timeoutMilliseconds, fabricMovePrimaryResult);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginMovePrimary, &IFabricFaultManagementClient::EndMovePrimary,
        timeoutMilliseconds,
        fabricMovePrimaryResult,
        description);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FFMC_MovePrimary returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FFMC_MovePrimary returning");
    }
    return hresult;
}

HRESULT FFMC_MoveSecondary(IFabricFaultManagementClient* client,
    const FABRIC_MOVE_SECONDARY_DESCRIPTION2 *description,
    DWORD timeoutMilliseconds,
    IFabricMoveSecondaryResult **fabricMoveSecondaryResult)
{
    LogInfo("FFMC_MoveSecondary called with client=%p, description=%" PRI_FABRIC_MOVE_SECONDARY_DESCRIPTION2 ", timeoutMilliseconds=%" PRIu32 ", fabricMoveSecondaryResult=%p",
        client, FABRIC_MOVE_SECONDARY_DESCRIPTION2_VALUES_OR_NULL(description), timeoutMilliseconds, fabricMoveSecondaryResult);

    HRESULT hresult = ServiceFabric_DoX(client, &IFabricFaultManagementClient::BeginMoveSecondary, &IFabricFaultManagementClient::EndMoveSecondary,
        timeoutMilliseconds,
        fabricMoveSecondaryResult,
        description);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FFMC_MoveSecondary returning");
    }
    else
    {
        LogHRESULTInfo(hresult, "FFMC_MoveSecondary returning");
    }
    return hresult;
}
