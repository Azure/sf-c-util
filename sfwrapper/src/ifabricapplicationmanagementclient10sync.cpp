// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"
#include "sf_c_util/servicefabric_enums_to_strings.h"
#include "ifabricapplicationmanagementclient10sync.h"

#include "servicefabricdox.h"

// Format specifiers for FABRIC_APPLICATION_PARAMETER
#define PRI_FABRIC_APPLICATION_PARAMETER "s(FABRIC_APPLICATION_PARAMETER){ .Name=%ls, .Value=%ls, .Reserved=%p }"
#define FABRIC_APPLICATION_PARAMETER_VALUES(param) \
    "", \
    (param).Name, \
    (param).Value, \
    (param).Reserved

#define FABRIC_APPLICATION_PARAMETER_VALUES_OR_NULL(param) \
    "", \
    ((param) == NULL) ? L"NULL" : (param)->Name, \
    ((param) == NULL) ? L"NULL" : (param)->Value, \
    ((param) == NULL) ? NULL : (param)->Reserved

// Format specifiers for FABRIC_APPLICATION_PARAMETER_LIST
#define PRI_FABRIC_APPLICATION_PARAMETER_LIST "s(FABRIC_APPLICATION_PARAMETER_LIST){ .Count=%" PRIu32 ", .Items=%p }"
#define FABRIC_APPLICATION_PARAMETER_LIST_VALUES(list) \
    "", \
    (uint32_t)(list).Count, \
    (list).Items

#define FABRIC_APPLICATION_PARAMETER_LIST_VALUES_OR_NULL(list) \
    "", \
    ((list) == NULL) ? 0 : (uint32_t)(list)->Count, \
    ((list) == NULL) ? NULL : (list)->Items

// Format specifiers for FABRIC_APPLICATION_DESCRIPTION
#define PRI_FABRIC_APPLICATION_DESCRIPTION "s(FABRIC_APPLICATION_DESCRIPTION){ .ApplicationName=%ls, .ApplicationTypeName=%ls, .ApplicationTypeVersion=%ls, .ApplicationParameters=%" PRI_FABRIC_APPLICATION_PARAMETER_LIST ", .Reserved=%p }"
#define FABRIC_APPLICATION_DESCRIPTION_VALUES(desc) \
    "", \
    (desc).ApplicationName, \
    (desc).ApplicationTypeName, \
    (desc).ApplicationTypeVersion, \
    FABRIC_APPLICATION_PARAMETER_LIST_VALUES_OR_NULL((desc).ApplicationParameters), \
    (desc).Reserved

#define FABRIC_APPLICATION_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeVersion, \
    FABRIC_APPLICATION_PARAMETER_LIST_VALUES_OR_NULL(((desc) == NULL) ? NULL : (desc)->ApplicationParameters), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_APPLICATION_UPGRADE_DESCRIPTION
#define PRI_FABRIC_APPLICATION_UPGRADE_DESCRIPTION "s(FABRIC_APPLICATION_UPGRADE_DESCRIPTION){ .ApplicationName=%ls, .TargetApplicationTypeVersion=%ls, .ApplicationParameters=%" PRI_FABRIC_APPLICATION_PARAMETER_LIST ", .UpgradeKind=%" PRI_MU_ENUM ", .UpgradePolicyDescription=%p, .Reserved=%p }"
#define FABRIC_APPLICATION_UPGRADE_DESCRIPTION_VALUES(desc) \
    "", \
    (desc).ApplicationName, \
    (desc).TargetApplicationTypeVersion, \
    FABRIC_APPLICATION_PARAMETER_LIST_VALUES_OR_NULL((desc).ApplicationParameters), \
    MU_ENUM_VALUE(FABRIC_APPLICATION_UPGRADE_KIND, (desc).UpgradeKind), \
    (desc).UpgradePolicyDescription, \
    (desc).Reserved

#define FABRIC_APPLICATION_UPGRADE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->TargetApplicationTypeVersion, \
    FABRIC_APPLICATION_PARAMETER_LIST_VALUES_OR_NULL(((desc) == NULL) ? NULL : (desc)->ApplicationParameters), \
    MU_ENUM_VALUE(FABRIC_APPLICATION_UPGRADE_KIND, ((desc) == NULL) ? FABRIC_APPLICATION_UPGRADE_KIND_INVALID : (desc)->UpgradeKind), \
    ((desc) == NULL) ? NULL : (desc)->UpgradePolicyDescription, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION
#define PRI_FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION "s(FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION){ .ApplicationName=%ls, .UpgradeKind=%" PRI_MU_ENUM ", .UpdateFlags=%" PRIu32 ", .UpgradePolicyDescription=%p, .Reserved=%p }"
#define FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    MU_ENUM_VALUE(FABRIC_APPLICATION_UPGRADE_KIND, ((desc) == NULL) ? FABRIC_APPLICATION_UPGRADE_KIND_INVALID : (desc)->UpgradeKind), \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->UpdateFlags, \
    ((desc) == NULL) ? NULL : (desc)->UpgradePolicyDescription, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION
#define PRI_FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION "s(FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION){ .NodeName=%ls, .ApplicationName=%ls, .ServiceManifestName=%ls, .CodePackageName=%ls, .CodePackageInstanceId=%" PRId64 ", .Reserved=%p }"
#define FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceManifestName, \
    ((desc) == NULL) ? L"NULL" : (desc)->CodePackageName, \
    ((desc) == NULL) ? 0 : (int64_t)(desc)->CodePackageInstanceId, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_PACKAGE_SHARING_POLICY_LIST
#define PRI_FABRIC_PACKAGE_SHARING_POLICY_LIST "s(FABRIC_PACKAGE_SHARING_POLICY_LIST){ .Count=%" PRIu32 ", .Items=%p }"
#define FABRIC_PACKAGE_SHARING_POLICY_LIST_VALUES_OR_NULL(list) \
    "", \
    ((list) == NULL) ? 0 : (uint32_t)(list)->Count, \
    ((list) == NULL) ? NULL : (list)->Items

// Format specifiers for FABRIC_APPLICATION_UPDATE_DESCRIPTION
#define PRI_FABRIC_APPLICATION_UPDATE_DESCRIPTION "s(FABRIC_APPLICATION_UPDATE_DESCRIPTION){ .Flags=%" PRIu32 ", .ApplicationName=%ls, .RemoveApplicationCapacity=%" PRI_BOOL ", .MaximumNodes=%" PRIu32 ", .MinimumNodes=%" PRIu32 ", .Metrics=%p, .Reserved=%p }"
#define FABRIC_APPLICATION_UPDATE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->Flags, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->RemoveApplicationCapacity), \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->MaximumNodes, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->MinimumNodes, \
    ((desc) == NULL) ? NULL : (desc)->Metrics, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_DELETE_APPLICATION_DESCRIPTION
#define PRI_FABRIC_DELETE_APPLICATION_DESCRIPTION "s(FABRIC_DELETE_APPLICATION_DESCRIPTION){ .ApplicationName=%ls, .ForceDelete=%" PRI_BOOL ", .Reserved=%p }"
#define FABRIC_DELETE_APPLICATION_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->ForceDelete), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION
#define PRI_FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION "s(FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION){ .BuildPath=%ls, .Async=%" PRI_BOOL ", .Reserved=%p }"
#define FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->BuildPath, \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->Async), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION
#define PRI_FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION "s(FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION){ .ApplicationTypeName=%ls, .ApplicationTypeVersion=%ls, .Async=%" PRI_BOOL ", .Reserved=%p }"
#define FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeVersion, \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->Async), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

// Format specifiers for FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE
#define PRI_FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE "s(FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"
#define FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_PROVISION_APPLICATION_TYPE_KIND, ((desc) == NULL) ? FABRIC_PROVISION_APPLICATION_TYPE_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value



HRESULT FAMC10_ProvisionApplicationType(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationBuildPath,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_ProvisionApplicationType called with This=%p, applicationBuildPath=%ls, timeoutMilliseconds=%" PRIu32 "",
        This, applicationBuildPath, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginProvisionApplicationType, &IFabricApplicationManagementClient10::EndProvisionApplicationType,
        timeoutMilliseconds,
        applicationBuildPath);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_ProvisionApplicationType returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_ProvisionApplicationType returning");
    }
    return result;
}

HRESULT FAMC10_CreateApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_CreateApplication called with This=%p, description=%" PRI_FABRIC_APPLICATION_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_APPLICATION_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginCreateApplication, &IFabricApplicationManagementClient10::EndCreateApplication,
        timeoutMilliseconds,
        description);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_CreateApplication returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_CreateApplication returning");
    }
    return result;
}

HRESULT FAMC10_UpgradeApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_UPGRADE_DESCRIPTION *upgradeDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_UpgradeApplication called with This=%p, upgradeDescription=%" PRI_FABRIC_APPLICATION_UPGRADE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_APPLICATION_UPGRADE_DESCRIPTION_VALUES_OR_NULL(upgradeDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUpgradeApplication, &IFabricApplicationManagementClient10::EndUpgradeApplication,
        timeoutMilliseconds,
        upgradeDescription);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_UpgradeApplication returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_UpgradeApplication returning");
    }
    return result;
}

HRESULT FAMC10_GetApplicationUpgradeProgress(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricApplicationUpgradeProgressResult2 **result)
{
    LogVerbose("FAMC10_GetApplicationUpgradeProgress called with This=%p, applicationName=%ls, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, applicationName, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricApplicationManagementClient10::BeginGetApplicationUpgradeProgress, &IFabricApplicationManagementClient10::EndGetApplicationUpgradeProgress,
        timeoutMilliseconds,
        result,
        applicationName
        );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FAMC10_GetApplicationUpgradeProgress returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FAMC10_GetApplicationUpgradeProgress returning");
    }
    return hresult;
}

HRESULT FAMC10_MoveNextApplicationUpgradeDomain(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ IFabricApplicationUpgradeProgressResult2 *progress,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_MoveNextApplicationUpgradeDomain called with This=%p, progress=%p, timeoutMilliseconds=%" PRIu32 "",
        This, progress, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginMoveNextApplicationUpgradeDomain, &IFabricApplicationManagementClient10::EndMoveNextApplicationUpgradeDomain,
        timeoutMilliseconds,
        progress);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_MoveNextApplicationUpgradeDomain returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_MoveNextApplicationUpgradeDomain returning");
    }
    return result;
}

HRESULT FAMC10_DeleteApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_DeleteApplication called with This=%p, applicationName=%ls, timeoutMilliseconds=%" PRIu32 "",
        This, applicationName, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginDeleteApplication, &IFabricApplicationManagementClient10::EndDeleteApplication,
        timeoutMilliseconds,
        applicationName);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_DeleteApplication returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_DeleteApplication returning");
    }
    return result;
}

HRESULT FAMC10_UnprovisionApplicationType(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_UnprovisionApplicationType called with This=%p, applicationTypeName=%ls, applicationTypeVersion=%ls, timeoutMilliseconds=%" PRIu32 "",
        This, applicationTypeName, applicationTypeVersion, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUnprovisionApplicationType, &IFabricApplicationManagementClient10::EndUnprovisionApplicationType,
        timeoutMilliseconds,
        applicationTypeName,
        applicationTypeVersion
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_UnprovisionApplicationType returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_UnprovisionApplicationType returning");
    }
    return result;
}

HRESULT FAMC10_GetApplicationManifest(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    LogVerbose("FAMC10_GetApplicationManifest called with This=%p, applicationTypeName=%ls, applicationTypeVersion=%ls, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, applicationTypeName, applicationTypeVersion, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricApplicationManagementClient10::BeginGetApplicationManifest, &IFabricApplicationManagementClient10::EndGetApplicationManifest,
        timeoutMilliseconds,
        result,
        applicationTypeName,
        applicationTypeVersion
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FAMC10_GetApplicationManifest returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FAMC10_GetApplicationManifest returning");
    }
    return hresult;
}

HRESULT FAMC10_MoveNextApplicationUpgradeDomain2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ LPCWSTR nextUpgradeDomain,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_MoveNextApplicationUpgradeDomain2 called with This=%p, applicationName=%ls, nextUpgradeDomain=%ls, timeoutMilliseconds=%" PRIu32 "",
        This, applicationName, nextUpgradeDomain, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginMoveNextApplicationUpgradeDomain2, &IFabricApplicationManagementClient10::EndMoveNextApplicationUpgradeDomain2,
        timeoutMilliseconds,
        applicationName,
        nextUpgradeDomain
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_MoveNextApplicationUpgradeDomain2 returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_MoveNextApplicationUpgradeDomain2 returning");
    }
    return result;
}

HRESULT FAMC10_UpdateApplicationUpgrade(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_UpdateApplicationUpgrade called with This=%p, description=%" PRI_FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUpdateApplicationUpgrade, &IFabricApplicationManagementClient10::EndUpdateApplicationUpgrade,
        timeoutMilliseconds,
        description
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_UpdateApplicationUpgrade returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_UpdateApplicationUpgrade returning");
    }
    return result;
}

HRESULT FAMC10_RestartDeployedCodePackage(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION *restartCodePackageDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_RestartDeployedCodePackage called with This=%p, restartCodePackageDescription=%" PRI_FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION_VALUES_OR_NULL(restartCodePackageDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginRestartDeployedCodePackage, &IFabricApplicationManagementClient10::EndRestartDeployedCodePackage,
        timeoutMilliseconds,
        restartCodePackageDescription
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_RestartDeployedCodePackage returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_RestartDeployedCodePackage returning");
    }
    return result;
}

HRESULT FAMC10_CopyApplicationPackage(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR applicationPackagePath,
    /* [in] */ LPCWSTR applicationPackagePathInImageStore)
{
    LogVerbose("FAMC10_CopyApplicationPackage called with This=%p, imageStoreConnectionString=%ls, applicationPackagePath=%ls, applicationPackagePathInImageStore=%ls",
        This, imageStoreConnectionString, applicationPackagePath, applicationPackagePathInImageStore);

    HRESULT result = This->CopyApplicationPackage(imageStoreConnectionString, applicationPackagePath, applicationPackagePathInImageStore);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_CopyApplicationPackage returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_CopyApplicationPackage returning");
    }
    return result;
}

HRESULT FAMC10_RemoveApplicationPackage(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR imageStoreConnectionString,
    /* [in] */ LPCWSTR applicationPackagePathInImageStore)
{
    LogVerbose("FAMC10_RemoveApplicationPackage called with This=%p, imageStoreConnectionString=%ls, applicationPackagePathInImageStore=%ls",
        This, imageStoreConnectionString, applicationPackagePathInImageStore);

    HRESULT result = This->RemoveApplicationPackage(imageStoreConnectionString, applicationPackagePathInImageStore);

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_RemoveApplicationPackage returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_RemoveApplicationPackage returning");
    }
    return result;
}

HRESULT FAMC10_DeployServicePackageToNode(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ LPCWSTR serviceManifestName,
    /* [in] */ const FABRIC_PACKAGE_SHARING_POLICY_LIST *sharingPolicy,
    /* [in] */ LPCWSTR nodeName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_DeployServicePackageToNode called with This=%p, applicationTypeName=%ls, applicationTypeVersion=%ls, serviceManifestName=%ls, sharingPolicy=%" PRI_FABRIC_PACKAGE_SHARING_POLICY_LIST ", nodeName=%ls, timeoutMilliseconds=%" PRIu32 "",
        This, applicationTypeName, applicationTypeVersion, serviceManifestName, FABRIC_PACKAGE_SHARING_POLICY_LIST_VALUES_OR_NULL(sharingPolicy), nodeName, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginDeployServicePackageToNode, &IFabricApplicationManagementClient10::EndDeployServicePackageToNode,
        timeoutMilliseconds,
        applicationTypeName,
        applicationTypeVersion,
        serviceManifestName,
        sharingPolicy,
        nodeName
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_DeployServicePackageToNode returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_DeployServicePackageToNode returning");
    }
    return result;
}

HRESULT FAMC10_RollbackApplicationUpgrade(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ FABRIC_URI applicationName,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_RollbackApplicationUpgrade called with This=%p, applicationName=%ls, timeoutMilliseconds=%" PRIu32 "",
        This, applicationName, timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginRollbackApplicationUpgrade, &IFabricApplicationManagementClient10::EndRollbackApplicationUpgrade,
        timeoutMilliseconds,
        applicationName
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_RollbackApplicationUpgrade returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_RollbackApplicationUpgrade returning");
    }
    return result;
}

HRESULT FAMC10_UpdateApplication(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_UPDATE_DESCRIPTION *applicationUpdateDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_UpdateApplication called with This=%p, applicationUpdateDescription=%" PRI_FABRIC_APPLICATION_UPDATE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_APPLICATION_UPDATE_DESCRIPTION_VALUES_OR_NULL(applicationUpdateDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUpdateApplication, &IFabricApplicationManagementClient10::EndUpdateApplication,
        timeoutMilliseconds,
        applicationUpdateDescription
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_UpdateApplication returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_UpdateApplication returning");
    }
    return result;
}

HRESULT FAMC10_DeleteApplication2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_DELETE_APPLICATION_DESCRIPTION *deleteDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_DeleteApplication2 called with This=%p, deleteDescription=%" PRI_FABRIC_DELETE_APPLICATION_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_DELETE_APPLICATION_DESCRIPTION_VALUES_OR_NULL(deleteDescription), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginDeleteApplication2, &IFabricApplicationManagementClient10::EndDeleteApplication2,
        timeoutMilliseconds,
        deleteDescription
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_DeleteApplication2 returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_DeleteApplication2 returning");
    }
    return result;
}

HRESULT FAMC10_ProvisionApplicationType2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_ProvisionApplicationType2 called with This=%p, description=%" PRI_FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginProvisionApplicationType2, &IFabricApplicationManagementClient10::EndProvisionApplicationType2,
        timeoutMilliseconds,
        description
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_ProvisionApplicationType2 returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_ProvisionApplicationType2 returning");
    }
    return result;
}

HRESULT FAMC10_UnprovisionApplicationType2(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_UnprovisionApplicationType2 called with This=%p, description=%" PRI_FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginUnprovisionApplicationType2, &IFabricApplicationManagementClient10::EndUnprovisionApplicationType2,
        timeoutMilliseconds,
        description
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_UnprovisionApplicationType2 returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_UnprovisionApplicationType2 returning");
    }
    return result;
}

HRESULT FAMC10_ProvisionApplicationType3(
    IFabricApplicationManagementClient10 * This,
    /* [in] */ const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FAMC10_ProvisionApplicationType3 called with This=%p, description=%" PRI_FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE_VALUES_OR_NULL(description), timeoutMilliseconds);

    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricApplicationManagementClient10::BeginProvisionApplicationType3, &IFabricApplicationManagementClient10::EndProvisionApplicationType3,
        timeoutMilliseconds,
        description
    );

    if (FAILED(result))
    {
        LogHRESULTError(result, "FAMC10_ProvisionApplicationType3 returning");
    }
    else
    {
        LogHRESULTVerbose(result, "FAMC10_ProvisionApplicationType3 returning");
    }
    return result;
}
