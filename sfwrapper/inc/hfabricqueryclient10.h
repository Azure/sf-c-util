// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef H_FABRIC_QUERY_CLIENT10_H
#define H_FABRIC_QUERY_CLIENT10_H

#include "windows.h"

#include "fabricclient.h"

#include "macro_utils/macro_utils.h"

#include "h_fabric_macro_generator.h"

H_FABRIC_DEFINE_TYPEDEF(IFabricQueryClient10);

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

    H_FABRIC_DECLARE_CREATE(IFabricQueryClient10);
    H_FABRIC_DECLARE_DESTROY(IFabricQueryClient10);

    /*below are the same API as ifabricqueryclient10sync exposes, which are the same as the methods of the IFabricQueryClient10 interface*/
    /*note: the computed outputs (such as IFabricGetNodeListResult are "as they come" from IFabricQueryClient10 interface*/

    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetNodeList,
        IN_ARGS(
            ARG(const FABRIC_NODE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetNodeListResult**, fabricGetNodeListResult)
        )
    )

    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetApplicationTypeList,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetApplicationTypeListResult**, fabricGetApplicationTypeListResult)
        )
    )

    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetServiceTypeList,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetServiceTypeListResult**, fabricGetServiceTypeListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetApplicationList,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetApplicationListResult **, fabricGetApplicationListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetServiceList,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetServiceListResult**, fabricGetServiceListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetPartitionList,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetPartitionListResult**, fabricGetPartitionListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetReplicaList,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetReplicaListResult**, fabricGetReplicaListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetDeployedApplicationList,
        IN_ARGS(
            ARG(const FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetDeployedApplicationListResult**, fabricGetDeployedApplicationListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetDeployedServicePackageList,
        IN_ARGS(
            ARG(const FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetDeployedServicePackageListResult**, fabricGetDeployedServicePackageListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetDeployedServiceTypeList,
        IN_ARGS(
            ARG(const FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetDeployedServiceTypeListResult**, fabricGetDeployedServiceTypeListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetDeployedCodePackageList,
        IN_ARGS(
            ARG(const FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetDeployedCodePackageListResult**, fabricGetDeployedCodePackageListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetDeployedReplicaList,
        IN_ARGS(
            ARG(const FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetDeployedReplicaListResult**, fabricGetDeployedReplicaListResult)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetDeployedReplicaDetail,
        IN_ARGS(
            ARG(const FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetDeployedServiceReplicaDetailResult**, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetClusterLoadInformation,
        IN_ARGS(
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetClusterLoadInformationResult **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetPartitionLoadInformation,
        IN_ARGS(
            ARG(const FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetPartitionLoadInformationResult **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetProvisionedFabricCodeVersionList,
        IN_ARGS(
            ARG(const FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetProvisionedCodeVersionListResult **,result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetProvisionedFabricConfigVersionList,
        IN_ARGS(
            ARG(const FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetProvisionedConfigVersionListResult **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetNodeLoadInformation,
        IN_ARGS(
            ARG(const FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetNodeLoadInformationResult **,result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetReplicaLoadInformation,
        IN_ARGS(
            ARG(const FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetReplicaLoadInformationResult **,result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetServiceGroupMemberList,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetServiceGroupMemberListResult **,result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetServiceGroupMemberTypeList,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetServiceGroupMemberTypeListResult **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetUnplacedReplicaInformation,
        IN_ARGS(
            ARG(const FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetUnplacedReplicaInformationResult**, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetNodeList2,
        IN_ARGS(
            ARG(const FABRIC_NODE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetNodeListResult2**, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetApplicationList2,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetApplicationListResult2 **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetServiceList2,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetServiceListResult2**, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetPartitionList2,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetPartitionListResult2**, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetReplicaList2,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetReplicaListResult2**, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetApplicationLoadInformation,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetApplicationLoadInformationResult **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetServiceName,
        IN_ARGS(
            ARG(const FABRIC_SERVICE_NAME_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetServiceNameResult **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetApplicationName,
        IN_ARGS(
            ARG(const FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetApplicationNameResult **, result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetApplicationTypePagedList,
        IN_ARGS(
            ARG(const PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetApplicationTypePagedListResult **,result)
        )
    )
    
    H_FABRIC_DECLARE_API(IFabricQueryClient10, FQC10_GetDeployedApplicationPagedList,
        IN_ARGS(
            ARG(const FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION*, queryDescription),
            ARG(DWORD, timeoutMilliseconds),
            ARG(IFabricGetDeployedApplicationPagedListResult **, result)
        )
    )

#ifdef __cplusplus
}
#endif


#endif /*H_FABRIC_QUERY_CLIENT10_H*/
