// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef IFABRICSERVICEMANAGEMENTCLIENT6SYNC_H
#define IFABRICSERVICEMANAGEMENTCLIENT6SYNC_H

#include "windows.h"

#include "fabricclient.h"

#include "sf_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

    HRESULT FSMC6_CreateService(IFabricServiceManagementClient6* client,
        const FABRIC_SERVICE_DESCRIPTION *description,
        DWORD timeoutMilliseconds);

    HRESULT FSMC6_CreateServiceFromTemplate(IFabricServiceManagementClient6* client,
        FABRIC_URI applicationName,
        FABRIC_URI serviceName,
        LPCWSTR serviceTypeName,
        ULONG InitializationDataSize,
        BYTE *InitializationData,
        DWORD timeoutMilliseconds);

    HRESULT FSMC6_DeleteService(IFabricServiceManagementClient6* client,
        FABRIC_URI name,
        DWORD timeoutMilliseconds);

    HRESULT FSMC6_GetServiceDescription(IFabricServiceManagementClient6* client,
        FABRIC_URI name,
        DWORD timeoutMilliseconds,
        IFabricServiceDescriptionResult **result);

    HRESULT FSMC6_RegisterServicePartitionResolutionChangeHandler(
        IFabricServiceManagementClient6 * This,
        /* [in] */ FABRIC_URI name,
        /* [in] */ FABRIC_PARTITION_KEY_TYPE keyType,
        /* [in] */ const void *partitionKey,
        /* [in] */ IFabricServicePartitionResolutionChangeHandler *callback,
        /* [retval][out] */ LONGLONG *callbackHandle);

    HRESULT FSMC6_UnregisterServicePartitionResolutionChangeHandler(
        IFabricServiceManagementClient6 * This,
        /* [in] */ LONGLONG callbackHandle);

    HRESULT FSMC6_ResolveServicePartition(IFabricServiceManagementClient6* client,
        FABRIC_URI name,
        FABRIC_PARTITION_KEY_TYPE partitionKeyType,
        const void *partitionKey,
        IFabricResolvedServicePartitionResult *previousResult,
        DWORD timeoutMilliseconds,
        IFabricResolvedServicePartitionResult **resolveServicePartitionResult);

    HRESULT FSMC6_GetServiceManifest(
        IFabricServiceManagementClient6* client,
        /* [in] */ LPCWSTR applicationTypeName,
        /* [in] */ LPCWSTR applicationTypeVersion,
        /* [in] */ LPCWSTR serviceManifestName,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricStringResult **result);

    HRESULT FSMC6_UpdateService(
        IFabricServiceManagementClient6* client,
        /* [in] */ FABRIC_URI name,
        /* [in] */ const FABRIC_SERVICE_UPDATE_DESCRIPTION *serviceUpdateDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FSMC6_RemoveReplica(
        IFabricServiceManagementClient6 * This,
        /* [in] */ const FABRIC_REMOVE_REPLICA_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FSMC6_RestartReplica(
        IFabricServiceManagementClient6 * This,
        /* [in] */ const FABRIC_RESTART_REPLICA_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FSMC6_RegisterServiceNotificationFilter(
        IFabricServiceManagementClient6 * This,
        /* [in] */ const FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION *description,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ LONGLONG *filterId);

    HRESULT FSMC6_UnregisterServiceNotificationFilter(
        IFabricServiceManagementClient6 * This,
        /* [in] */ LONGLONG filterId,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FSMC6_DeleteService2(
        IFabricServiceManagementClient6 * This,
        /* [in] */ const FABRIC_DELETE_SERVICE_DESCRIPTION *deleteDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    HRESULT FSMC6_CreateServiceFromTemplate2(
        IFabricServiceManagementClient6 * This,
        /* [in] */ const FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION *serviceFromTemplateDescription,
        /* [in] */ DWORD timeoutMilliseconds);

    DECLARE_CREATE_IFABRICINSTANCE(IFabricServiceManagementClient6);

#ifdef __cplusplus
}
#endif

#endif /*IFABRICSERVICEMANAGEMENTCLIENT6SYNC_H*/
