// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef H_FABRIC_SERVICE_MANAGEMENT_CLIENT6_H
#define H_FABRIC_SERVICE_MANAGEMENT_CLIENT6_H

#include "windows.h"

#include "fabricclient.h"

#include "macro_utils/macro_utils.h"

#include "h_fabric_macro_generator.h"

H_FABRIC_DEFINE_TYPEDEF(IFabricServiceManagementClient6);

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    H_FABRIC_DECLARE_CREATE(IFabricServiceManagementClient6);
    H_FABRIC_DECLARE_DESTROY(IFabricServiceManagementClient6);

    /*below are the same API as ifabricqueryclient10sync exposes, which are the same as the methods of the IFabricServiceManagementClient6 interface*/
    /*note: the computed outputs (such as IFabricGetNodeListResult are "as they come" from IFabricServiceManagementClient6 interface*/

    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_CreateService,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_CreateServiceFromTemplate,
        IN_ARGS(
            ARG(FABRIC_URI, applicationName),
            ARG(FABRIC_URI, serviceName),
            ARG(LPCWSTR, serviceTypeName),
            ARG(ULONG, InitializationDataSize),
            ARG(BYTE*, InitializationData),
            ARG(DWORD, timeoutMilliseconds)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_DeleteService,
        IN_ARGS(
            ARG(FABRIC_URI, name),
            ARG(DWORD, timeoutMilliseconds)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_GetServiceDescription,
        IN_ARGS(
            ARG(FABRIC_URI, name),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricServiceDescriptionResult**, result)
        )
    )
    
    // The below APIs do not have a timeoutMillisecond.
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_RegisterServicePartitionResolutionChangeHandler,
        IN_ARGS(
            ARG(FABRIC_URI, name),
            ARG(FABRIC_PARTITION_KEY_TYPE, keyType),
            ARG(const void*, partitionKey),
            ARG(IFabricServicePartitionResolutionChangeHandler*, callback),
            ARG(LONGLONG*, callbackHandle)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_UnregisterServicePartitionResolutionChangeHandler,
        IN_ARGS(
            ARG(LONGLONG, callbackHandle)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_ResolveServicePartition,
        IN_ARGS(
            ARG(FABRIC_URI, name),
            ARG(FABRIC_PARTITION_KEY_TYPE, partitionKeyType),
            ARG(const void*, partitionKey),
            ARG(IFabricResolvedServicePartitionResult*, previousResult),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricResolvedServicePartitionResult**, resolveServicePartitionResult)
            )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_GetServiceManifest,
        IN_ARGS(
            ARG(LPCWSTR, applicationTypeName),
            ARG(LPCWSTR, applicationTypeVersion),
            ARG(LPCWSTR, serviceManifestName),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricStringResult**, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_UpdateService,
        IN_ARGS(
            ARG(FABRIC_URI, name),
            ARG(const FABRIC_SERVICE_UPDATE_DESCRIPTION*, serviceUpdateDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_RemoveReplica,
        IN_ARGS(
            ARG(const FABRIC_REMOVE_REPLICA_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_RestartReplica,
        IN_ARGS(
            ARG(const FABRIC_RESTART_REPLICA_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_RegisterServiceNotificationFilter,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_NOTIFICATION_FILTER_DESCRIPTION*, description),
            ARG(DWORD, timeoutMilliseconds),
            ARG(LONGLONG*, filterId)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_UnregisterServiceNotificationFilter,
        IN_ARGS(
            ARG(LONGLONG, filterId),
            ARG(DWORD, timeoutMilliseconds)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_DeleteService2,
        IN_ARGS(
            ARG(const FABRIC_DELETE_SERVICE_DESCRIPTION*, deleteDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricServiceManagementClient6, FSMC6_CreateServiceFromTemplate2,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_FROM_TEMPLATE_DESCRIPTION*, serviceFromTemplateDescription),
            ARG(DWORD, timeoutMilliseconds)
        )
    )

#ifdef __cplusplus
}
#endif

#endif /*H_FABRIC_SERVICE_MANAGEMENT_CLIENT6_H*/
