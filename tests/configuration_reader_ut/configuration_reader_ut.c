// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
#else
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#endif

#define CINTERFACE

#include "windows.h"

#include "fabricruntime.h"
#include "fabrictypes.h"

#include "macro_utils/macro_utils.h"

#include "real_gballoc_ll.h"
static void* my_gballoc_malloc(size_t size)
{
    return real_gballoc_ll_malloc(size);
}

static void my_gballoc_free(void* ptr)
{
     real_gballoc_ll_free(ptr);
}

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_charptr.h"
#include "umock_c/umocktypes_wcharptr.h"
#include "umock_c/umocktypes_bool.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"
#include "c_util/rc_string.h"
#include "c_util/thandle.h"
#undef ENABLE_MOCKS

// Must include umock_c_prod so mocks are not expanded in real_rc_string
#include "umock_c/umock_c_prod.h"

#include "real_gballoc_hl.h"
#include "real_rc_string.h"

#include "c_util_test_helpers/rc_string_test_type.h"

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

#include "sf_c_util/configuration_reader.h"

CTEST_DECLARE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING);
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING, );

static TEST_MUTEX_HANDLE test_serialize_mutex;

static const wchar_t* test_config_package_name = L"my_package_name";
static const wchar_t* test_section_name = L"SectionName";
static const wchar_t* test_parameter_name = L"Parameter1";

static const wchar_t* test_value_to_return_default = L"foobar";

static IFabricCodePackageActivationContext test_fabric_code_package_activation_context;
static IFabricCodePackageActivationContextVtbl test_fabric_code_package_activation_context_vtbl;

static IFabricConfigurationPackage test_configuration_package;
static IFabricConfigurationPackageVtbl test_configuration_package_vtble;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static FABRIC_NODE_CONTEXT test_node_context;

MOCK_FUNCTION_WITH_CODE(, HRESULT, test_GetConfigurationPackage, IFabricCodePackageActivationContext*, This, LPCWSTR, configPackageName, IFabricConfigurationPackage**, configPackage)
    *configPackage = &test_configuration_package;
MOCK_FUNCTION_END(S_OK)

static const wchar_t* test_value_to_return;
MOCK_FUNCTION_WITH_CODE(, HRESULT, test_GetValue, IFabricConfigurationPackage*, This, LPCWSTR, sectionName, LPCWSTR, parameterName, BOOLEAN*, isEncrypted, LPCWSTR*, bufferedValue)
    *isEncrypted = FALSE;
    *bufferedValue = test_value_to_return;
MOCK_FUNCTION_END(S_OK)

MOCK_FUNCTION_WITH_CODE(, ULONG, test_Release, IFabricConfigurationPackage*, This)
MOCK_FUNCTION_END(0)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

static void setup_expectation_read_values(void)
{
    STRICT_EXPECTED_CALL(test_GetConfigurationPackage(&test_fabric_code_package_activation_context, test_config_package_name, IGNORED_ARG));
    STRICT_EXPECTED_CALL(test_GetValue(&test_configuration_package, test_section_name, test_parameter_name, IGNORED_ARG, IGNORED_ARG));
}

static void setup_expectation_read_uint_values(void)
{
    setup_expectation_read_values();
    STRICT_EXPECTED_CALL(test_Release(&test_configuration_package))
        .CallCannotFail();
}

static void setup_expectation_read_string_values(void)
{
    setup_expectation_read_values();
    STRICT_EXPECTED_CALL(vsprintf_char("%ls", IGNORED_ARG));
    STRICT_EXPECTED_CALL(test_Release(&test_configuration_package))
        .CallCannotFail();
}

static void setup_expectation_read_thandle_rc_string_values(const char* value)
{
    setup_expectation_read_values();
    STRICT_EXPECTED_CALL(vsprintf_char("%ls", IGNORED_ARG));
    STRICT_EXPECTED_CALL(rc_string_create_with_move_memory(value));
    STRICT_EXPECTED_CALL(THANDLE_INITIALIZE_MOVE(RC_STRING)(IGNORED_ARG, IGNORED_ARG));
    STRICT_EXPECTED_CALL(test_Release(&test_configuration_package))
        .CallCannotFail();
}

static void setup_expectation_read_wstring_values(void)
{
    setup_expectation_read_values();
    STRICT_EXPECTED_CALL(vsprintf_wchar(L"%s", IGNORED_ARG));
    STRICT_EXPECTED_CALL(test_Release(&test_configuration_package))
        .CallCannotFail();
}

static void setup_expectation_read_bool_values(void)
{
    setup_expectation_read_values();
    STRICT_EXPECTED_CALL(test_Release(&test_configuration_package))
        .CallCannotFail();
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error));

    ASSERT_ARE_EQUAL(int, 0, umocktypes_charptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_wcharptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_wcharptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_bool_register_types(), "umocktypes_bool_register_types");
    ASSERT_ARE_EQUAL(int, 0, umocktypes_THANDLE_RC_STRING_register_types(), "umocktypes_THANDLE_RC_STRING_register_types");

    REGISTER_RC_STRING_GLOBAL_MOCK_HOOKS();
    REGISTER_STRING_UTILS_GLOBAL_MOCK_HOOK();

    REGISTER_GLOBAL_MOCK_HOOK(malloc, my_gballoc_malloc);
    REGISTER_GLOBAL_MOCK_HOOK(free, my_gballoc_free);

    REGISTER_GLOBAL_MOCK_FAIL_RETURN(test_GetConfigurationPackage, E_FAIL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(test_GetValue, E_FAIL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(vsprintf_char, NULL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(vsprintf_wchar, NULL);

    REGISTER_UMOCK_ALIAS_TYPE(va_list, void*);
    REGISTER_TYPE(LPCWSTR, const_wcharptr);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    TEST_MUTEX_DESTROY(test_serialize_mutex);

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    if (TEST_MUTEX_ACQUIRE(test_serialize_mutex))
    {
        ASSERT_FAIL("Could not acquire test serialization mutex.");
    }

    test_fabric_code_package_activation_context.lpVtbl = &test_fabric_code_package_activation_context_vtbl;
    test_fabric_code_package_activation_context.lpVtbl->GetConfigurationPackage = test_GetConfigurationPackage;

    test_configuration_package.lpVtbl = &test_configuration_package_vtble;
    test_configuration_package.lpVtbl->GetValue = test_GetValue;
    test_configuration_package.lpVtbl->Release = test_Release;

    test_value_to_return = test_value_to_return_default;

    umock_c_reset_all_calls();
    umock_c_negative_tests_init();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();
    TEST_MUTEX_RELEASE(test_serialize_mutex);
}

//
// configuration_reader_get_uint64_t
//

/*Tests_SRS_CONFIGURATION_READER_42_001: [ If activation_context is NULL then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_null_activation_context_fails)
{
    /// arrange
    uint64_t value;

    ///act
    int result = configuration_reader_get_uint64_t(NULL, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_002: [ If config_package_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_null_config_package_name_fails)
{
    /// arrange
    uint64_t value;

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, NULL, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_002: [ If config_package_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_empty_config_package_name_fails)
{
    /// arrange
    uint64_t value;

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, L"", test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_003: [ If section_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_null_section_name_fails)
{
    /// arrange
    uint64_t value;

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, NULL, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_003: [ If section_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_empty_section_name_fails)
{
    /// arrange
    uint64_t value;

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, L"", test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_004: [ If parameter_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_null_parameter_name_fails)
{
    /// arrange
    uint64_t value;

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, NULL, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_004: [ If parameter_name is NULL or empty then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_empty_parameter_name_fails)
{
    /// arrange
    uint64_t value;

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, L"", &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_005: [ If value is NULL then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_null_value_fails)
{
    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, NULL);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_006: [ configuration_reader_get_uint64_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_007: [ configuration_reader_get_uint64_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_008: [ configuration_reader_get_uint64_t shall convert the value to uint64_t and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_011: [ configuration_reader_get_uint64_t shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_succeeds)
{
    /// arrange
    uint64_t value;

    test_value_to_return = L"42";

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(uint64_t, 42, value);
}

/*Tests_SRS_CONFIGURATION_READER_42_006: [ configuration_reader_get_uint64_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_007: [ configuration_reader_get_uint64_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_008: [ configuration_reader_get_uint64_t shall convert the value to uint64_t and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_011: [ configuration_reader_get_uint64_t shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_max_succeeds)
{
    /// arrange
    uint64_t value;

    test_value_to_return = L"18446744073709551614"; // UINT64_MAX-1 as decimal

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(uint64_t, 18446744073709551614, value);
}

/*Tests_SRS_CONFIGURATION_READER_42_006: [ configuration_reader_get_uint64_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_007: [ configuration_reader_get_uint64_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_008: [ configuration_reader_get_uint64_t shall convert the value to uint64_t and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_011: [ configuration_reader_get_uint64_t shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_with_0_succeeds)
{
    /// arrange
    uint64_t value;

    test_value_to_return = L"0";

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(uint64_t, 0, value);
}

/*Tests_SRS_CONFIGURATION_READER_42_009: [ If the value is outside the range of representable values then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_too_large_fails)
{
    /// arrange
    uint64_t value;

    test_value_to_return = L"18446744073709551616";

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_010: [ If there are any other failures then configuration_reader_get_uint64_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint64_t_fails_when_underlying_functions_fail)
{
    /// arrange
    uint64_t value;

    test_value_to_return = L"42";

    setup_expectation_read_uint_values();

    umock_c_negative_tests_snapshot();

    for (uint32_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            int result = configuration_reader_get_uint64_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

            /// assert
            ASSERT_ARE_NOT_EQUAL(int, 0, result, "On failed call %" PRIu32, i);
        }
    }
}

//
// configuration_reader_get_uint32_t
//

/*Tests_SRS_CONFIGURATION_READER_42_012: [ If activation_context is NULL then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_null_activation_context_fails)
{
    /// arrange
    uint32_t value;

    ///act
    int result = configuration_reader_get_uint32_t(NULL, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_013: [ If config_package_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_null_config_package_name_fails)
{
    /// arrange
    uint32_t value;

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, NULL, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_013: [ If config_package_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_empty_config_package_name_fails)
{
    /// arrange
    uint32_t value;

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, L"", test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_014: [ If section_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_null_section_name_fails)
{
    /// arrange
    uint32_t value;

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, NULL, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_014: [ If section_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_empty_section_name_fails)
{
    /// arrange
    uint32_t value;

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, L"", test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_015: [ If parameter_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_null_parameter_name_fails)
{
    /// arrange
    uint32_t value;

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, NULL, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_015: [ If parameter_name is NULL or empty then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_empty_parameter_name_fails)
{
    /// arrange
    uint32_t value;

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, L"", &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_016: [ If value is NULL then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_null_value_fails)
{
    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, NULL);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_017: [ configuration_reader_get_uint32_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_018: [ configuration_reader_get_uint32_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_019: [ configuration_reader_get_uint32_t shall convert the value to uint32_t and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_022: [ configuration_reader_get_uint32_t shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_succeeds)
{
    /// arrange
    uint32_t value;

    test_value_to_return = L"42";

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(uint64_t, 42, value);
}

/*Tests_SRS_CONFIGURATION_READER_42_017: [ configuration_reader_get_uint32_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_018: [ configuration_reader_get_uint32_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_019: [ configuration_reader_get_uint32_t shall convert the value to uint32_t and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_022: [ configuration_reader_get_uint32_t shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_max_succeeds)
{
    /// arrange
    uint32_t value;

    test_value_to_return = L"4294967295"; // UINT32_MAX as decimal

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(uint32_t, 4294967295, value);
}

/*Tests_SRS_CONFIGURATION_READER_42_017: [ configuration_reader_get_uint32_t shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_018: [ configuration_reader_get_uint32_t shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_019: [ configuration_reader_get_uint32_t shall convert the value to uint32_t and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_022: [ configuration_reader_get_uint32_t shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_with_0_succeeds)
{
    /// arrange
    uint32_t value;

    test_value_to_return = L"0"; // UINT32_MAX as decimal

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(uint32_t, 0, value);
}

/*Tests_SRS_CONFIGURATION_READER_42_020: [ If the value is outside the range of representable values then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_too_large_fails)
{
    /// arrange
    uint32_t value;

    test_value_to_return = L"4294967296";

    setup_expectation_read_uint_values();

    ///act
    int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_021: [ If there are any other failures then configuration_reader_get_uint32_t shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_uint32_t_fails_when_underlying_functions_fail)
{
    /// arrange
    uint32_t value;

    test_value_to_return = L"42";

    setup_expectation_read_uint_values();

    umock_c_negative_tests_snapshot();

    for (uint32_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            int result = configuration_reader_get_uint32_t(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

            /// assert
            ASSERT_ARE_NOT_EQUAL(int, 0, result, "On failed call %" PRIu32, i);
        }
    }
}

//
// configuration_reader_get_char_string
//

/*Tests_SRS_CONFIGURATION_READER_42_023: [ If activation_context is NULL then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_null_activation_context_fails)
{
    /// arrange
    char* value;

    ///act
    int result = configuration_reader_get_char_string(NULL, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_024: [ If config_package_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_null_config_package_name_fails)
{
    /// arrange
    char* value;

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, NULL, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_024: [ If config_package_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_empty_config_package_name_fails)
{
    /// arrange
    char* value;

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, L"", test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_025: [ If section_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_null_section_name_fails)
{
    /// arrange
    char* value;

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, NULL, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_025: [ If section_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_empty_section_name_fails)
{
    /// arrange
    char* value;

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, L"", test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_026: [ If parameter_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_null_parameter_name_fails)
{
    /// arrange
    char* value;

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, NULL, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_026: [ If parameter_name is NULL or empty then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_empty_parameter_name_fails)
{
    /// arrange
    char* value;

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, L"", &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_027: [ If value is NULL then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_null_value_fails)
{
    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, NULL);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_028: [ configuration_reader_get_char_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_029: [ configuration_reader_get_char_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_030: [ configuration_reader_get_char_string shall convert the value from a wide-character string to narrow-character string and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_032: [ configuration_reader_get_char_string shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_succeeds)
{
    /// arrange
    char* value;

    test_value_to_return = L"helloworld";

    setup_expectation_read_string_values();

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(char_ptr, "helloworld", value);

    ///cleanup
    free(value);
}

/*Tests_SRS_CONFIGURATION_READER_42_028: [ configuration_reader_get_char_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_029: [ configuration_reader_get_char_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_030: [ configuration_reader_get_char_string shall convert the value from a wide-character string to narrow-character string and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_032: [ configuration_reader_get_char_string shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_with_empty_string_succeeds)
{
    /// arrange
    char* value;

    test_value_to_return = L"";

    setup_expectation_read_string_values();

    ///act
    int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(char_ptr, "", value);

    ///cleanup
    free(value);
}

/*Tests_SRS_CONFIGURATION_READER_42_031: [ If there are any other failures then configuration_reader_get_char_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_char_string_fails_when_underlying_functions_fail)
{
    /// arrange
    char* value;

    test_value_to_return = L"blah";

    setup_expectation_read_string_values();

    umock_c_negative_tests_snapshot();

    for (uint32_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            int result = configuration_reader_get_char_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

            /// assert
            ASSERT_ARE_NOT_EQUAL(int, 0, result, "On failed call %" PRIu32, i);
        }
    }
}

//
// configuration_reader_get_thandle_rc_string
//

/*Tests_SRS_CONFIGURATION_READER_42_043: [ If activation_context is NULL then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_null_activation_context_fails)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    ///act
    int result = configuration_reader_get_thandle_rc_string(NULL, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_044: [ If config_package_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_null_config_package_name_fails)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, NULL, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_044: [ If config_package_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_empty_config_package_name_fails)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, L"", test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_045: [ If section_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_null_section_name_fails)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, NULL, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_045: [ If section_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_empty_section_name_fails)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, L"", test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_046: [ If parameter_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_null_parameter_name_fails)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, NULL, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_046: [ If parameter_name is NULL or empty then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_empty_parameter_name_fails)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, L"", &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_047: [ If value is NULL then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_null_value_fails)
{
    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, NULL);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_048: [ configuration_reader_get_thandle_rc_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_049: [ configuration_reader_get_thandle_rc_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_050: [ configuration_reader_get_thandle_rc_string shall convert the value from a wide-character string to narrow-character string. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_051: [ configuration_reader_get_thandle_rc_string shall store the converted string in a THANDLE(RC_STRING). ]*/
/*Tests_SRS_CONFIGURATION_READER_42_053: [ configuration_reader_get_thandle_rc_string shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_succeeds)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    test_value_to_return = L"helloworld";
    THANDLE(RC_STRING) expected_result = real_rc_string_create("helloworld");

    setup_expectation_read_thandle_rc_string_values("helloworld");

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(TEST_THANDLE_RC_STRING, expected_result, value);

    ///cleanup
    THANDLE_DEC_REF(RC_STRING)(value);
    THANDLE_DEC_REF(real_RC_STRING)(expected_result);
}

/*Tests_SRS_CONFIGURATION_READER_42_048: [ configuration_reader_get_thandle_rc_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_049: [ configuration_reader_get_thandle_rc_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_050: [ configuration_reader_get_thandle_rc_string shall convert the value from a wide-character string to narrow-character string. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_051: [ configuration_reader_get_thandle_rc_string shall store the converted string in a THANDLE(RC_STRING). ]*/
/*Tests_SRS_CONFIGURATION_READER_42_053: [ configuration_reader_get_thandle_rc_string shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_with_empty_string_succeeds)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    test_value_to_return = L"";
    THANDLE(RC_STRING) expected_result = real_rc_string_create("");

    setup_expectation_read_thandle_rc_string_values("");

    ///act
    int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(TEST_THANDLE_RC_STRING, expected_result, value);

    ///cleanup
    THANDLE_DEC_REF(RC_STRING)(value);
    THANDLE_DEC_REF(real_RC_STRING)(expected_result);
}

/*Tests_SRS_CONFIGURATION_READER_42_052: [ If there are any other failures then configuration_reader_get_thandle_rc_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_thandle_rc_string_fails_when_underlying_functions_fail)
{
    /// arrange
    THANDLE(RC_STRING) value = NULL;

    test_value_to_return = L"blah";

    setup_expectation_read_thandle_rc_string_values("blah");

    umock_c_negative_tests_snapshot();

    for (uint32_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            int result = configuration_reader_get_thandle_rc_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

            /// assert
            ASSERT_ARE_NOT_EQUAL(int, 0, result, "On failed call %" PRIu32, i);
        }
    }
}

//
// configuration_reader_get_wchar_string
//

/*Tests_SRS_CONFIGURATION_READER_42_033: [ If activation_context is NULL then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_null_activation_context_fails)
{
    /// arrange
    wchar_t* value;

    ///act
    int result = configuration_reader_get_wchar_string(NULL, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_034: [ If config_package_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_null_config_package_name_fails)
{
    /// arrange
    wchar_t* value;

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, NULL, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_034: [ If config_package_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_empty_config_package_name_fails)
{
    /// arrange
    wchar_t* value;

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, L"", test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_035: [ If section_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_null_section_name_fails)
{
    /// arrange
    wchar_t* value;

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, NULL, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_035: [ If section_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_empty_section_name_fails)
{
    /// arrange
    wchar_t* value;

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, L"", test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_036: [ If parameter_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_null_parameter_name_fails)
{
    /// arrange
    wchar_t* value;

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, NULL, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_036: [ If parameter_name is NULL or empty then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_empty_parameter_name_fails)
{
    /// arrange
    wchar_t* value;

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, L"", &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_037: [ If value is NULL then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_null_value_fails)
{
    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, NULL);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_42_038: [ configuration_reader_get_wchar_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_039: [ configuration_reader_get_wchar_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_040: [ configuration_reader_get_wchar_string shall copy the string and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_042: [ configuration_reader_get_wchar_string shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_succeeds)
{
    /// arrange
    wchar_t* value;

    test_value_to_return = L"helloworld";

    setup_expectation_read_wstring_values();

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(wchar_ptr, L"helloworld", value);

    ///cleanup
    free(value);
}

/*Tests_SRS_CONFIGURATION_READER_42_038: [ configuration_reader_get_wchar_string shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_039: [ configuration_reader_get_wchar_string shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_040: [ configuration_reader_get_wchar_string shall copy the string and store it in value. ]*/
/*Tests_SRS_CONFIGURATION_READER_42_042: [ configuration_reader_get_wchar_string shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_with_empty_string_succeeds)
{
    /// arrange
    wchar_t* value;

    test_value_to_return = L"";

    setup_expectation_read_wstring_values();

    ///act
    int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(wchar_ptr, L"", value);

    ///cleanup
    free(value);
}

/*Tests_SRS_CONFIGURATION_READER_42_041: [ If there are any other failures then configuration_reader_get_wchar_string shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_wchar_string_fails_when_underlying_functions_fail)
{
    /// arrange
    wchar_t* value;

    test_value_to_return = L"blah";

    setup_expectation_read_wstring_values();

    umock_c_negative_tests_snapshot();

    for (uint32_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            int result = configuration_reader_get_wchar_string(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

            /// assert
            ASSERT_ARE_NOT_EQUAL(int, 0, result, "On failed call %" PRIu32, i);
        }
    }
}


//
// configuration_reader_get_bool
//

/*Tests_SRS_CONFIGURATION_READER_03_001: [ If activation_context is NULL then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_null_activation_context_fails)
{
    /// arrange
    bool value;

    ///act
    int result = configuration_reader_get_bool(NULL, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_002: [ If config_package_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_null_config_package_name_fails)
{
    /// arrange
    bool value;

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, NULL, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_002: [ If config_package_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_empty_config_package_name_fails)
{
    /// arrange
    bool value;

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, L"", test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_003: [ If section_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_null_section_name_fails)
{
    /// arrange
    bool value;

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, NULL, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_003: [ If section_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_empty_section_name_fails)
{
    /// arrange
    bool value;

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, L"", test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_004: [ If parameter_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_null_parameter_name_fails)
{
    /// arrange
    bool value;

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, NULL, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_004: [ If parameter_name is NULL or empty then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_empty_parameter_name_fails)
{
    /// arrange
    bool value;

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, L"", &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}


/*Tests_SRS_CONFIGURATION_READER_03_005: [ If value is NULL then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_with_null_value_fails)
{
    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, NULL);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_006: [ configuration_reader_get_bool shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_03_007: [ configuration_reader_get_bool shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_03_010: [ If the string is True, configuration_reader_get_bool shall set value to true and return 0. ]*/
/*Tests_SRS_CONFIGURATION_READER_03_012: [ configuration_reader_get_bool shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_bool_succeeds_returns_true)
{
    /// arrange
    bool value;

    test_value_to_return = L"True";

    setup_expectation_read_bool_values();

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_IS_TRUE(value);
}

// Tests_SRS_CONFIGURATION_READER_11_001: [ configuration_reader_get_bool shall do a case insensitive comparison of the string. ]
TEST_FUNCTION(configuration_reader_get_bool_succeeds_returns_true_on_mix_case)
{
    /// arrange
    bool value;

    test_value_to_return = L"trUE";

    setup_expectation_read_bool_values();

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_IS_TRUE(value);
}


/*Tests_SRS_CONFIGURATION_READER_03_006: [ configuration_reader_get_bool shall call the GetConfigurationPackage function on activation_context with config_package_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_03_007: [ configuration_reader_get_bool shall call GetValue on the configuration package with section_name and parameter_name. ]*/
/*Tests_SRS_CONFIGURATION_READER_03_009: [ If the string is False, configuration_reader_get_bool shall set value to false and return 0. ]*/
/*Tests_SRS_CONFIGURATION_READER_03_012: [ configuration_reader_get_bool shall succeed and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_bool_succeeds_returns_false)
{
    /// arrange
    bool value;

    test_value_to_return = L"False";

    setup_expectation_read_bool_values();

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_IS_FALSE(value);
}

// Tests_SRS_CONFIGURATION_READER_11_001: [ configuration_reader_get_bool shall do a case insensitive comparison of the string. ]
TEST_FUNCTION(configuration_reader_get_bool_succeeds_returns_false_on_mix_case)
{
    /// arrange
    bool value;

    test_value_to_return = L"falSE";

    setup_expectation_read_bool_values();

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_IS_FALSE(value);
}

/*Tests_SRS_CONFIGURATION_READER_03_013: [ If the string is anything other than the above, configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_fails_with_random_string)
{
    /// arrange
    bool value;

    test_value_to_return = L"rAndOm";

    setup_expectation_read_bool_values();

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_NOT_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_014: [ If the string is an empty string, configuration_reader_get_bool shall set value to false and return 0. ]*/
TEST_FUNCTION(configuration_reader_get_bool_returns_false_with_empty_string)
{
    /// arrange
    bool value;

    test_value_to_return = L"";

    setup_expectation_read_bool_values();

    ///act
    int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_CONFIGURATION_READER_03_011: [ If there are any other failures then configuration_reader_get_bool shall fail and return a non-zero value. ]*/
TEST_FUNCTION(configuration_reader_get_bool_fails_when_underlying_functions_fail)
{
    /// arrange
    bool value;

    test_value_to_return = L"blah";

    setup_expectation_read_bool_values();

    umock_c_negative_tests_snapshot();

    for (uint32_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            int result = configuration_reader_get_bool(&test_fabric_code_package_activation_context, test_config_package_name, test_section_name, test_parameter_name, &value);

            /// assert
            ASSERT_ARE_NOT_EQUAL(int, 0, result, "On failed call %" PRIu32, i);
        }
    }
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
