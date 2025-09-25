// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef IFABRICQUERYCLIENT10SYNC_H
#define IFABRICQUERYCLIENT10SYNC_H

#include "windows.h"

#include "fabricclient.h"

#include "sf_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

    HRESULT FQC10_GetNodeList(IFabricQueryClient10* This,
        const FABRIC_NODE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetNodeListResult** fabricGetNodeListResult);

    HRESULT FQC10_GetApplicationTypeList(IFabricQueryClient10* This,
        const FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetApplicationTypeListResult **fabricGetApplicationTypeListResult);

    HRESULT FQC10_GetServiceTypeList(IFabricQueryClient10* This,
        const FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetServiceTypeListResult **fabricGetServiceTypeListResult);

    HRESULT FQC10_GetApplicationList(IFabricQueryClient10 * This,
        const FABRIC_APPLICATION_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetApplicationListResult **fabricGetApplicationListResult);

    HRESULT FQC10_GetServiceList(IFabricQueryClient10 * This,
        const FABRIC_SERVICE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetServiceListResult** fabricGetServiceListResult);

    HRESULT FQC10_GetPartitionList(IFabricQueryClient10 * This,
        const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetPartitionListResult** fabricGetPartitionListResult);

    HRESULT FQC10_GetReplicaList(IFabricQueryClient10 * This,
        const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetReplicaListResult** fabricGetReplicaListResult);

    HRESULT FQC10_GetDeployedApplicationList(IFabricQueryClient10 * This,
        const FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetDeployedApplicationListResult** fabricGetDeployedApplicationListResult);

    HRESULT FQC10_GetDeployedServicePackageList(IFabricQueryClient10 * This,
        const FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetDeployedServicePackageListResult** fabricGetDeployedServicePackageListResult);

    HRESULT FQC10_GetDeployedServiceTypeList(IFabricQueryClient10 * This,
        const FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetDeployedServiceTypeListResult** fabricGetDeployedServiceTypeListResult);

    HRESULT FQC10_GetDeployedCodePackageList(IFabricQueryClient10 * This,
        const FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetDeployedCodePackageListResult** fabricGetDeployedCodePackageListResult);

    HRESULT FQC10_GetDeployedReplicaList(IFabricQueryClient10 * This,
        const FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetDeployedReplicaListResult** fabricGetDeployedReplicaListResult);

    HRESULT FQC10_GetDeployedReplicaDetail(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetDeployedServiceReplicaDetailResult **result);

    HRESULT FQC10_GetClusterLoadInformation(
        IFabricQueryClient10 * This,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetClusterLoadInformationResult **result);

    HRESULT FQC10_GetPartitionLoadInformation(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetPartitionLoadInformationResult **result);

    HRESULT FQC10_GetProvisionedFabricCodeVersionList(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetProvisionedCodeVersionListResult **result);

    HRESULT FQC10_GetProvisionedFabricConfigVersionList(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetProvisionedConfigVersionListResult **result);

    HRESULT FQC10_GetNodeLoadInformation(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetNodeLoadInformationResult **result);

    HRESULT FQC10_GetReplicaLoadInformation(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetReplicaLoadInformationResult **result);

    HRESULT FQC10_GetServiceGroupMemberList(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetServiceGroupMemberListResult **result);

    HRESULT FQC10_GetServiceGroupMemberTypeList(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetServiceGroupMemberTypeListResult **result);

    HRESULT FQC10_GetUnplacedReplicaInformation(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetUnplacedReplicaInformationResult **result);

    HRESULT FQC10_GetNodeList2(IFabricQueryClient10* This,
        const FABRIC_NODE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetNodeListResult2** result);

    HRESULT FQC10_GetApplicationList2(IFabricQueryClient10 * This,
        const FABRIC_APPLICATION_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetApplicationListResult2 **result);

    HRESULT FQC10_GetServiceList2(IFabricQueryClient10 * This,
        const FABRIC_SERVICE_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetServiceListResult2** result);

    HRESULT FQC10_GetPartitionList2(IFabricQueryClient10 * This,
        const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetPartitionListResult2** result);

    HRESULT FQC10_GetReplicaList2(IFabricQueryClient10 * This,
        const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
        DWORD timeoutMilliseconds,
        IFabricGetReplicaListResult2** result);

    HRESULT FQC10_GetApplicationLoadInformation(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetApplicationLoadInformationResult **result);

    HRESULT FQC10_GetServiceName(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_SERVICE_NAME_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetServiceNameResult **result);

    HRESULT FQC10_GetApplicationName(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetApplicationNameResult **result);

    HRESULT FQC10_GetApplicationTypePagedList(
        IFabricQueryClient10 * This,
        /* [in] */ const PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetApplicationTypePagedListResult **result);

    HRESULT FQC10_GetDeployedApplicationPagedList(
        IFabricQueryClient10 * This,
        /* [in] */ const FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION *queryDescription,
        /* [in] */ DWORD timeoutMilliseconds,
        /* [retval][out] */ IFabricGetDeployedApplicationPagedListResult **result);

    DECLARE_CREATE_IFABRICINSTANCE(IFabricQueryClient10);

#ifdef __cplusplus
}
#endif


#endif /*IFABRICQUERYCLIENT10SYNC_H*/
