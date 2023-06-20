// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"

#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"
#include "mycallback.h"
#include "servicefabricdox.h"

#include "ifabricservicemanagementclient6sync.h"

HRESULT FSMC6_CreateService(IFabricServiceManagementClient6* client,
    const FABRIC_SERVICE_DESCRIPTION *description,
    DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginCreateService, &IFabricServiceManagementClient6::EndCreateService, timeoutMilliseconds, description);
}

HRESULT FSMC6_CreateServiceFromTemplate(IFabricServiceManagementClient6* client,
    FABRIC_URI applicationName,
    FABRIC_URI serviceName,
    LPCWSTR serviceTypeName,
    ULONG InitializationDataSize,
    BYTE *InitializationData,
    DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginCreateServiceFromTemplate, &IFabricServiceManagementClient6::EndCreateServiceFromTemplate,
        timeoutMilliseconds,
        applicationName,
        serviceName,
        serviceTypeName,
        InitializationDataSize,
        InitializationData);
}

HRESULT FSMC6_DeleteService(IFabricServiceManagementClient6* client,
    FABRIC_URI name,
    DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginDeleteService, &IFabricServiceManagementClient6::EndDeleteService,
        timeoutMilliseconds,
        name);
}

HRESULT FSMC6_GetServiceDescription(IFabricServiceManagementClient6* client,
    FABRIC_URI name,
    DWORD timeoutMilliseconds,
    IFabricServiceDescriptionResult **result)
{
    return ServiceFabric_DoX(client, &IFabricServiceManagementClient6::BeginGetServiceDescription, &IFabricServiceManagementClient6::EndGetServiceDescription,
        timeoutMilliseconds,
        result,
        name);
}

HRESULT FSMC6_RegisterServicePartitionResolutionChangeHandler(
    IFabricServiceManagementClient6 * This,
    /* [in] */ FABRIC_URI name,
    /* [in] */ FABRIC_PARTITION_KEY_TYPE keyType,
    /* [in] */ const void *partitionKey,
    /* [in] */ IFabricServicePartitionResolutionChangeHandler *callback,
    /* [retval][out] */ LONGLONG *callbackHandle)
{
    return This->RegisterServicePartitionResolutionChangeHandler(name, keyType, partitionKey, callback, callbackHandle);
}

HRESULT FSMC6_UnregisterServicePartitionResolutionChangeHandler(
    IFabricServiceManagementClient6 * This,
    /* [in] */ LONGLONG callbackHandle)
{
    return This->UnregisterServicePartitionResolutionChangeHandler(callbackHandle);
}

HRESULT FSMC6_ResolveServicePartition(IFabricServiceManagementClient6* client,
    FABRIC_URI name,
    FABRIC_PARTITION_KEY_TYPE partitionKeyType,
    const void *partitionKey,
    IFabricResolvedServicePartitionResult *previousResult,
    DWORD timeoutMilliseconds,
    IFabricResolvedServicePartitionResult **resolveServicePartitionResult)
{
    return ServiceFabric_DoX(client, &IFabricServiceManagementClient6::BeginResolveServicePartition, &IFabricServiceManagementClient6::EndResolveServicePartition,
        timeoutMilliseconds,
        resolveServicePartitionResult,
        name,
        partitionKeyType,
        partitionKey,
        previousResult);
}

HRESULT FSMC6_GetServiceManifest(
    IFabricServiceManagementClient6* client,
    /* [in] */ LPCWSTR applicationTypeName,
    /* [in] */ LPCWSTR applicationTypeVersion,
    /* [in] */ LPCWSTR serviceManifestName,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricStringResult **result)
{
    return ServiceFabric_DoX(client, &IFabricServiceManagementClient6::BeginGetServiceManifest, &IFabricServiceManagementClient6::EndGetServiceManifest,
        timeoutMilliseconds,
        result,
        applicationTypeName,
        applicationTypeVersion,
        serviceManifestName);
}

HRESULT FSMC6_UpdateService(
    IFabricServiceManagementClient6* client,
    /* [in] */ FABRIC_URI name,
    /* [in] */ const FABRIC_SERVICE_UPDATE_DESCRIPTION *serviceUpdateDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(client, &IFabricServiceManagementClient6::BeginUpdateService, &IFabricServiceManagementClient6::EndUpdateService,
        timeoutMilliseconds,
        name,
        serviceUpdateDescription);
}

HRESULT FSMC6_RemoveReplica(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_REMOVE_REPLICA_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginRemoveReplica, &IFabricServiceManagementClient6::EndRemoveReplica,
        timeoutMilliseconds,
        description
        );
}

HRESULT FSMC6_RestartReplica(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_RESTART_REPLICA_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginRestartReplica, &IFabricServiceManagementClient6::EndRestartReplica,
        timeoutMilliseconds,
        description
    );
}

HRESULT FSMC6_RegisterServiceNotificationFilter(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION *description,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ LONGLONG *filterId)
{
    return ServiceFabric_DoX(This, &IFabricServiceManagementClient6::BeginRegisterServiceNotificationFilter, &IFabricServiceManagementClient6::EndRegisterServiceNotificationFilter,
        timeoutMilliseconds,
        filterId,
        description
        );
}

HRESULT FSMC6_UnregisterServiceNotificationFilter(
    IFabricServiceManagementClient6 * This,
    /* [in] */ LONGLONG filterId,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginUnregisterServiceNotificationFilter, &IFabricServiceManagementClient6::EndUnregisterServiceNotificationFilter,
        timeoutMilliseconds,
        filterId
    );
}

HRESULT FSMC6_DeleteService2(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_DELETE_SERVICE_DESCRIPTION *deleteDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginDeleteService2, &IFabricServiceManagementClient6::EndDeleteService2,
        timeoutMilliseconds,
        deleteDescription
    );
}

HRESULT FSMC6_CreateServiceFromTemplate2(
    IFabricServiceManagementClient6 * This,
    /* [in] */ const FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION *serviceFromTemplateDescription,
    /* [in] */ DWORD timeoutMilliseconds)
{
    return ServiceFabric_DoX_NoResult(This, &IFabricServiceManagementClient6::BeginCreateServiceFromTemplate2, &IFabricServiceManagementClient6::EndCreateServiceFromTemplate2,
        timeoutMilliseconds,
        serviceFromTemplateDescription
    );
}
