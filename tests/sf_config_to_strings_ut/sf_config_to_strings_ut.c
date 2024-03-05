// Copyright (c) Microsoft. All rights reserved.


#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>


#include "fabricruntime.h"
#include "fabrictypes.h"

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_charptr.h"
#include "umock_c/umocktypes_wcharptr.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"

#include "c_util/rc_string.h"
#include "c_pal/thandle.h"


MOCKABLE_FUNCTION(, RPC_STATUS, mocked_UuidToStringA, const UUID*, Uuid, RPC_CSTR*, StringUuid);
MOCKABLE_FUNCTION(, RPC_STATUS, mocked_RpcStringFreeA, RPC_CSTR*, String);
MOCKABLE_FUNCTION(, HRESULT, mocked_FabricGetNodeContext, void**, nodeContext);



#undef ENABLE_MOCKS

// Must include umock_c_prod so mocks are not expanded in real_rc_string
#include "umock_c/umock_c_prod.h"

#include "real_gballoc_hl.h"
#include "real_rc_string.h"

#include "c_util_test_helpers/rc_string_test_type.h"
#include "c_util_test_helpers/rc_string_test_decl.h"


/*following function cannot be mocked because of variable number of arguments:( so it is copy&pasted here*/
char* sprintf_char_function(const char* format, ...)
{
    char* result;
    va_list va;
    va_start(va, format);
    result = vsprintf_char(format, va);
    va_end(va);
    return result;
}

/*following function cannot be mocked because of variable number of arguments:( so it is copy&pasted here*/
wchar_t* sprintf_wchar_function(const wchar_t* format, ...)
{
    wchar_t* result;
    va_list va;
    va_start(va, format);
    result = vsprintf_wchar(format, va);
    va_end(va);
    return result;
}

#include "real_string_utils.h"

#include "sf_c_util\sf_config_to_strings.h"

CTEST_DECLARE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING);
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING, );

RC_STRING_TEST_DECL(
    test_offload_container_name_rc, "my-container-",
    test_offload_container_name_full_rc, "my-container-abcdef12-4242-4343-0102-030405060708",
    test_code_path_rc, "c:/my/path/blah"
)

static IFabricNodeContextResult test_fabric_node_context_result;
static IFabricNodeContextResultVtbl test_fabric_node_context_result_vtbl;

static FABRIC_NODE_CONTEXT test_fabric_node_context;

static IFabricCodePackageActivationContext test_fabric_code_package_activation_context;
static IFabricCodePackageActivationContextVtbl test_fabric_code_package_activation_context_vtbl;

static IFabricCodePackage test_code_package;
static IFabricCodePackageVtbl test_code_package_vtbl;

static FABRIC_ENDPOINT_RESOURCE_DESCRIPTION test_block_storage_endpoint_resource_description;
static FABRIC_ENDPOINT_RESOURCE_DESCRIPTION test_block_storage_backdoor_endpoint_resource_description;
static FABRIC_ENDPOINT_RESOURCE_DESCRIPTION test_test_hook_endpoint_resource_description;

static const wchar_t* test_block_storage_tcp_endpoint_name = L"BlockStorageTCPServerTypeEndpoint";
static const wchar_t* test_block_storage_backdoor_tcp_endpoint_name = L"BlockStorageBackdoorTCPServerTypeEndpoint";
static const wchar_t* test_hook_endpoint_name = L"BlockStorageTCPTestEndpoint";

static const wchar_t* test_node_name_wchar = L"MyNode_1";
static const char* test_node_name = "MyNode_1";

static const wchar_t* test_code_path_wchar = L"c:/my/path/blah";
static const char* test_code_path = "c:/my/path/blah";

static const char* test_offload_container_name = "my-container-";

static FABRIC_PARTITION_ID test_fabric_partition_id = { 0xABCDEF12, 0x4242, 0x4343, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08} };
static FABRIC_REPLICA_ID test_fabric_replica_id = 123457999;

static const char* test_expected_partition_id = "abcdef12-4242-4343-0102-030405060708";
static const char* test_expected_replica_id = "123457999";

static const char* test_offload_container_name_full = "my-container-abcdef12-4242-4343-0102-030405060708";


RPC_STATUS hook_UuidToStringA(
    const UUID* Uuid,
    RPC_CSTR* StringUuid)
{
    return UuidToStringA(Uuid, StringUuid);
}

RPC_STATUS hook_RpcStringFreeA(
    RPC_CSTR* String)
{
    return RpcStringFreeA(String);
}

HRESULT hook_FabricGetNodeContext(
    void** nodeContext)
{
    ASSERT_IS_NOT_NULL(nodeContext);
    *nodeContext = (void*)&test_fabric_node_context_result;
    return S_OK;
}

MOCK_FUNCTION_WITH_CODE(, const FABRIC_NODE_CONTEXT*, test_get_NodeContext, IFabricNodeContextResult*, This)
MOCK_FUNCTION_END(&test_fabric_node_context);

MOCK_FUNCTION_WITH_CODE(, ULONG, test_node_context_Release, IFabricNodeContextResult*, This)
MOCK_FUNCTION_END(0);

MOCK_FUNCTION_WITH_CODE(, HRESULT, test_GetCodePackage, IFabricCodePackageActivationContext*, This, LPCWSTR, codePackageName, IFabricCodePackage**, codePackage)
    *codePackage = &test_code_package;
MOCK_FUNCTION_END(S_OK);

MOCK_FUNCTION_WITH_CODE(, HRESULT, test_get_service_endpoint_resource, IFabricCodePackageActivationContext*, This, LPCWSTR, serviceEndpointResourceName, const FABRIC_ENDPOINT_RESOURCE_DESCRIPTION**, fabric_endpoint_resource)
if (lstrcmpW(serviceEndpointResourceName, test_block_storage_tcp_endpoint_name) == 0)
{
    *fabric_endpoint_resource = &test_block_storage_endpoint_resource_description;
}
else if (lstrcmpW(serviceEndpointResourceName, test_block_storage_backdoor_tcp_endpoint_name) == 0)
{
    *fabric_endpoint_resource = &test_block_storage_backdoor_endpoint_resource_description;
}
else
{
    *fabric_endpoint_resource = &test_test_hook_endpoint_resource_description;
}
MOCK_FUNCTION_END(S_OK);

MOCK_FUNCTION_WITH_CODE(, LPCWSTR, test_get_Path, IFabricCodePackage*, This)
MOCK_FUNCTION_END(test_code_path_wchar);

MOCK_FUNCTION_WITH_CODE(, ULONG, test_code_package_Release, IFabricCodePackage*, This)
MOCK_FUNCTION_END(0);

static void setup_create_expectations(void)
{
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_UuidToStringA(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(vsprintf_char("%s", IGNORED_ARG)); // copy partition id
    STRICT_EXPECTED_CALL(mocked_FabricGetNodeContext(IGNORED_ARG));
    STRICT_EXPECTED_CALL(test_get_NodeContext(&test_fabric_node_context_result))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(vsprintf_char("%ls", IGNORED_ARG)); // copy node name
    STRICT_EXPECTED_CALL(test_GetCodePackage(&test_fabric_code_package_activation_context, L"Code", IGNORED_ARG));
    STRICT_EXPECTED_CALL(test_get_Path(&test_code_package))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(vsprintf_char("%ls", IGNORED_ARG)); // copy code path
    STRICT_EXPECTED_CALL(rc_string_create_with_move_memory(test_code_path));
    STRICT_EXPECTED_CALL(THANDLE_INITIALIZE_MOVE(RC_STRING)(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(vsprintf_char("%lld", IGNORED_ARG)); // format replica ID

    STRICT_EXPECTED_CALL(test_code_package_Release(&test_code_package))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(test_node_context_Release(&test_fabric_node_context_result))
        .CallCannotFail();
    STRICT_EXPECTED_CALL(mocked_RpcStringFreeA(IGNORED_ARG))
        .CallCannotFail();
}

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error));

    ASSERT_ARE_EQUAL(int, 0, umocktypes_charptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_wcharptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_THANDLE_RC_STRING_register_types(), "umocktypes_THANDLE_RC_STRING_register_types");

    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();
    REGISTER_RC_STRING_GLOBAL_MOCK_HOOKS();
    REGISTER_STRING_UTILS_GLOBAL_MOCK_HOOK();


    REGISTER_GLOBAL_MOCK_HOOK(mocked_UuidToStringA, hook_UuidToStringA);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(mocked_UuidToStringA, RPC_E_FAULT);
    REGISTER_GLOBAL_MOCK_HOOK(mocked_RpcStringFreeA, hook_RpcStringFreeA);
    REGISTER_GLOBAL_MOCK_HOOK(mocked_FabricGetNodeContext, hook_FabricGetNodeContext);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(mocked_FabricGetNodeContext, E_FAIL);

    REGISTER_GLOBAL_MOCK_FAIL_RETURN(test_GetCodePackage, E_FAIL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(test_get_service_endpoint_resource, E_FAIL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(vsprintf_char, NULL);

    REGISTER_UMOCK_ALIAS_TYPE(va_list, void*);
    REGISTER_TYPE(LPCWSTR, const_wcharptr);

    rc_string_test_init_statics();
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    rc_string_test_cleanup_statics();

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    test_block_storage_endpoint_resource_description.Port = 666;
    test_block_storage_backdoor_endpoint_resource_description.Port = 667;
    test_test_hook_endpoint_resource_description.Port = 420;

    test_fabric_node_context_result.lpVtbl = &test_fabric_node_context_result_vtbl;
    test_fabric_node_context_result.lpVtbl->get_NodeContext = test_get_NodeContext;
    test_fabric_node_context_result.lpVtbl->Release = test_node_context_Release;

    test_fabric_node_context.NodeName = test_node_name_wchar;

    test_fabric_code_package_activation_context.lpVtbl = &test_fabric_code_package_activation_context_vtbl;
    test_fabric_code_package_activation_context.lpVtbl->GetCodePackage = test_GetCodePackage;
    test_fabric_code_package_activation_context.lpVtbl->GetServiceEndpointResource = test_get_service_endpoint_resource;


    test_code_package.lpVtbl = &test_code_package_vtbl;
    test_code_package.lpVtbl->get_Path = test_get_Path;
    test_code_package.lpVtbl->Release = test_code_package_Release;

    umock_c_reset_all_calls();
    umock_c_negative_tests_init();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();
}

//
// sf_config_to_strings_create
//

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_001: [ If activation_context is NULL then sf_config_to_strings_create shall fail and return NULL. ]*/
TEST_FUNCTION(sf_config_to_strings_create_with_null_activation_context_fails)
{
    ///act
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(NULL, &test_fabric_partition_id, test_fabric_replica_id);

    ///assert
    ASSERT_IS_NULL(handle);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_063: [ If fabric_partition_id is NULL then sf_config_to_strings_create shall fail and return NULL. ]*/
TEST_FUNCTION(sf_config_to_strings_create_with_null_fabric_partition_id_fails)
{
    ///act
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, NULL, test_fabric_replica_id);

    ///assert
    ASSERT_IS_NULL(handle);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_002: [ sf_config_to_strings_create shall allocate memory for the SF_CONFIG_TO_STRINGS_HANDLE. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_064: [ sf_config_to_strings_create shall call UuidToStringA to convert the fabric_partition_id to a string. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_082: [ sf_config_to_strings_create shall copy the result of UuidToStringA to a string. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_027: [ sf_config_to_strings_create shall call FabricGetNodeContext to get the fabric node context result. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_028: [ sf_config_to_strings_create shall call get_NodeContext on the fabric node context result to get the fabric node context. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_029: [ sf_config_to_strings_create shall copy the NodeName from the fabric node context. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_068: [ sf_config_to_strings_create shall call GetCodePackage on the activation_context to get the code_package. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_069: [ sf_config_to_strings_create shall call get_Path on the code_package_path and copy the string. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_074: [ sf_config_to_strings_create shall generate a string from the fabric_replica_id and store it as replica_id. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_031: [ sf_config_to_strings_create shall return a handle to the configuration. ]*/
TEST_FUNCTION(sf_config_to_strings_create_succeeds)
{
    ///arrange
    setup_create_expectations();

    ///act
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, &test_fabric_partition_id, test_fabric_replica_id);

    ///assert
    ASSERT_IS_NOT_NULL(handle);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    sf_config_to_strings_destroy(handle);
}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_030: [ If there are any other failures then sf_config_to_strings_create shall fail and return NULL. ]*/
TEST_FUNCTION(sf_config_to_strings_create_fails_when_underlying_functions_fail)
{
    ///arrange
    setup_create_expectations();

    umock_c_negative_tests_snapshot();

    for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, &test_fabric_partition_id, test_fabric_replica_id);

            /// assert
            ASSERT_IS_NULL(handle, "On failed call %zu", i);
        }
    }
}

//
// sf_config_to_strings_destroy
//

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_032: [ If handle is NULL then sf_config_to_strings_destroy shall fail and return. ]*/
TEST_FUNCTION(sf_config_to_strings_destroy_with_null_handle_fails)
{
    ///act
    sf_config_to_strings_destroy(NULL);

    ///assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_033: [ sf_config_to_strings_destroy shall free all of the strings. ]*/
/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_034: [ sf_config_to_strings_destroy shall free the memory that was allocated to store the configuration. ]*/
TEST_FUNCTION(sf_config_to_strings_destroy_works)
{
    ///arrange
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, &test_fabric_partition_id, test_fabric_replica_id);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));
    STRICT_EXPECTED_CALL(THANDLE_ASSIGN(RC_STRING)(IGNORED_ARG, NULL));
    // STRICT_EXPECTED_CALL(THANDLE_ASSIGN(RC_STRING)(IGNORED_ARG, NULL));
    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    sf_config_to_strings_destroy(handle);

    ///assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

}

//
// sf_config_to_strings_get_node_name
//

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_059: [ If handle is NULL then sf_config_to_strings_get_node_name shall fail and return NULL. ]*/
TEST_FUNCTION(sf_config_to_strings_get_node_name_with_null_handle_fails)
{
    ///act
    const char* node_name = sf_config_to_strings_get_node_name(NULL);

    ///assert
    ASSERT_IS_NULL(node_name);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_060: [ sf_config_to_strings_get_node_name shall return the value that was read as node_name. ]*/
TEST_FUNCTION(sf_config_to_strings_get_node_name_succeeds)
{
    ///arrange
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, &test_fabric_partition_id, test_fabric_replica_id);
    umock_c_reset_all_calls();

    ///act
    const char* node_name = sf_config_to_strings_get_node_name(handle);

    ///assert
    ASSERT_IS_NOT_NULL(node_name);
    ASSERT_ARE_EQUAL(char_ptr, test_node_name, node_name);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    sf_config_to_strings_destroy(handle);
}

//
// sf_config_to_strings_get_partition_id
//

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_070: [ If handle is NULL then sf_config_to_strings_get_partition_id shall fail and return NULL. ]*/
TEST_FUNCTION(sf_config_to_strings_get_partition_id_with_null_handle_fails)
{
    ///act
    const char* partition_id = sf_config_to_strings_get_partition_id(NULL);

    ///assert
    ASSERT_IS_NULL(partition_id);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_071: [ sf_config_to_strings_get_partition_id shall return the partition_id string. ]*/
TEST_FUNCTION(sf_config_to_strings_get_partition_id_succeeds)
{
    ///arrange
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, &test_fabric_partition_id, test_fabric_replica_id);
    umock_c_reset_all_calls();

    ///act
    const char* partition_id = sf_config_to_strings_get_partition_id(handle);

    ///assert
    ASSERT_IS_NOT_NULL(partition_id);
    ASSERT_ARE_EQUAL(char_ptr, test_expected_partition_id, partition_id);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    sf_config_to_strings_destroy(handle);
}

//
// sf_config_to_strings_get_replica_id
//

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_072: [ If handle is NULL then sf_config_to_strings_get_replica_id shall fail and return NULL. ]*/
TEST_FUNCTION(sf_config_to_strings_get_replica_id_with_null_handle_fails)
{
    ///act
    const char* replica_id = sf_config_to_strings_get_replica_id(NULL);

    ///assert
    ASSERT_IS_NULL(replica_id);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_073: [ sf_config_to_strings_get_replica_id shall return the value that was read as replica_id. ]*/
TEST_FUNCTION(sf_config_to_strings_get_replica_id_succeeds)
{
    ///arrange
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, &test_fabric_partition_id, test_fabric_replica_id);
    umock_c_reset_all_calls();

    ///act
    const char* replica_id = sf_config_to_strings_get_replica_id(handle);

    ///assert
    ASSERT_IS_NOT_NULL(replica_id);
    ASSERT_ARE_EQUAL(char_ptr, test_expected_replica_id, replica_id);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    sf_config_to_strings_destroy(handle);
}

//
// sf_config_to_strings_get_code_package_path
//

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_075: [ If handle is NULL then sf_config_to_strings_get_code_package_path shall fail and return NULL. ]*/
TEST_FUNCTION(sf_config_to_strings_get_code_package_path_with_null_handle_fails)
{
    ///act
    THANDLE(RC_STRING) code_path = sf_config_to_strings_get_code_package_path(NULL);

    ///assert
    ASSERT_IS_NULL(code_path);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_CONFIG_TO_STRINGS_42_076: [ sf_config_to_strings_get_code_package_path shall return the value that was read as code_package_path. ]*/
TEST_FUNCTION(sf_config_to_strings_get_code_package_path_succeeds)
{
    ///arrange
    SF_CONFIG_TO_STRINGS_HANDLE handle = sf_config_to_strings_create(&test_fabric_code_package_activation_context, &test_fabric_partition_id, test_fabric_replica_id);
    umock_c_reset_all_calls();

    STRICT_EXPECTED_CALL(THANDLE_INITIALIZE(RC_STRING)(IGNORED_ARG, g.test_code_path_rc));

    ///act
    THANDLE(RC_STRING) code_path = sf_config_to_strings_get_code_package_path(handle);

    ///assert
    ASSERT_IS_NOT_NULL(code_path);
    ASSERT_ARE_EQUAL(TEST_THANDLE_RC_STRING, g.test_code_path_rc, code_path);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    sf_config_to_strings_destroy(handle);
    THANDLE_ASSIGN(real_RC_STRING)(&code_path, NULL);
}

//
// sf_config_to_strings_get_offload_blob_container_name
//



END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
