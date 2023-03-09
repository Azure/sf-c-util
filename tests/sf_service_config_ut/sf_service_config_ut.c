// Copyright (c) Microsoft. All rights reserved.

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

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
#include "umock_c/umocktypes_stdint.h"
#include "umock_c/umocktypes_charptr.h"
#include "umock_c/umocktypes_wcharptr.h"
#include "umock_c/umock_c_negative_tests.h"

#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"
#include "c_util/rc_string.h"
#include "c_util/thandle.h"
#include "sf_c_util/configuration_reader.h"
#undef ENABLE_MOCKS

// Must include umock_c_prod so mocks are not expanded in real_rc_string
#include "umock_c/umock_c_prod.h"

#include "real_gballoc_hl.h"
#include "real_rc_string.h"
#include "real_string_utils.h"
#include "c_util_test_helpers/rc_string_test_decl.h"
#include "c_util_test_helpers/rc_string_test_type.h"

#include "sf_service_config_ut_helpers.h"
#include "test_sf_service_config.h"

#include "sf_c_util/sf_service_config.h"

CTEST_DECLARE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING);
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING, );

static TEST_MUTEX_HANDLE test_serialize_mutex;


RC_STRING_TEST_DECL(
    empty_string, ""
)

typedef THANDLE(RC_STRING) thandle_rc_string;

static const wchar_t* expected_config_package_name = L"default_config";
static const wchar_t* expected_section_name = L"MyConfigSectionName";

TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOKS(my_config, MY_CONFIG_TEST_PARAMS)

TEST_SF_SERVICE_CONFIG_DEFINE_EXPECTED_CALL_HELPERS(my_config, expected_config_package_name, expected_section_name, MY_CONFIG_TEST_PARAMS)


// Also test that the generated code can be mocked

#define ENABLE_MOCKS
#include "umock_c/umock_c_prod.h"

#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_parameter_1 L"Parameter1"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_parameter_2 L"Parameter2"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_parameter_3 L"Parameter3WithLongerName"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_some_flag L"SomeFlag"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option_in_thandle L"StringOptionThandle"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option L"MyString"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_wide_string_option L"MyWideString"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option_in_thandle_optional L"OptionalStringOptionThandle"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option_optional L"OptionalStringOption"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_wide_string_option_optional L"OptionalWideStringOption"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_another_flag L"AnotherFlag"

#define MY_MOCKED_CONFIG_TEST_PARAMS \
    CONFIG_REQUIRED(uint64_t, mocked_parameter_1), \
    CONFIG_REQUIRED(uint64_t, mocked_parameter_2), \
    CONFIG_REQUIRED(uint32_t, mocked_parameter_3), \
    CONFIG_REQUIRED(bool, mocked_some_flag), \
    CONFIG_REQUIRED(thandle_rc_string, mocked_string_option_in_thandle), \
    CONFIG_REQUIRED(char_ptr, mocked_string_option), \
    CONFIG_REQUIRED(wchar_ptr, mocked_wide_string_option), \
    CONFIG_OPTIONAL(thandle_rc_string, mocked_string_option_in_thandle_optional), \
    CONFIG_OPTIONAL(char_ptr, mocked_string_option_optional), \
    CONFIG_OPTIONAL(wchar_ptr, mocked_wide_string_option_optional), \
    CONFIG_REQUIRED(bool, mocked_another_flag) \

DECLARE_SF_SERVICE_CONFIG(my_mocked_config, MY_MOCKED_CONFIG_TEST_PARAMS)

#undef ENABLE_MOCKS

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error));

    ASSERT_ARE_EQUAL(int, 0, umocktypes_stdint_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_charptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_wcharptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_THANDLE_RC_STRING_register_types(), "umocktypes_THANDLE_RC_STRING_register_types");

    REGISTER_RC_STRING_GLOBAL_MOCK_HOOKS();
    REGISTER_STRING_UTILS_GLOBAL_MOCK_HOOK();
    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();

    REGISTER_GLOBAL_MOCK_HOOK(malloc, my_gballoc_malloc);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(malloc_flex, NULL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(realloc, NULL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(realloc_2, NULL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(realloc_flex, NULL);
    REGISTER_GLOBAL_MOCK_HOOK(free, my_gballoc_free);

    REGISTER_UMOCK_ALIAS_TYPE(THANDLE(SF_SERVICE_CONFIG(my_mocked_config)), void*);
    TEST_SF_SERVICE_CONFIG_HOOK_CONFIGURATION_READER(my_config)

    rc_string_test_init_statics();
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    rc_string_test_cleanup_statics();

    TEST_MUTEX_DESTROY(test_serialize_mutex);

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    if (TEST_MUTEX_ACQUIRE(test_serialize_mutex))
    {
        ASSERT_FAIL("Could not acquire test serialization mutex.");
    }

    // Initialize so each default value is returned (individual tests can override what gets returned)
    TEST_SF_SERVICE_CONFIG_RESET(MY_CONFIG_TEST_PARAMS);

    umock_c_reset_all_calls();
    umock_c_negative_tests_init();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();

    TEST_SF_SERVICE_CONFIG_CLEANUP(MY_CONFIG_TEST_PARAMS);

    TEST_MUTEX_RELEASE(test_serialize_mutex);
}

TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_is_mockable)
{
    // arrange
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_CREATE(my_mocked_config)(IGNORED_ARG))
        .SetReturn((SF_SERVICE_CONFIG(my_mocked_config)*)0xABCD);

    // act
    THANDLE(SF_SERVICE_CONFIG(my_mocked_config)) result = SF_SERVICE_CONFIG_CREATE(my_mocked_config)((IFabricCodePackageActivationContext*)42);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(void_ptr, (void*)0xABCD, (void*)result);
}

TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_is_mockable)
{
    // arrange
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_2)(IGNORED_ARG))
        .SetReturn(42);

    // act
    uint64_t result = SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_2)((SF_SERVICE_CONFIG(my_mocked_config)*)0xffff);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    ASSERT_ARE_EQUAL(uint64_t, 42, result);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_001: [ DECLARE_SF_SERVICE_CONFIG_HANDLE shall generate a THANDLE declaration of type SF_SERVICE_CONFIG(name). ]*/
static THANDLE(my_config_CONFIGURATION) my_config_CONFIGURATION_has_THANDLE = NULL;

// Tested implicitly in the cases below
/*Tests_SRS_SF_SERVICE_CONFIG_42_004: [ DEFINE_SF_SERVICE_CONFIG shall generate the SF_SERVICE_CONFIG(name) struct. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_005: [ DEFINE_SF_SERVICE_CONFIG shall generate the implementation of SF_SERVICE_CONFIG_CREATE(name). ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_006: [ DECLARE_SF_SERVICE_CONFIG shall generate the implementation of the getter functions SF_SERVICE_CONFIG_GETTER(name, param) for each of the configurations provided. ]*/

/*Tests_SRS_SF_SERVICE_CONFIG_42_007: [ SF_SERVICE_CONFIG shall expand to the name of the configuration module by appending the suffix _CONFIGURATION. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_macro_expands_to_name)
{
    // arrange

    // act
    const char* name = MU_TOSTRING(SF_SERVICE_CONFIG(name));

    // assert
    ASSERT_ARE_EQUAL(char_ptr, "name_CONFIGURATION", name);
}

//
// SF_SERVICE_CONFIG_CREATE
//

/*Tests_SRS_SF_SERVICE_CONFIG_42_008: [ SF_SERVICE_CONFIG_CREATE shall expand to the name of the create function for the configuration module by appending the suffix _configuration_create. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_macro_expands_to_name)
{
    // arrange

    // act
    const char* name = MU_TOSTRING(SF_SERVICE_CONFIG_CREATE(name));

    // assert
    ASSERT_ARE_EQUAL(char_ptr, "name_configuration_create", name);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_009: [ If activation_context is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_NULL_activation_context_fails)
{
    // arrange

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(NULL);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_002: [ DECLARE_SF_SERVICE_CONFIG_GETTERS shall generate a mockable create function SF_SERVICE_CONFIG_CREATE(name) which takes an IFabricCodePackageActivationContext* and produces the THANDLE. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_010: [ SF_SERVICE_CONFIG_CREATE(name) shall allocate the THANDLE(SF_SERVICE_CONFIG(name)) with MU_C2A(SF_SERVICE_CONFIG(name), _dispose) as the dispose function. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_011: [ SF_SERVICE_CONFIG_CREATE(name) shall call AddRef and store the activation_context. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_012: [ SF_SERVICE_CONFIG_CREATE(name) shall store the sf_config_name and sf_parameters_section_name. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_013: [ For each configuration value with name config_name: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_014: [ If the type is bool then: ]*/
        /*Tests_SRS_SF_SERVICE_CONFIG_42_015: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_bool with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_016: [ If the type is uint32_t then: ]*/
        /*Tests_SRS_SF_SERVICE_CONFIG_42_017: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_uint32_t with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_019: [ If the type is uint64_t then: ]*/
        /*Tests_SRS_SF_SERVICE_CONFIG_42_020: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_uint64_t with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_022: [ If the type is char_ptr then: ]*/
        /*Tests_SRS_SF_SERVICE_CONFIG_42_023: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_char_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_026: [ If the type is wchar_ptr then: ]*/
        /*Tests_SRS_SF_SERVICE_CONFIG_42_027: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_wchar_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_030: [ If the type is thandle_rc_string then: ]*/
        /*Tests_SRS_SF_SERVICE_CONFIG_42_031: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_thandle_rc_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_normal_values_for_all_types_succeeds)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&result, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_fails_when_underlying_functions_fail)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();

    umock_c_negative_tests_snapshot();

    for (size_t i = 0; i < umock_c_negative_tests_call_count(); i++)
    {
        if (umock_c_negative_tests_can_call_fail(i))
        {
            umock_c_negative_tests_reset();
            umock_c_negative_tests_fail_call(i);

            /// act
            THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

            /// assert
            ASSERT_IS_NULL(result, "On failed call %zu", i);
        }
    }
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_016: [ If the type is uint32_t then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_017: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_uint32_t with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_018: [ If the result is UINT32_MAX then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_fails_when_uint32_t_value_is_UINT32_MAX)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(parameter_3) = UINT32_MAX;
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, parameter_3));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_019: [ If the type is uint64_t then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_020: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_uint64_t with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_021: [ If the result is UINT64_MAX then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_fails_when_uint64_t_value_is_UINT64_MAX)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(parameter_1) = UINT64_MAX;
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, parameter_1));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_022: [ If the type is char_ptr then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_023: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_char_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_024: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_empty_optional_string_succeeds)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_optional) = "";
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&result, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_026: [ If the type is wchar_ptr then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_027: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_wchar_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_028: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_empty_optional_wide_string_succeeds)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(wide_string_option_optional) = L"";
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&result, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_030: [ If the type is thandle_rc_string then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_031: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_thandle_rc_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_032: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_empty_optional_thandle_rc_string_succeeds)
{
    // arrange
    THANDLE_ASSIGN(real_RC_STRING)(&TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_in_thandle_optional).x, g.empty_string);
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NOT_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&result, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_022: [ If the type is char_ptr then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_023: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_char_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_024: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_025: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_empty_required_string_fails)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option) = "";
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, string_option));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_022: [ If the type is char_ptr then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_023: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_char_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_025: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_NULL_required_string_fails)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option) = NULL;
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, string_option));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_026: [ If the type is wchar_ptr then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_027: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_wchar_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_028: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_029: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_empty_required_wide_string_fails)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(wide_string_option) = L"";
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, wide_string_option));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_026: [ If the type is wchar_ptr then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_027: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_wchar_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_029: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_NULL_required_wide_string_fails)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(wide_string_option) = NULL;
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, wide_string_option));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_030: [ If the type is thandle_rc_string then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_031: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_thandle_rc_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_032: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_033: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_empty_required_thandle_rcstring_fails)
{
    // arrange
    THANDLE_ASSIGN(real_RC_STRING)(&TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_in_thandle).x, g.empty_string);
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, string_option_in_thandle));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_030: [ If the type is thandle_rc_string then: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_031: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_thandle_rc_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_033: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_CREATE_with_NULL_required_thandle_rcstring_fails)
{
    // arrange
    THANDLE_ASSIGN(real_RC_STRING)(&TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_in_thandle).x, NULL);
    TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(my_config)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(my_config, string_option_in_thandle));

    // act
    THANDLE(SF_SERVICE_CONFIG(my_config)) result = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

//
// Dispose
//

/*Tests_SRS_SF_SERVICE_CONFIG_42_035: [ For each config value: ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_036: [ If the type is char_ptr then MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall free the string. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_038: [ If the type is wchar_ptr then MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall free the string. ]*/
    /*Tests_SRS_SF_SERVICE_CONFIG_42_040: [ If the type is thandle_rc_string then MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall assign the THANDLE to NULL. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_042: [ MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall Release the activation_context. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_Dispose_frees_all_strings)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    TEST_SF_SERVICE_CONFIG_EXPECT_DESTROY(my_config)();

    // act
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_042: [ MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall Release the activation_context. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_Dispose_works_when_optional_strings_are_null)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_optional) = "";
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(wide_string_option_optional) = L"";
    THANDLE_ASSIGN(real_RC_STRING)(&TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_in_thandle_optional).x, g.empty_string);

    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    TEST_SF_SERVICE_CONFIG_EXPECT_DESTROY(my_config)();

    // act
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

//
// SF_SERVICE_CONFIG_GETTER
//

/*Tests_SRS_SF_SERVICE_CONFIG_42_043: [ SF_SERVICE_CONFIG_GETTER shall expand to the name of the getter function for the configuration module and the given param by concatenating the name, the string _configuration_get, and the param. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_macro_expands_to_name_of_getter_function)
{
    // arrange

    // act
    const char* name = MU_TOSTRING(SF_SERVICE_CONFIG_GETTER(name, parameter));

    // assert
    ASSERT_ARE_EQUAL(char_ptr, "name_configuration_get_parameter", name);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_044: [ If handle is NULL then SF_SERVICE_CONFIG_GETTER(name, field_name) shall fail and return... ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_045: [ ...false if the type is bool ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_bool_with_NULL_handle_returns_false)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    bool result = SF_SERVICE_CONFIG_GETTER(my_config, some_flag)(NULL);

    // assert
    ASSERT_IS_FALSE(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_044: [ If handle is NULL then SF_SERVICE_CONFIG_GETTER(name, field_name) shall fail and return... ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_046: [ ...UINT32_MAX if the type is uint32_t ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_uint32_t_with_NULL_handle_returns_UINT32_MAX)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    uint32_t result = SF_SERVICE_CONFIG_GETTER(my_config, parameter_3)(NULL);

    // assert
    ASSERT_ARE_EQUAL(uint32_t, UINT32_MAX, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_044: [ If handle is NULL then SF_SERVICE_CONFIG_GETTER(name, field_name) shall fail and return... ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_047: [ ...UINT64_MAX if the type is uint64_t ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_uint64_t_with_NULL_handle_returns_UINT64_MAX)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    uint64_t result = SF_SERVICE_CONFIG_GETTER(my_config, parameter_1)(NULL);

    // assert
    ASSERT_ARE_EQUAL(uint64_t, UINT64_MAX, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_044: [ If handle is NULL then SF_SERVICE_CONFIG_GETTER(name, field_name) shall fail and return... ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_048: [ ...NULL otherwise ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_char_ptr_with_NULL_handle_returns_NULL)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    const char* result = SF_SERVICE_CONFIG_GETTER(my_config, string_option)(NULL);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_044: [ If handle is NULL then SF_SERVICE_CONFIG_GETTER(name, field_name) shall fail and return... ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_048: [ ...NULL otherwise ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_wchar_ptr_with_NULL_handle_returns_NULL)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    const wchar_t* result = SF_SERVICE_CONFIG_GETTER(my_config, wide_string_option)(NULL);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_044: [ If handle is NULL then SF_SERVICE_CONFIG_GETTER(name, field_name) shall fail and return... ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_048: [ ...NULL otherwise ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_thandle_rc_string_with_NULL_handle_returns_NULL)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    STRICT_EXPECTED_CALL(THANDLE_INITIALIZE(RC_STRING)(IGNORED_ARG, NULL));

    // act
    THANDLE(RC_STRING) result = SF_SERVICE_CONFIG_GETTER(my_config, string_option_in_thandle)(NULL);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_003: [ DECLARE_SF_SERVICE_CONFIG_GETTERS shall generate mockable getter functions SF_SERVICE_CONFIG_GETTER(name, param) for each of the configurations provided. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_bool_with_true_value_returns_true)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(some_flag) = true;
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    bool result = SF_SERVICE_CONFIG_GETTER(my_config, some_flag)(config);

    // assert
    ASSERT_IS_TRUE(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_bool_with_false_value_returns_false)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(some_flag) = false;
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    bool result = SF_SERVICE_CONFIG_GETTER(my_config, some_flag)(config);

    // assert
    ASSERT_IS_FALSE(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_uint32_t_returns_value)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(parameter_3) = 12345;
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    uint32_t result = SF_SERVICE_CONFIG_GETTER(my_config, parameter_3)(config);

    // assert
    ASSERT_ARE_EQUAL(uint32_t, 12345, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_uint64_t_returns_value)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(parameter_2) = 0x12345678FFFFFFFF;
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    uint64_t result = SF_SERVICE_CONFIG_GETTER(my_config, parameter_2)(config);

    // assert
    ASSERT_ARE_EQUAL(uint64_t, 0x12345678FFFFFFFF, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_const_char_returns_string)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option) = "parameter value";
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    const char* result = SF_SERVICE_CONFIG_GETTER(my_config, string_option)(config);

    // assert
    ASSERT_ARE_EQUAL(char_ptr, "parameter value", result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_optional_const_char_empty_returns_NULL)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_optional) = "";
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    const char* result = SF_SERVICE_CONFIG_GETTER(my_config, string_option_optional)(config);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_const_wchar_returns_string)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(wide_string_option) = L"parameter value";
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    const wchar_t* result = SF_SERVICE_CONFIG_GETTER(my_config, wide_string_option)(config);

    // assert
    ASSERT_ARE_EQUAL(wchar_ptr, L"parameter value", result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_optional_const_wchar_empty_returns_NULL)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(wide_string_option_optional) = L"";
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    // act
    const wchar_t* result = SF_SERVICE_CONFIG_GETTER(my_config, wide_string_option_optional)(config);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_049: [ If the type is thandle_rc_string then the returned value will be set using THANDLE_INITIALIZE and the caller will have a reference they must free. ]*/
/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_rc_string_returns_string)
{
    // arrange
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    STRICT_EXPECTED_CALL(THANDLE_INITIALIZE(RC_STRING)(IGNORED_ARG, TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_in_thandle).x));

    // act
    THANDLE(RC_STRING) result = SF_SERVICE_CONFIG_GETTER(my_config, string_option_in_thandle)(config);

    // assert
    ASSERT_ARE_EQUAL(TEST_THANDLE_RC_STRING, TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_in_thandle).x, result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(RC_STRING)(&result, NULL);
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

/*Tests_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/
TEST_FUNCTION(SF_SERVICE_CONFIG_GETTER_for_rc_string_with_NULL_returns_NULL)
{
    // arrange
    THANDLE_ASSIGN(real_RC_STRING)(&TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(string_option_in_thandle_optional).x, g.empty_string);
    TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(my_config)();
    THANDLE(SF_SERVICE_CONFIG(my_config)) config = SF_SERVICE_CONFIG_CREATE(my_config)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(my_config));
    ASSERT_IS_NOT_NULL(config);

    STRICT_EXPECTED_CALL(THANDLE_INITIALIZE(RC_STRING)(IGNORED_ARG, NULL));

    // act
    THANDLE(RC_STRING) result = SF_SERVICE_CONFIG_GETTER(my_config, string_option_in_thandle_optional)(config);

    // assert
    ASSERT_IS_NULL(result);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    // cleanup
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_config))(&config, NULL);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
