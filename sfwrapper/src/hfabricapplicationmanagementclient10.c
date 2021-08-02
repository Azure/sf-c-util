// Copyright (C) Microsoft Corporation. All rights reserved.

#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

#include "windows.h"

#include "fabrictypes.h"
#include "fabricclient.h"

#include "h_fabric_macro_generator.h"
#include "ifabricapplicationmanagementclient10sync.h"

#include "hfabricapplicationmanagementclient10.h"

H_FABRIC_DEFINE_STRUCT_TYPE(IFabricApplicationManagementClient10);

H_FABRIC_DEFINE_CREATE(IFabricApplicationManagementClient10);
H_FABRIC_DEFINE_DESTROY(IFabricApplicationManagementClient10);

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_ProvisionApplicationType,
    IN_ARGS(
        ARG(LPCWSTR, applicationBuildPath),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_CreateApplication,
    IN_ARGS(
        ARG(const FABRIC_APPLICATION_DESCRIPTION*, description),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_UpgradeApplication,
    IN_ARGS(
        ARG(const FABRIC_APPLICATION_UPGRADE_DESCRIPTION*, upgradeDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_GetApplicationUpgradeProgress,
    IN_ARGS(
        ARG(FABRIC_URI, applicationName),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricApplicationUpgradeProgressResult2**, result)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_MoveNextApplicationUpgradeDomain,
    IN_ARGS(
        ARG(IFabricApplicationUpgradeProgressResult2*, progress),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API_WITH_RESULTS(IFabricApplicationManagementClient10, FAMC10_DeleteApplication,
    IN_ARGS(
        ARG(FABRIC_URI, applicationName),
        ARG(DWORD, timeoutMilliseconds)
    ),
    RESULTS(FABRIC_E_APPLICATION_NOT_FOUND)
)

H_FABRIC_DEFINE_API_WITH_RESULTS(IFabricApplicationManagementClient10, FAMC10_UnprovisionApplicationType,
    IN_ARGS(
        ARG(LPCWSTR, applicationTypeName),
        ARG(LPCWSTR, applicationTypeVersion),
        ARG(DWORD, timeoutMilliseconds)
    ),
    RESULTS(FABRIC_E_APPLICATION_TYPE_NOT_FOUND)
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_GetApplicationManifest,
    IN_ARGS(
        ARG(LPCWSTR, applicationTypeName),
        ARG(LPCWSTR, applicationTypeVersion),
        ARG(DWORD, timeoutMilliseconds),
        ARG(IFabricStringResult**, result)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_MoveNextApplicationUpgradeDomain2,
    IN_ARGS(
        ARG(LPCWSTR, applicationName),
        ARG(LPCWSTR, nextUpgradeDomain),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_UpdateApplicationUpgrade,
    IN_ARGS(
        ARG(const FABRIC_APPLICATION_UPGRADE_UPDATE_DESCRIPTION*, description),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_RestartDeployedCodePackage,
    IN_ARGS(
        ARG(const FABRIC_RESTART_DEPLOYED_CODE_PACKAGE_DESCRIPTION*, restartCodePackageDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API_NO_SF_TIMEOUT(IFabricApplicationManagementClient10, FAMC10_CopyApplicationPackage,
    IN_ARGS(
        ARG(LPCWSTR, imageStoreConnectionString),
        ARG(LPCWSTR, applicationPackagePath),
        ARG(LPCWSTR, applicationPackagePathInImageStore)
    )
)

H_FABRIC_DEFINE_API_NO_SF_TIMEOUT(IFabricApplicationManagementClient10, FAMC10_RemoveApplicationPackage,
    IN_ARGS(
        ARG(LPCWSTR, imageStoreConnectionString),
        ARG(LPCWSTR, applicationPackagePathInImageStore)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_DeployServicePackageToNode,
    IN_ARGS(
        ARG(LPCWSTR, applicationTypeName),
        ARG(LPCWSTR, applicationTypeVersion),
        ARG(LPCWSTR, serviceManifestName),
        ARG(const FABRIC_PACKAGE_SHARING_POLICY_LIST*, sharingPolicy),
        ARG(LPCWSTR, nodeName),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_RollbackApplicationUpgrade,
    IN_ARGS(
        ARG(FABRIC_URI, applicationName),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_UpdateApplication,
    IN_ARGS(
        ARG(const FABRIC_APPLICATION_UPDATE_DESCRIPTION*, applicationUpdateDescription),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API_WITH_RESULTS(IFabricApplicationManagementClient10, FAMC10_DeleteApplication2,
    IN_ARGS(
        ARG(const FABRIC_DELETE_APPLICATION_DESCRIPTION*, deleteDescription),
        ARG(DWORD, timeoutMilliseconds)
    ),
    RESULTS(FABRIC_E_APPLICATION_NOT_FOUND)
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_ProvisionApplicationType2,
    IN_ARGS(
        ARG(const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION*, description),
        ARG(DWORD, timeoutMilliseconds)
    )
)

H_FABRIC_DEFINE_API_WITH_RESULTS(IFabricApplicationManagementClient10, FAMC10_UnprovisionApplicationType2,
    IN_ARGS(
        ARG(const FABRIC_UNPROVISION_APPLICATION_TYPE_DESCRIPTION*, description),
        ARG(DWORD, timeoutMilliseconds)
    ),
    RESULTS(FABRIC_E_APPLICATION_TYPE_NOT_FOUND)
)

H_FABRIC_DEFINE_API(IFabricApplicationManagementClient10, FAMC10_ProvisionApplicationType3,
    IN_ARGS(
        ARG(const FABRIC_PROVISION_APPLICATION_TYPE_DESCRIPTION_BASE*, description),
        ARG(DWORD, timeoutMilliseconds)
    )
)
