// Copyright (C) Microsoft Corporation. All rights reserved.

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>


#include "windows.h"

#include "fabricruntime.h"
#include "fabrictypes.h"

#include "macro_utils/macro_utils.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "c_util/rc_string.h"
#include "c_pal/thandle.h"

#include "sf_c_util/hresult_to_string.h"

#include "sf_c_util/sf_config_to_strings.h"

typedef struct SF_CONFIG_TO_STRINGS_TAG
{
    char* node_name;
    char* partition_id;
    char* replica_id;
    THANDLE(RC_STRING) code_package_path;
} SF_CONFIG_TO_STRINGS;

SF_CONFIG_TO_STRINGS_HANDLE sf_config_to_strings_create(IFabricCodePackageActivationContext* activation_context, const FABRIC_PARTITION_ID* fabric_partition_id, FABRIC_REPLICA_ID fabric_replica_id)
{
    SF_CONFIG_TO_STRINGS_HANDLE result;
    HRESULT hr;

    if (
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_001: [ If activation_context is NULL then sf_config_to_strings_create shall fail and return NULL. ]*/
        activation_context == NULL ||
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_063: [ If fabric_partition_id is NULL then sf_config_to_strings_create shall fail and return NULL. ]*/
        fabric_partition_id == NULL
        )
    {
        LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p, const FABRIC_PARTITION_ID* fabric_partition_id = %p, FABRIC_REPLICA_ID fabric_replica_id = %lld",
            activation_context, fabric_partition_id, fabric_replica_id);
        result = NULL;
    }
    else
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_002: [ sf_config_to_strings_create shall allocate memory for the SF_CONFIG_TO_STRINGS_HANDLE. ]*/
        SF_CONFIG_TO_STRINGS_HANDLE temp = malloc(sizeof(SF_CONFIG_TO_STRINGS));

        if (temp == NULL)
        {
            /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
            LogError("malloc failed for SF_CONFIG_TO_STRINGS");
            result = NULL;
        }
        else
        {
            /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_064: [ sf_config_to_strings_create shall call UuidToStringA to convert the fabric_partition_id to a string. ]*/
            UUID partition_id_uuid;
            RPC_CSTR partition_id_rpc_string;
            partition_id_uuid.Data1 = fabric_partition_id->Data1;
            partition_id_uuid.Data2 = fabric_partition_id->Data2;
            partition_id_uuid.Data3 = fabric_partition_id->Data3;
            (void)memcpy(partition_id_uuid.Data4, fabric_partition_id->Data4, 8);
            RPC_STATUS rpc_status = UuidToStringA(&partition_id_uuid, &partition_id_rpc_string);
            if (rpc_status != RPC_S_OK)
            {
                /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                LogError("UuidToStringA failed for partition ID, %x", rpc_status);
                result = NULL;
            }
            else
            {
                /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_082: [ sf_config_to_strings_create shall copy the result of UuidToStringA to a string. ]*/
                temp->partition_id = sprintf_char("%s", partition_id_rpc_string);

                if (temp->partition_id == NULL)
                {
                    /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                    LogError("Failed to copy partition ID string");
                    result = NULL;
                }
                else
                {
                    /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_027: [ sf_config_to_strings_create shall call FabricGetNodeContext to get the fabric node context result. ]*/
                    IFabricNodeContextResult* node_context_result;
                    hr = FabricGetNodeContext(&node_context_result);

                    if (FAILED(hr))
                    {
                        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                        LogHRESULTError(hr, "FabricGetNodeContext failed");
                        result = NULL;
                    }
                    else
                    {
                        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_028: [ sf_config_to_strings_create shall call get_NodeContext on the fabric node context result to get the fabric node context. ]*/
                        const FABRIC_NODE_CONTEXT* node_context = node_context_result->lpVtbl->get_NodeContext(node_context_result);

                        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_029: [ sf_config_to_strings_create shall copy the NodeName from the fabric node context. ]*/
                        temp->node_name = sprintf_char("%ls", node_context->NodeName);

                        if (temp->node_name == NULL)
                        {
                            /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                            LogError("Failed to copy NodeName");
                            result = NULL;
                        }
                        else
                        {
                            /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_068: [ sf_config_to_strings_create shall call GetCodePackage on the activation_context to get the code_package. ]*/
                            IFabricCodePackage* code_package;
                            hr = activation_context->lpVtbl->GetCodePackage(activation_context, L"Code", &code_package);

                            if (FAILED(hr))
                            {
                                /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                                LogHRESULTError(hr, "GetCodePackage failed");
                                result = NULL;
                            }
                            else
                            {
                                /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_069: [ sf_config_to_strings_create shall call get_Path on the code_package_path and copy the string. ]*/
                                char* temp_code_package_path = sprintf_char("%ls", code_package->lpVtbl->get_Path(code_package));

                                if (temp_code_package_path == NULL)
                                {
                                    /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                                    LogError("Failed to copy code package path");
                                    result = NULL;
                                }
                                else
                                {
                                    THANDLE(RC_STRING) temp_code_package_path_rc = rc_string_create_with_move_memory(temp_code_package_path);

                                    if (temp_code_package_path_rc == NULL)
                                    {
                                        free(temp_code_package_path);
                                        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                                        LogError("rc_string_create_with_move_memory for code package path failed");
                                        result = NULL;
                                    }
                                    else
                                    {
                                        THANDLE_INITIALIZE_MOVE(RC_STRING)(&temp->code_package_path, &temp_code_package_path_rc);

                                        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_074: [ sf_config_to_strings_create shall generate a string from the fabric_replica_id and store it as replica_id. ]*/
                                        temp->replica_id = sprintf_char("%lld", fabric_replica_id);

                                        if (temp->replica_id == NULL)
                                        {
                                            /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
                                            LogError("Failed to generate string for replica ID (%lld)", fabric_replica_id);
                                            result = NULL;
                                        }
                                        else
                                        {
                                            /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_031: [ sf_config_to_strings_create shall return a handle to the configuration. ]*/
                                            result = temp;
                                            temp = NULL;
                                        }

                                        if (temp != NULL)
                                        {
                                            THANDLE_ASSIGN(RC_STRING)(&temp->code_package_path, NULL);
                                        }
                                    }
                                }
                                (void)code_package->lpVtbl->Release(code_package);
                            }

                            if (temp != NULL)
                            {
                                free(temp->node_name);
                            }
                        }
                        (void)node_context_result->lpVtbl->Release(node_context_result);
                    }

                    if (temp != NULL)
                    {
                        free(temp->partition_id);
                    }
                }
                (void)RpcStringFreeA(&partition_id_rpc_string);
            }

            if (temp != NULL)
            {
                free(temp);
            }
        }
    }
    return result;
}

void sf_config_to_strings_destroy(SF_CONFIG_TO_STRINGS_HANDLE handle)
{
    if (handle == NULL)
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_032: [ If handle is NULL then sf_config_to_strings_destroy shall fail and return. ]*/
        LogError("Invalid args: SF_CONFIG_TO_STRINGS_HANDLE handle = %p", handle);
    }
    else
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_033: [ sf_config_to_strings_destroy shall free all of the strings. ]*/
        free(handle->node_name);
        free(handle->partition_id);
        free(handle->replica_id);
        THANDLE_ASSIGN(RC_STRING)(&handle->code_package_path, NULL);

        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_034: [ sf_config_to_strings_destroy shall free the memory that was allocated to store the configuration. ]*/
        free(handle);
    }
}

const char* sf_config_to_strings_get_node_name(SF_CONFIG_TO_STRINGS_HANDLE handle)
{
    const char* result;

    if (handle == NULL)
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_059: [ If handle is NULL then sf_config_to_strings_get_node_name shall fail and return NULL. ]*/
        LogError("Invalid args: SF_CONFIG_TO_STRINGS_HANDLE handle = %p", handle);
        result = NULL;
    }
    else
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_060: [ sf_config_to_strings_get_node_name shall return the value that was read as node_name. ]*/
        result = handle->node_name;
    }

    return result;
}

const char* sf_config_to_strings_get_partition_id(SF_CONFIG_TO_STRINGS_HANDLE handle)
{
    const char* result;

    if (handle == NULL)
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_070: [ If handle is NULL then sf_config_to_strings_get_partition_id shall fail and return NULL. ]*/
        LogError("Invalid args: SF_CONFIG_TO_STRINGS_HANDLE handle = %p", handle);
        result = NULL;
    }
    else
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_071: [ sf_config_to_strings_get_partition_id shall return the partition_id string. ]*/
        result = handle->partition_id;
    }

    return result;
}

const char* sf_config_to_strings_get_replica_id(SF_CONFIG_TO_STRINGS_HANDLE handle)
{
    const char* result;

    if (handle == NULL)
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_072: [ If handle is NULL then sf_config_to_strings_get_replica_id shall fail and return NULL. ]*/
        LogError("Invalid args: SF_CONFIG_TO_STRINGS_HANDLE handle = %p", handle);
        result = NULL;
    }
    else
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_073: [ sf_config_to_strings_get_replica_id shall return the value that was read as replica_id. ]*/
        result = handle->replica_id;
    }

    return result;
}

THANDLE(RC_STRING) sf_config_to_strings_get_code_package_path(SF_CONFIG_TO_STRINGS_HANDLE handle)
{
    THANDLE(RC_STRING) result = NULL;

    if (handle == NULL)
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_075: [ If handle is NULL then sf_config_to_strings_get_code_package_path shall fail and return NULL. ]*/
        LogError("Invalid args: SF_CONFIG_TO_STRINGS_HANDLE handle = %p", handle);
    }
    else
    {
        /*Codes_SRS_SF_CONFIG_TO_STRINGS_42_076: [ sf_config_to_strings_get_code_package_path shall return the value that was read as code_package_path. ]*/
        THANDLE_INITIALIZE(RC_STRING)(&result, handle->code_package_path);
    }

    return result;
}

