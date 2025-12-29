// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"
#include "c_pal/uuid.h"

#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"
#include "sf_c_util/servicefabric_enums_to_strings.h"
#include "mycallback.h"
#include "servicefabricdox.h"

#include "ifabricservicemanagementclient6sync.h"

/* Format specifiers for FABRIC_SERVICE_DESCRIPTION */
#define PRI_FABRIC_SERVICE_DESCRIPTION "s(FABRIC_SERVICE_DESCRIPTION){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_SERVICE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_SERVICE_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_SERVICE_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

/* Format specifiers for FABRIC_SERVICE_UPDATE_DESCRIPTION */
#define PRI_FABRIC_SERVICE_UPDATE_DESCRIPTION "s(FABRIC_SERVICE_UPDATE_DESCRIPTION){ .Kind=%" PRI_MU_ENUM ", .Value=%p }"

#define FABRIC_SERVICE_UPDATE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    MU_ENUM_VALUE(FABRIC_SERVICE_DESCRIPTION_KIND, ((desc) == NULL) ? FABRIC_SERVICE_DESCRIPTION_KIND_INVALID : (desc)->Kind), \
    ((desc) == NULL) ? NULL : (desc)->Value

/* Format specifiers for FABRIC_REMOVE_REPLICA_DESCRIPTION */
#define PRI_FABRIC_REMOVE_REPLICA_DESCRIPTION "s(FABRIC_REMOVE_REPLICA_DESCRIPTION){ .NodeName=%ls, .PartitionId=%" PRI_GUID ", .ReplicaOrInstanceId=%" PRId64 ", .Reserved=%p }"

#define FABRIC_REMOVE_REPLICA_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    ((desc) == NULL) ? 0 : (int64_t)(desc)->ReplicaOrInstanceId, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_RESTART_REPLICA_DESCRIPTION */
#define PRI_FABRIC_RESTART_REPLICA_DESCRIPTION "s(FABRIC_RESTART_REPLICA_DESCRIPTION){ .NodeName=%ls, .PartitionId=%" PRI_GUID ", .ReplicaOrInstanceId=%" PRId64 ", .Reserved=%p }"

#define FABRIC_RESTART_REPLICA_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    ((desc) == NULL) ? 0 : (int64_t)(desc)->ReplicaOrInstanceId, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION */
#define PRI_FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION "s(FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION){ .Name=%ls, .Flags=%" PRIu32 ", .Reserved=%p }"

#define FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->Name, \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->Flags, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_DELETE_SERVICE_DESCRIPTION */
#define PRI_FABRIC_DELETE_SERVICE_DESCRIPTION "s(FABRIC_DELETE_SERVICE_DESCRIPTION){ .ServiceName=%ls, .ForceDelete=%" PRI_BOOL ", .Reserved=%p }"

#define FABRIC_DELETE_SERVICE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceName, \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->ForceDelete), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION */
#define PRI_FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION "s(FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION){ .ApplicationName=%ls, .ServiceName=%ls, .ServiceDnsName=%ls, .ServiceTypeName=%ls, .ServicePackageActivationMode=%" PRI_MU_ENUM ", .InitializationDataSize=%" PRIu32 ", .InitializationData=%p, .Reserved=%p }"

#define FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceDnsName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceTypeName, \
    MU_ENUM_VALUE(FABRIC_SERVICE_PACKAGE_ACTIVATION_MODE, ((desc) == NULL) ? FABRIC_SERVICE_PACKAGE_ACTIVATION_MODE_SHARED_PROCESS : (desc)->ServicePackageActivationMode), \
    ((desc) == NULL) ? 0 : (uint32_t)(desc)->InitializationDataSize, \
    ((desc) == NULL) ? NULL : (desc)->InitializationData, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

HRESULT FSMC6_CreateService(IFabricServiceManagementClient6* client,
    const FABRIC_SERVICE_DESCRIPTION *description,
    DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_CreateService called with client=%p, description=%" PRI_FABRIC_SERVICE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, FABRIC_SERVICE_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginCreateService, &IFabricServiceManagementClient6::EndCreateService, timeoutMilliseconds, description);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_CreateService failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_CreateService succeeded");
    }
    return result;
}

HRESULT FSMC6_CreateServiceFromTemplate(IFabricServiceManagementClient6* client,
    FABRIC_URI applicationName,
    FABRIC_URI serviceName,
    LPCWSTR serviceTypeName,
    ULONG InitializationDataSize,
    BYTE *InitializationData,
    DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_CreateServiceFromTemplate called with client=%p, applicationName=%ls, serviceName=%ls, serviceTypeName=%ls, InitializationDataSize=%" PRIu32 ", InitializationData=%p, timeoutMilliseconds=%" PRIu32 "",
        client, applicationName, serviceName, serviceTypeName, (uint32_t)InitializationDataSize, InitializationData, timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginCreateServiceFromTemplate, &IFabricServiceManagementClient6::EndCreateServiceFromTemplate,
        timeoutMilliseconds,
        applicationName,
        serviceName,
        serviceTypeName,
        InitializationDataSize,
        InitializationData);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_CreateServiceFromTemplate failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_CreateServiceFromTemplate succeeded");
    }
    return result;
}

HRESULT FSMC6_DeleteService(IFabricServiceManagementClient6* client,
    FABRIC_URI name,
    DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_DeleteService called with client=%p, name=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, name, timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginDeleteService, &IFabricServiceManagementClient6::EndDeleteService,
        timeoutMilliseconds,
        name);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_DeleteService failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_DeleteService succeeded");
    }
    return result;
}

HRESULT FSMC6_GetServiceDescription(IFabricServiceManagementClient6* client,
    FABRIC_URI name,
    DWORD timeoutMilliseconds,
    IFabricServiceDescriptionResult **result)
{
    LogVerbose("FSMC6_GetServiceDescription called with client=%p, name=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, name, timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricServiceManagementClient6::BeginGetServiceDescription, &IFabricServiceManagementClient6::EndGetServiceDescription,
        timeoutMilliseconds,
        result,
        name);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FSMC6_GetServiceDescription failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FSMC6_GetServiceDescription succeeded with result=%p", *result);
    }
    return hresult;
}

HRESULT FSMC6_RegisterServicePartitionResolutionChangeHandler(
    IFabricServiceManagementClient6 * This,
    /* [in] */ FABRIC_URI name,
    /* [in] */ FABRIC_PARTITION_KEY_TYPE keyType,
    /* [in] */ const void *partitionKey,
    /* [in] */ IFabricServicePartitionResolutionChangeHandler *callback,
    /* [retval][out] */ LONGLONG *callbackHandle)
{
    LogVerbose("FSMC6_RegisterServicePartitionResolutionChangeHandler called with This=%p, name=%ls, keyType=%" PRI_MU_ENUM ", partitionKey=%p, callback=%p",
        This, name, MU_ENUM_VALUE(FABRIC_PARTITION_KEY_TYPE, keyType), partitionKey, callback);
    HRESULT result = This->RegisterServicePartitionResolutionChangeHandler(name, keyType, partitionKey, callback, callbackHandle);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_RegisterServicePartitionResolutionChangeHandler failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_RegisterServicePartitionResolutionChangeHandler succeeded with callbackHandle=%" PRId64 "", *callbackHandle);
    }
    return result;
}

HRESULT FSMC6_UnregisterServicePartitionResolutionChangeHandler(
    IFabricServiceManagementClient6 * This,
    /* [in] */ LONGLONG callbackHandle)
{
    LogVerbose("FSMC6_UnregisterServicePartitionResolutionChangeHandler called with This=%p, callbackHandle=%" PRId64 "",
        This, callbackHandle);
    HRESULT result = This->UnregisterServicePartitionResolutionChangeHandler(callbackHandle);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_UnregisterServicePartitionResolutionChangeHandler failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_UnregisterServicePartitionResolutionChangeHandler succeeded");
    }
    return result;
}

HRESULT FSMC6_ResolveServicePartition(IFabricServiceManagementClient6* client,
    FABRIC_URI name,
    FABRIC_PARTITION_KEY_TYPE partitionKeyType,
    const void *partitionKey,
    IFabricResolvedServicePartitionResult *previousResult,
    DWORD timeoutMilliseconds,
    IFabricResolvedServicePartitionResult **resolveServicePartitionResult)
{
    LogVerbose("FSMC6_ResolveServicePartition called with client=%p, name=%ls, partitionKeyType=%" PRI_MU_ENUM ", partitionKey=%p, previousResult=%p, timeoutMilliseconds=%" PRIu32 "",
        client, name, MU_ENUM_VALUE(FABRIC_PARTITION_KEY_TYPE, partitionKeyType), partitionKey, previousResult, timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricServiceManagementClient6::BeginResolveServicePartition, &IFabricServiceManagementClient6::EndResolveServicePartition,
        timeoutMilliseconds,
        resolveServicePartitionResult,
        name,
        partitionKeyType,
        partitionKey,
        previousResult);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FSMC6_ResolveServicePartition failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FSMC6_ResolveServicePartition succeeded with resolveServicePartitionResult=%p", *resolveServicePartitionResult);
    }
    return hresult;
}

HRESULT FSMC6_GetServiceManifest(
    IFabricServiceManagementClient6* client,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ LPCWSTR serviceManifestName,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    LogVerbose("FSMC6_GetServiceManifest called with client=%p, applicationTypeName=%ls, applicationTypeVersion=%ls, serviceManifestName=%ls, timeoutMilliseconds=%" PRIu32 "",
        client, applicationTypeName, applicationTypeVersion, serviceManifestName, timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(client, &IFabricServiceManagementClient6::BeginGetServiceManifest, &IFabricServiceManagementClient6::EndGetServiceManifest,
        timeoutMilliseconds,
        result,
        applicationTypeName,
        applicationTypeVersion,
        serviceManifestName);
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FSMC6_GetServiceManifest failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FSMC6_GetServiceManifest succeeded with result=%p", *result);
    }
    return hresult;
}

HRESULT FSMC6_UpdateService(
    IFabricServiceManagementClient6* client,
    /* [in] */ FABRIC_URI name,
    /* [in] */ const FABRIC_SERVICE_UPDATE_DESCRIPTION *serviceUpdateDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_UpdateService called with client=%p, name=%ls, serviceUpdateDescription=%" PRI_FABRIC_SERVICE_UPDATE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        client, name, FABRIC_SERVICE_UPDATE_DESCRIPTION_VALUES_OR_NULL(serviceUpdateDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginUpdateService, &IFabricServiceManagementClient6::EndUpdateService,
        timeoutMilliseconds,
        name,
        serviceUpdateDescription);
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_UpdateService failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_UpdateService succeeded");
    }
    return result;
}

HRESULT FSMC6_RemoveReplica(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_REMOVE_REPLICA_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_RemoveReplica called with This=%p, description=%" PRI_FABRIC_REMOVE_REPLICA_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_REMOVE_REPLICA_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginRemoveReplica, &IFabricServiceManagementClient6::EndRemoveReplica,
        timeoutMilliseconds,
        description
        );
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_RemoveReplica failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_RemoveReplica succeeded");
    }
    return result;
}

HRESULT FSMC6_RestartReplica(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_RESTART_REPLICA_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_RestartReplica called with This=%p, description=%" PRI_FABRIC_RESTART_REPLICA_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_RESTART_REPLICA_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginRestartReplica, &IFabricServiceManagementClient6::EndRestartReplica,
        timeoutMilliseconds,
        description
    );
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_RestartReplica failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_RestartReplica succeeded");
    }
    return result;
}

HRESULT FSMC6_RegisterServiceNotificationFilter(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ LONGLONG *filterId)
{
    LogVerbose("FSMC6_RegisterServiceNotificationFilter called with This=%p, description=%" PRI_FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION_VALUES_OR_NULL(description), timeoutMilliseconds);
    HRESULT hresult = ServiceFabric_DoX(This, &IFabricServiceManagementClient6::BeginRegisterServiceNotificationFilter, &IFabricServiceManagementClient6::EndRegisterServiceNotificationFilter,
        timeoutMilliseconds,
        filterId,
        description
        );
    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FSMC6_RegisterServiceNotificationFilter failed");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FSMC6_RegisterServiceNotificationFilter succeeded with filterId=%" PRId64 "", *filterId);
    }
    return hresult;
}

HRESULT FSMC6_UnregisterServiceNotificationFilter(
    IFabricServiceManagementClient6 * This,
    /* [in] */ LONGLONG filterId,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_UnregisterServiceNotificationFilter called with This=%p, filterId=%" PRId64 ", timeoutMilliseconds=%" PRIu32 "",
        This, filterId, timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginUnregisterServiceNotificationFilter, &IFabricServiceManagementClient6::EndUnregisterServiceNotificationFilter,
        timeoutMilliseconds,
        filterId
    );
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_UnregisterServiceNotificationFilter failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_UnregisterServiceNotificationFilter succeeded");
    }
    return result;
}

HRESULT FSMC6_DeleteService2(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_DELETE_SERVICE_DESCRIPTION *deleteDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_DeleteService2 called with This=%p, deleteDescription=%" PRI_FABRIC_DELETE_SERVICE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_DELETE_SERVICE_DESCRIPTION_VALUES_OR_NULL(deleteDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginDeleteService2, &IFabricServiceManagementClient6::EndDeleteService2,
        timeoutMilliseconds,
        deleteDescription
    );
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_DeleteService2 failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_DeleteService2 succeeded");
    }
    return result;
}

HRESULT FSMC6_CreateServiceFromTemplate2(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION *serviceFromTemplateDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    LogVerbose("FSMC6_CreateServiceFromTemplate2 called with This=%p, serviceFromTemplateDescription=%" PRI_FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 "",
        This, FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION_VALUES_OR_NULL(serviceFromTemplateDescription), timeoutMilliseconds);
    HRESULT result = ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginCreateServiceFromTemplate2, &IFabricServiceManagementClient6::EndCreateServiceFromTemplate2,
        timeoutMilliseconds,
        serviceFromTemplateDescription
    );
    if (FAILED(result))
    {
        LogHRESULTError(result, "FSMC6_CreateServiceFromTemplate2 failed");
    }
    else
    {
        LogHRESULTVerbose(result, "FSMC6_CreateServiceFromTemplate2 succeeded");
    }
    return result;
}
