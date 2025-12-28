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

#include "ifabricqueryclient10sync.h"

/* Format specifiers for FABRIC_NODE_QUERY_DESCRIPTION */
#define PRI_FABRIC_NODE_QUERY_DESCRIPTION "s(FABRIC_NODE_QUERY_DESCRIPTION){ .NodeNameFilter=%ls, .Reserved=%p }"

#define FABRIC_NODE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION */
#define PRI_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION "s(FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION){ .ApplicationTypeNameFilter=%ls, .Reserved=%p }"

#define FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION */
#define PRI_FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION "s(FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION){ .ApplicationTypeName=%ls, .ApplicationTypeVersion=%ls, .ServiceTypeNameFilter=%ls, .Reserved=%p }"

#define FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeVersion, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceTypeNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_APPLICATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_APPLICATION_QUERY_DESCRIPTION "s(FABRIC_APPLICATION_QUERY_DESCRIPTION){ .ApplicationNameFilter=%ls, .Reserved=%p }"

#define FABRIC_APPLICATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_QUERY_DESCRIPTION */
#define PRI_FABRIC_SERVICE_QUERY_DESCRIPTION "s(FABRIC_SERVICE_QUERY_DESCRIPTION){ .ApplicationName=%ls, .ServiceNameFilter=%ls, .Reserved=%p }"

#define FABRIC_SERVICE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION */
#define PRI_FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION "s(FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION){ .ServiceName=%ls, .PartitionIdFilter=%" PRI_GUID ", .Reserved=%p }"

#define FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceName, \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionIdFilter), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION */
#define PRI_FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION "s(FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION){ .PartitionId=%" PRI_GUID ", .ReplicaIdOrInstanceIdFilter=%" PRId64 ", .Reserved=%p }"

#define FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    ((desc) == NULL) ? 0 : (int64_t)(desc)->ReplicaIdOrInstanceIdFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION "s(FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION){ .NodeName=%ls, .ApplicationNameFilter=%ls, .Reserved=%p }"

#define FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION */
#define PRI_FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION "s(FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION){ .NodeName=%ls, .ApplicationName=%ls, .ServiceManifestNameFilter=%ls, .Reserved=%p }"

#define FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceManifestNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION */
#define PRI_FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION "s(FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION){ .NodeName=%ls, .ApplicationName=%ls, .ServiceManifestNameFilter=%ls, .ServiceTypeNameFilter=%ls, .Reserved=%p }"

#define FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceManifestNameFilter, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceTypeNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION */
#define PRI_FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION "s(FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION){ .NodeName=%ls, .ApplicationName=%ls, .ServiceManifestNameFilter=%ls, .CodePackageNameFilter=%ls, .Reserved=%p }"

#define FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceManifestNameFilter, \
    ((desc) == NULL) ? L"NULL" : (desc)->CodePackageNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION */
#define PRI_FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION "s(FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION){ .NodeName=%ls, .ApplicationName=%ls, .ServiceManifestNameFilter=%ls, .PartitionIdFilter=%" PRI_GUID ", .Reserved=%p }"

#define FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceManifestNameFilter, \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionIdFilter), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION */
#define PRI_FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION "s(FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION){ .NodeName=%ls, .PartitionId=%" PRI_GUID ", .ReplicaId=%" PRId64 ", .Reserved=%p }"

#define FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    ((desc) == NULL) ? 0 : (int64_t)(desc)->ReplicaId, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION "s(FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION){ .PartitionId=%" PRI_GUID ", .Reserved=%p }"

#define FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION */
#define PRI_FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION "s(FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION){ .CodeVersionFilter=%ls, .Reserved=%p }"

#define FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->CodeVersionFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION */
#define PRI_FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION "s(FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION){ .ConfigVersionFilter=%ls, .Reserved=%p }"

#define FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ConfigVersionFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION "s(FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION){ .NodeName=%ls, .Reserved=%p }"

#define FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION "s(FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION){ .PartitionId=%" PRI_GUID ", .ReplicaOrInstanceId=%" PRId64 ", .Reserved=%p }"

#define FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    ((desc) == NULL) ? 0 : (int64_t)(desc)->ReplicaOrInstanceId, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION */
#define PRI_FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION "s(FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION){ .ApplicationName=%ls, .ServiceNameFilter=%ls, .Reserved=%p }"

#define FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION */
#define PRI_FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION "s(FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION){ .ApplicationTypeName=%ls, .ApplicationTypeVersion=%ls, .ServiceGroupTypeNameFilter=%ls, .Reserved=%p }"

#define FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeVersion, \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceGroupTypeNameFilter, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION "s(FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION){ .ServiceName=%ls, .PartitionId=%" PRI_GUID ", .OnlyQueryPrimaries=%" PRI_BOOL ", .Reserved=%p }"

#define FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceName, \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->OnlyQueryPrimaries), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION "s(FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION){ .ApplicationName=%ls, .Reserved=%p }"

#define FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationName, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_SERVICE_NAME_QUERY_DESCRIPTION */
#define PRI_FABRIC_SERVICE_NAME_QUERY_DESCRIPTION "s(FABRIC_SERVICE_NAME_QUERY_DESCRIPTION){ .PartitionId=%" PRI_GUID ", .Reserved=%p }"

#define FABRIC_SERVICE_NAME_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    GUID_VALUES(((desc) == NULL) ? GUID{} : (desc)->PartitionId), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION */
#define PRI_FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION "s(FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION){ .ServiceName=%ls, .Reserved=%p }"

#define FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ServiceName, \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION */
#define PRI_PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION "s(PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION){ .ApplicationTypeNameFilter=%ls, .MaxResults=%" PRId32 ", .ContinuationToken=%ls, .ExcludeApplicationParameters=%" PRI_BOOL ", .Reserved=%p }"

#define PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationTypeNameFilter, \
    ((desc) == NULL) ? 0 : (int32_t)(desc)->MaxResults, \
    ((desc) == NULL) ? L"NULL" : (desc)->ContinuationToken, \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->ExcludeApplicationParameters), \
    ((desc) == NULL) ? NULL : (desc)->Reserved

/* Format specifiers for FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION */
#define PRI_FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION "s(FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION){ .NodeName=%ls, .ApplicationNameFilter=%ls, .IncludeHealthState=%" PRI_BOOL ", .PagingDescription=%p, .Reserved=%p }"

#define FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION_VALUES_OR_NULL(desc) \
    "", \
    ((desc) == NULL) ? L"NULL" : (desc)->NodeName, \
    ((desc) == NULL) ? L"NULL" : (desc)->ApplicationNameFilter, \
    MU_BOOL_VALUE(((desc) == NULL) ? 0 : (desc)->IncludeHealthState), \
    ((desc) == NULL) ? NULL : (desc)->PagingDescription, \
    ((desc) == NULL) ? NULL : (desc)->Reserved


HRESULT FQC10_GetNodeList(IFabricQueryClient10* This,
    const FABRIC_NODE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetNodeListResult** fabricGetNodeListResult)
{
    LogVerbose("FQC10_GetNodeList called with This=%p, queryDescription=%" PRI_FABRIC_NODE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetNodeListResult=%p",
        This, FABRIC_NODE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetNodeListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetNodeList, &IFabricQueryClient10::EndGetNodeList,
        timeoutMilliseconds,
        fabricGetNodeListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetNodeList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetNodeList returning");
    }
    return hresult;
}

HRESULT FQC10_GetApplicationTypeList(
    IFabricQueryClient10* This,
    const FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetApplicationTypeListResult **fabricGetApplicationTypeListResult)
{
    LogVerbose("FQC10_GetApplicationTypeList called with This=%p, queryDescription=%" PRI_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetApplicationTypeListResult=%p",
        This, FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetApplicationTypeListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationTypeList, &IFabricQueryClient10::EndGetApplicationTypeList,
        timeoutMilliseconds,
        fabricGetApplicationTypeListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetApplicationTypeList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetApplicationTypeList returning");
    }
    return hresult;
}

HRESULT FQC10_GetServiceTypeList(IFabricQueryClient10* This,
    const FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetServiceTypeListResult **fabricGetServiceTypeListResult)
{
    LogVerbose("FQC10_GetServiceTypeList called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetServiceTypeListResult=%p",
        This, FABRIC_SERVICE_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetServiceTypeListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceTypeList, &IFabricQueryClient10::EndGetServiceTypeList,
        timeoutMilliseconds,
        fabricGetServiceTypeListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetServiceTypeList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetServiceTypeList returning");
    }
    return hresult;
}

HRESULT FQC10_GetApplicationList(IFabricQueryClient10 * This,
    const FABRIC_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetApplicationListResult **fabricGetApplicationListResult)
{
    LogVerbose("FQC10_GetApplicationList called with This=%p, queryDescription=%" PRI_FABRIC_APPLICATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetApplicationListResult=%p",
        This, FABRIC_APPLICATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetApplicationListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationList, &IFabricQueryClient10::EndGetApplicationList,
        timeoutMilliseconds,
        fabricGetApplicationListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetApplicationList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetApplicationList returning");
    }
    return hresult;
}

HRESULT FQC10_GetServiceList(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetServiceListResult** fabricGetServiceListResult)
{
    LogVerbose("FQC10_GetServiceList called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetServiceListResult=%p",
        This, FABRIC_SERVICE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetServiceListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceList, &IFabricQueryClient10::EndGetServiceList,
        timeoutMilliseconds,
        fabricGetServiceListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetServiceList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetServiceList returning");
    }
    return hresult;
}

HRESULT FQC10_GetPartitionList(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetPartitionListResult** fabricGetPartitionListResult)
{
    LogVerbose("FQC10_GetPartitionList called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetPartitionListResult=%p",
        This, FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetPartitionListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetPartitionList, &IFabricQueryClient10::EndGetPartitionList,
        timeoutMilliseconds,
        fabricGetPartitionListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetPartitionList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetPartitionList returning");
    }
    return hresult;
}

HRESULT FQC10_GetReplicaList(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetReplicaListResult** fabricGetReplicaListResult)
{
    LogVerbose("FQC10_GetReplicaList called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetReplicaListResult=%p",
        This, FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetReplicaListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetReplicaList, &IFabricQueryClient10::EndGetReplicaList,
        timeoutMilliseconds,
        fabricGetReplicaListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetReplicaList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetReplicaList returning");
    }
    return hresult;
}

HRESULT FQC10_GetDeployedApplicationList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedApplicationListResult** fabricGetDeployedApplicationListResult)
{
    LogVerbose("FQC10_GetDeployedApplicationList called with This=%p, queryDescription=%" PRI_FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetDeployedApplicationListResult=%p",
        This, FABRIC_DEPLOYED_APPLICATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetDeployedApplicationListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedApplicationList, &IFabricQueryClient10::EndGetDeployedApplicationList,
        timeoutMilliseconds,
        fabricGetDeployedApplicationListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetDeployedApplicationList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetDeployedApplicationList returning");
    }
    return hresult;
}

HRESULT FQC10_GetDeployedServicePackageList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedServicePackageListResult** fabricGetDeployedServicePackageListResult)
{
    LogVerbose("FQC10_GetDeployedServicePackageList called with This=%p, queryDescription=%" PRI_FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetDeployedServicePackageListResult=%p",
        This, FABRIC_DEPLOYED_SERVICE_PACKAGE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetDeployedServicePackageListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedServicePackageList, &IFabricQueryClient10::EndGetDeployedServicePackageList,
        timeoutMilliseconds,
        fabricGetDeployedServicePackageListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetDeployedServicePackageList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetDeployedServicePackageList returning");
    }
    return hresult;
}

HRESULT FQC10_GetDeployedServiceTypeList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedServiceTypeListResult** fabricGetDeployedServiceTypeListResult)
{
    LogVerbose("FQC10_GetDeployedServiceTypeList called with This=%p, queryDescription=%" PRI_FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetDeployedServiceTypeListResult=%p",
        This, FABRIC_DEPLOYED_SERVICE_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetDeployedServiceTypeListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedServiceTypeList, &IFabricQueryClient10::EndGetDeployedServiceTypeList,
        timeoutMilliseconds,
        fabricGetDeployedServiceTypeListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetDeployedServiceTypeList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetDeployedServiceTypeList returning");
    }
    return hresult;
}

HRESULT FQC10_GetDeployedCodePackageList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedCodePackageListResult** fabricGetDeployedCodePackageListResult)
{
    LogVerbose("FQC10_GetDeployedCodePackageList called with This=%p, queryDescription=%" PRI_FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetDeployedCodePackageListResult=%p",
        This, FABRIC_DEPLOYED_CODE_PACKAGE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetDeployedCodePackageListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedCodePackageList, &IFabricQueryClient10::EndGetDeployedCodePackageList,
        timeoutMilliseconds,
        fabricGetDeployedCodePackageListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetDeployedCodePackageList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetDeployedCodePackageList returning");
    }
    return hresult;
}

HRESULT FQC10_GetDeployedReplicaList(IFabricQueryClient10 * This,
    const FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetDeployedReplicaListResult** fabricGetDeployedReplicaListResult)
{
    LogVerbose("FQC10_GetDeployedReplicaList called with This=%p, queryDescription=%" PRI_FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", fabricGetDeployedReplicaListResult=%p",
        This, FABRIC_DEPLOYED_SERVICE_REPLICA_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, fabricGetDeployedReplicaListResult);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedReplicaList, &IFabricQueryClient10::EndGetDeployedReplicaList,
        timeoutMilliseconds,
        fabricGetDeployedReplicaListResult,
        queryDescription);

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetDeployedReplicaList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetDeployedReplicaList returning");
    }
    return hresult;
}

HRESULT FQC10_GetDeployedReplicaDetail(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetDeployedServiceReplicaDetailResult **result)
{
    LogVerbose("FQC10_GetDeployedReplicaDetail called with This=%p, queryDescription=%" PRI_FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_DEPLOYED_SERVICE_REPLICA_DETAIL_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedReplicaDetail, &IFabricQueryClient10::EndGetDeployedReplicaDetail,
        timeoutMilliseconds,
        result,
        queryDescription
        );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetDeployedReplicaDetail returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetDeployedReplicaDetail returning");
    }
    return hresult;
}

HRESULT FQC10_GetClusterLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetClusterLoadInformationResult **result)
{
    LogVerbose("FQC10_GetClusterLoadInformation called with This=%p, timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetClusterLoadInformation, &IFabricQueryClient10::EndGetClusterLoadInformation,
        timeoutMilliseconds,
        result
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetClusterLoadInformation returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetClusterLoadInformation returning");
    }
    return hresult;
}

HRESULT FQC10_GetPartitionLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetPartitionLoadInformationResult **result)
{
    LogVerbose("FQC10_GetPartitionLoadInformation called with This=%p, queryDescription=%" PRI_FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_PARTITION_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetPartitionLoadInformation, &IFabricQueryClient10::EndGetPartitionLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetPartitionLoadInformation returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetPartitionLoadInformation returning");
    }
    return hresult;
}

HRESULT FQC10_GetProvisionedFabricCodeVersionList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetProvisionedCodeVersionListResult **result)
{
    LogVerbose("FQC10_GetProvisionedFabricCodeVersionList called with This=%p, queryDescription=%" PRI_FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_PROVISIONED_CODE_VERSION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetProvisionedFabricCodeVersionList, &IFabricQueryClient10::EndGetProvisionedFabricCodeVersionList,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetProvisionedFabricCodeVersionList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetProvisionedFabricCodeVersionList returning");
    }
    return hresult;
}

HRESULT FQC10_GetProvisionedFabricConfigVersionList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetProvisionedConfigVersionListResult **result)
{
    LogVerbose("FQC10_GetProvisionedFabricConfigVersionList called with This=%p, queryDescription=%" PRI_FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_PROVISIONED_CONFIG_VERSION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetProvisionedFabricConfigVersionList, &IFabricQueryClient10::EndGetProvisionedFabricConfigVersionList,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetProvisionedFabricConfigVersionList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetProvisionedFabricConfigVersionList returning");
    }
    return hresult;
}

HRESULT FQC10_GetNodeLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetNodeLoadInformationResult **result)
{
    LogVerbose("FQC10_GetNodeLoadInformation called with This=%p, queryDescription=%" PRI_FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_NODE_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetNodeLoadInformation, &IFabricQueryClient10::EndGetNodeLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetNodeLoadInformation returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetNodeLoadInformation returning");
    }
    return hresult;
}

HRESULT FQC10_GetReplicaLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetReplicaLoadInformationResult **result)
{
    LogVerbose("FQC10_GetReplicaLoadInformation called with This=%p, queryDescription=%" PRI_FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_REPLICA_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetReplicaLoadInformation, &IFabricQueryClient10::EndGetReplicaLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetReplicaLoadInformation returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetReplicaLoadInformation returning");
    }
    return hresult;
}

HRESULT FQC10_GetServiceGroupMemberList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetServiceGroupMemberListResult **result)
{
    LogVerbose("FQC10_GetServiceGroupMemberList called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_SERVICE_GROUP_MEMBER_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceGroupMemberList, &IFabricQueryClient10::EndGetServiceGroupMemberList,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetServiceGroupMemberList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetServiceGroupMemberList returning");
    }
    return hresult;
}

HRESULT FQC10_GetServiceGroupMemberTypeList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetServiceGroupMemberTypeListResult **result) 
{
    LogVerbose("FQC10_GetServiceGroupMemberTypeList called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_SERVICE_GROUP_MEMBER_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceGroupMemberTypeList, &IFabricQueryClient10::EndGetServiceGroupMemberTypeList,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetServiceGroupMemberTypeList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetServiceGroupMemberTypeList returning");
    }
    return hresult;
}


HRESULT FQC10_GetUnplacedReplicaInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetUnplacedReplicaInformationResult **result)
{
    LogVerbose("FQC10_GetUnplacedReplicaInformation called with This=%p, queryDescription=%" PRI_FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_UNPLACED_REPLICA_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetUnplacedReplicaInformation, &IFabricQueryClient10::EndGetUnplacedReplicaInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetUnplacedReplicaInformation returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetUnplacedReplicaInformation returning");
    }
    return hresult;
}

HRESULT FQC10_GetNodeList2(IFabricQueryClient10* This,
    const FABRIC_NODE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetNodeListResult2** result)
{
    LogVerbose("FQC10_GetNodeList2 called with This=%p, queryDescription=%" PRI_FABRIC_NODE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_NODE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetNodeList, &IFabricQueryClient10::EndGetNodeList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetNodeList2 returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetNodeList2 returning");
    }
    return hresult;
}

HRESULT FQC10_GetApplicationList2(IFabricQueryClient10 * This,
    const FABRIC_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetApplicationListResult2 **result)
{
    LogVerbose("FQC10_GetApplicationList2 called with This=%p, queryDescription=%" PRI_FABRIC_APPLICATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_APPLICATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationList, &IFabricQueryClient10::EndGetApplicationList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetApplicationList2 returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetApplicationList2 returning");
    }
    return hresult;
}

HRESULT FQC10_GetServiceList2(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetServiceListResult2** result)
{
    LogVerbose("FQC10_GetServiceList2 called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_SERVICE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceList, &IFabricQueryClient10::EndGetServiceList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetServiceList2 returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetServiceList2 returning");
    }
    return hresult;
}

HRESULT FQC10_GetPartitionList2(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetPartitionListResult2** result)
{
    LogVerbose("FQC10_GetPartitionList2 called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_SERVICE_PARTITION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetPartitionList, &IFabricQueryClient10::EndGetPartitionList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetPartitionList2 returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetPartitionList2 returning");
    }
    return hresult;
}

HRESULT FQC10_GetReplicaList2(IFabricQueryClient10 * This,
    const FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION *queryDescription,
    DWORD timeoutMilliseconds,
    IFabricGetReplicaListResult2** result)
{
    LogVerbose("FQC10_GetReplicaList2 called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_SERVICE_REPLICA_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetReplicaList, &IFabricQueryClient10::EndGetReplicaList2,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetReplicaList2 returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetReplicaList2 returning");
    }
    return hresult;
}

HRESULT FQC10_GetApplicationLoadInformation(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetApplicationLoadInformationResult **result)
{
    LogVerbose("FQC10_GetApplicationLoadInformation called with This=%p, queryDescription=%" PRI_FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_APPLICATION_LOAD_INFORMATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationLoadInformation, &IFabricQueryClient10::EndGetApplicationLoadInformation,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetApplicationLoadInformation returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetApplicationLoadInformation returning");
    }
    return hresult;
}

HRESULT FQC10_GetServiceName(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_SERVICE_NAME_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetServiceNameResult **result)
{
    LogVerbose("FQC10_GetServiceName called with This=%p, queryDescription=%" PRI_FABRIC_SERVICE_NAME_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_SERVICE_NAME_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetServiceName, &IFabricQueryClient10::EndGetServiceName,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetServiceName returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetServiceName returning");
    }
    return hresult;
}

HRESULT FQC10_GetApplicationName(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetApplicationNameResult **result)
{
    LogVerbose("FQC10_GetApplicationName called with This=%p, queryDescription=%" PRI_FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_APPLICATION_NAME_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationName, &IFabricQueryClient10::EndGetApplicationName,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetApplicationName returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetApplicationName returning");
    }
    return hresult;
}

HRESULT FQC10_GetApplicationTypePagedList(
    IFabricQueryClient10 * This,
    /* [in] */ const PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetApplicationTypePagedListResult **result)
{
    LogVerbose("FQC10_GetApplicationTypePagedList called with This=%p, queryDescription=%" PRI_PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, PAGED_FABRIC_APPLICATION_TYPE_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetApplicationTypePagedList, &IFabricQueryClient10::EndGetApplicationTypePagedList,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetApplicationTypePagedList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetApplicationTypePagedList returning");
    }
    return hresult;
}

HRESULT FQC10_GetDeployedApplicationPagedList(
    IFabricQueryClient10 * This,
    /* [in] */ const FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION *queryDescription,
    /* [in] */ DWORD timeoutMilliseconds,
    /* [retval][out] */ IFabricGetDeployedApplicationPagedListResult **result)
{
    LogVerbose("FQC10_GetDeployedApplicationPagedList called with This=%p, queryDescription=%" PRI_FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION ", timeoutMilliseconds=%" PRIu32 ", result=%p",
        This, FABRIC_PAGED_DEPLOYED_APPLICATION_QUERY_DESCRIPTION_VALUES_OR_NULL(queryDescription), timeoutMilliseconds, result);

    HRESULT hresult = ServiceFabric_DoX(This, &IFabricQueryClient10::BeginGetDeployedApplicationPagedList, &IFabricQueryClient10::EndGetDeployedApplicationPagedList,
        timeoutMilliseconds,
        result,
        queryDescription
    );

    if (FAILED(hresult))
    {
        LogHRESULTError(hresult, "FQC10_GetDeployedApplicationPagedList returning");
    }
    else
    {
        LogHRESULTVerbose(hresult, "FQC10_GetDeployedApplicationPagedList returning");
    }
    return hresult;
}
