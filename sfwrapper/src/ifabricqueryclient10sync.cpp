// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "c_logging/logger.h"

#include "fabricclient.h"

#include "sf_c_util/hresult_to_string.h"
#include "mycallback.h"
#include "servicefabricdox.h"

#include "ifabricqueryclient10sync.h"


HRESULT FQC10_GetNodeList(IFabricQueryClient10* This,
    const FABRIC_NODE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetNodeListResult** fabricGetNodeListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetNodeList, &IFabricQueryClient10::EndGetNodeList,
        timeoutMilliseconds,
        fabricGetNodeListResult,
        queryDescription);
}

HRESULT FQC10_GetApplicationTypeList(
    IFabricQueryClient10* This,
    const FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetApplicationTypeListResult **fabricGetApplicationTypeListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationTypeList, &IFabricQueryClient10::EndGetApplicationTypeList,
        timeoutMilliseconds,
        fabricGetApplicationTypeListResult,
        queryDescription);
}

HRESULT FQC10_GetServiceTypeList(IFabricQueryClient10* This,
    const FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetServiceTypeListResult **fabricGetServiceTypeListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceTypeList, &IFabricQueryClient10::EndGetServiceTypeList,
        timeoutMilliseconds,
        fabricGetServiceTypeListResult,
        queryDescription);
}

HRESULT FQC10_GetApplicationList(IFabricQueryClient10 * This,
    const FABRIC_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetApplicationListResult **fabricGetApplicationListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationList, &IFabricQueryClient10::EndGetApplicationList,
        timeoutMilliseconds,
        fabricGetApplicationListResult,
        queryDescription);
}

HRESULT FQC10_GetServiceList(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetServiceListResult** fabricGetServiceListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceList, &IFabricQueryClient10::EndGetServiceList,
        timeoutMilliseconds,
        fabricGetServiceListResult,
        queryDescription);
}

HRESULT FQC10_GetPartitionList(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetPartitionListResult** fabricGetPartitionListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetPartitionList, &IFabricQueryClient10::EndGetPartitionList,
        timeoutMilliseconds,
        fabricGetPartitionListResult,
        queryDescription);
}

HRESULT FQC10_GetReplicaList(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetReplicaListResult** fabricGetReplicaListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetReplicaList, &IFabricQueryClient10::EndGetReplicaList,
        timeoutMilliseconds,
        fabricGetReplicaListResult,
        queryDescription);
}

HRESULT FQC10_GetDeployedApplicationList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedApplicationListResult** fabricGetDeployedApplicationListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedApplicationList, &IFabricQueryClient10::EndGetDeployedApplicationList,
        timeoutMilliseconds,
        fabricGetDeployedApplicationListResult,
        queryDescription);
}

HRESULT FQC10_GetDeployedServicePackageList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedServicePackageListResult** fabricGetDeployedServicePackageListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedServicePackageList, &IFabricQueryClient10::EndGetDeployedServicePackageList,
        timeoutMilliseconds,
        fabricGetDeployedServicePackageListResult,
        queryDescription);
}

HRESULT FQC10_GetDeployedServiceTypeList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedServiceTypeListResult** fabricGetDeployedServiceTypeListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedServiceTypeList, &IFabricQueryClient10::EndGetDeployedServiceTypeList,
        timeoutMilliseconds,
        fabricGetDeployedServiceTypeListResult,
        queryDescription);
}

HRESULT FQC10_GetDeployedCodePackageList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedCodePackageListResult** fabricGetDeployedCodePackageListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedCodePackageList, &IFabricQueryClient10::EndGetDeployedCodePackageList,
        timeoutMilliseconds,
        fabricGetDeployedCodePackageListResult,
        queryDescription);
}

HRESULT FQC10_GetDeployedReplicaList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedReplicaListResult** fabricGetDeployedReplicaListResult)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedReplicaList, &IFabricQueryClient10::EndGetDeployedReplicaList,
        timeoutMilliseconds,
        fabricGetDeployedReplicaListResult,
        queryDescription);
}

HRESULT FQC10_GetDeployedReplicaDetail(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetDeployedServiceReplicaDetailResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedReplicaDetail, &IFabricQueryClient10::EndGetDeployedReplicaDetail,
        timeoutMilliseconds,
        result,
        queryDescription
        );
}

HRESULT FQC10_GetClusterLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetClusterLoadInformationResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetClusterLoadInformation, &IFabricQueryClient10::EndGetClusterLoadInformation,
        timeoutMilliseconds,
        result
    );
}

HRESULT FQC10_GetPartitionLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetPartitionLoadInformationResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetPartitionLoadInformation, &IFabricQueryClient10::EndGetPartitionLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetProvisionedFabricCodeVersionList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetProvisionedCodeVersionListResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetProvisionedFabricCodeVersionList, &IFabricQueryClient10::EndGetProvisionedFabricCodeVersionList,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetProvisionedFabricConfigVersionList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetProvisionedConfigVersionListResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetProvisionedFabricConfigVersionList, &IFabricQueryClient10::EndGetProvisionedFabricConfigVersionList,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetNodeLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetNodeLoadInformationResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetNodeLoadInformation, &IFabricQueryClient10::EndGetNodeLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetReplicaLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetReplicaLoadInformationResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetReplicaLoadInformation, &IFabricQueryClient10::EndGetReplicaLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetServiceGroupMemberList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetServiceGroupMemberListResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceGroupMemberList, &IFabricQueryClient10::EndGetServiceGroupMemberList,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetServiceGroupMemberTypeList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetServiceGroupMemberTypeListResult **result) 
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceGroupMemberTypeList, &IFabricQueryClient10::EndGetServiceGroupMemberTypeList,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}


HRESULT FQC10_GetUnplacedReplicaInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetUnplacedReplicaInformationResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetUnplacedReplicaInformation, &IFabricQueryClient10::EndGetUnplacedReplicaInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetNodeList2(IFabricQueryClient10* This,
    const FABRIC_NODE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetNodeListResult2** result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetNodeList, &IFabricQueryClient10::EndGetNodeList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetApplicationList2(IFabricQueryClient10 * This,
    const FABRIC_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetApplicationListResult2 **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationList, &IFabricQueryClient10::EndGetApplicationList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetServiceList2(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetServiceListResult2** result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceList, &IFabricQueryClient10::EndGetServiceList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetPartitionList2(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetPartitionListResult2** result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetPartitionList, &IFabricQueryClient10::EndGetPartitionList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetReplicaList2(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetReplicaListResult2** result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetReplicaList, &IFabricQueryClient10::EndGetReplicaList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetApplicationLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetApplicationLoadInformationResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationLoadInformation, &IFabricQueryClient10::EndGetApplicationLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetServiceName(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_SERVICE_NAME_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetServiceNameResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceName, &IFabricQueryClient10::EndGetServiceName,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetApplicationName(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetApplicationNameResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationName, &IFabricQueryClient10::EndGetApplicationName,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetApplicationTypePagedList(
    IFabricQueryClient10 * This,
    /* [in] */ const PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetApplicationTypePagedListResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationTypePagedList, &IFabricQueryClient10::EndGetApplicationTypePagedList,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}

HRESULT FQC10_GetDeployedApplicationPagedList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetDeployedApplicationPagedListResult **result)
{
    return ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedApplicationPagedList, &IFabricQueryClient10::EndGetDeployedApplicationPagedList,
        timeoutMilliseconds,
        result,
        queryDescription
    );
}
