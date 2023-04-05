// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SF_SERVICE_CONFIG_UT_HELPERS_H
#define SF_SERVICE_CONFIG_UT_HELPERS_H

#include <inttypes.h>
#include <stdbool.h>
#include <wchar.h>

#include "windows.h"
#include "fabricruntime.h"

#include "macro_utils/macro_utils.h"

#include "c_util/rc_string.h"
#include "c_pal/thandle.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/configuration_reader.h"

#include "sf_c_util/sf_service_config.h"

#include "umock_c/umock_c_prod.h"

// Setup the test hooks for configuration_reader to return configured values
#define TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOKS(config_name, ...) \
    /* This expands to a bunch of global variables for the values to return in the configuration reader */ \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__))\
    /* This expands to a bunch of global variables for the default values to return in the configuration reader */ \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__))\
    /* This expands to the configuration reader mock hooks */ \
    TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOK(uint64_t, config_name, __VA_ARGS__) \
    TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOK(uint32_t, config_name, __VA_ARGS__) \
    TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOK(wchar_ptr, config_name, __VA_ARGS__) \
    TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOK(char_ptr, config_name, __VA_ARGS__) \
    TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOK(thandle_rc_string, config_name, __VA_ARGS__) \
    TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOK(bool, config_name, __VA_ARGS__) \
    /* Creates variables "test_sf_service_config_NAME_index" which map the config name to its order in the list of configs (0 based index) */ \
    TEST_SF_SERVICE_CONFIG_DEFINE_INDEX_OF(config_name, __VA_ARGS__) \

// In the test, each value which is returned by the reader hook can be referenced as TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(NAME)
// Example: TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(foo) = 42;

#define TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) MU_C2(sf_service_config_default_, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) MU_C2(sf_service_config_value_to_return_, name)

// Helper to get the index of the config (map the name to the index)
#define TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG(name, field) MU_C3(test_sf_service_config_, name, _index_of)(TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG_MACRO_COUNTER_NAME(field))

// Should be called in test suite setup
#define TEST_SF_SERVICE_CONFIG_HOOK_CONFIGURATION_READER(config_name) \
    REGISTER_GLOBAL_MOCK_HOOK(configuration_reader_get_uint64_t, MU_C3(hook_, config_name, _configuration_reader_get_uint64_t)); \
    REGISTER_GLOBAL_MOCK_HOOK(configuration_reader_get_uint32_t, MU_C3(hook_, config_name, _configuration_reader_get_uint32_t)); \
    REGISTER_GLOBAL_MOCK_HOOK(configuration_reader_get_char_string, MU_C3(hook_, config_name, _configuration_reader_get_char_ptr)); \
    REGISTER_GLOBAL_MOCK_HOOK(configuration_reader_get_thandle_rc_string, MU_C3(hook_, config_name, _configuration_reader_get_thandle_rc_string)); \
    REGISTER_GLOBAL_MOCK_HOOK(configuration_reader_get_wchar_string, MU_C3(hook_, config_name, _configuration_reader_get_wchar_ptr)); \
    REGISTER_GLOBAL_MOCK_HOOK(configuration_reader_get_bool, MU_C3(hook_, config_name, _configuration_reader_get__Bool)); \
    MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name), _vtbl).Release = MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name), _Release); \
    MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name), _vtbl).AddRef = MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name), _AddRef); \
    MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name), _storage).lpVtbl = &MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name), _vtbl); \
    TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name) = &MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(config_name), _storage);

// Should be called in test method setup
#define TEST_SF_SERVICE_CONFIG_RESET(...) \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__))

// Should be called in test method cleanup
#define TEST_SF_SERVICE_CONFIG_CLEANUP(...) \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__))

// Should be called in a test where testing max values is desired
#define TEST_SF_SERVICE_CONFIG_SET_VALUES_TO_MAX(...) \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__))

#define TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name) MU_C2(name, _test_fabric_code_package_activation_context)

// Helper to expect the setup of the config, which should read all params
#define TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(name) MU_C3(expect_, name, _read)

// Helper to expect the setup of the config, which should read all params up to an index then fail
#define TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(name) MU_C3(expect_, name, _read_up_to)

// Helper to expect the cleanup of the config, which should free all the strings
#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_STRINGS(name) MU_C3(expect_, name, _free_strings)

// Helper to expect the destroy of the config
#define TEST_SF_SERVICE_CONFIG_EXPECT_DESTROY(name) MU_C3(expect_, name, _destroy)

// define expectation helpers
#define TEST_SF_SERVICE_CONFIG_DEFINE_EXPECTED_CALL_HELPERS(name, sf_config_name, sf_parameters_section_name, ...) \
    static IFabricCodePackageActivationContextVtbl MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _vtbl); \
    static IFabricCodePackageActivationContext MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _storage); \
    static IFabricCodePackageActivationContext* TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name); \
    MOCK_FUNCTION_WITH_CODE(, ULONG, MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _AddRef), IFabricCodePackageActivationContext*, This) \
    MOCK_FUNCTION_END(0); \
    MOCK_FUNCTION_WITH_CODE(, ULONG, MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _Release), IFabricCodePackageActivationContext*, This) \
    MOCK_FUNCTION_END(0); \
    TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ(name, sf_config_name, sf_parameters_section_name, uint64_t) \
    TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ(name, sf_config_name, sf_parameters_section_name, uint32_t) \
    TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ(name, sf_config_name, sf_parameters_section_name, wchar_ptr) \
    TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ(name, sf_config_name, sf_parameters_section_name, char_ptr) \
    TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ(name, sf_config_name, sf_parameters_section_name, bool) \
    TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ_THANDLE_RC_STRING(name, sf_config_name, sf_parameters_section_name) \
    static void TEST_SF_SERVICE_CONFIG_EXPECT_ALL_READ(name)(void) \
    { \
        STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); \
        STRICT_EXPECTED_CALL(MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _AddRef)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name))) \
            .CallCannotFail(); \
        SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_KEEP_1(TEST_SF_SERVICE_CONFIG_SETUP_EXPECTATION, name, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__)); \
    } \
    static void TEST_SF_SERVICE_CONFIG_EXPECT_READ_UP_TO(name)(uint32_t up_to_index) \
    { \
        STRICT_EXPECTED_CALL(malloc(IGNORED_ARG)); \
        STRICT_EXPECTED_CALL(MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _AddRef)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name))) \
            .CallCannotFail(); \
        /* Counter for the up_to_index check */ \
        uint32_t expectation_counter = 0; \
        SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_KEEP_1(TEST_SF_SERVICE_CONFIG_SETUP_EXPECTATION_IF_LESS, name, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__)); \
        /* Every string that was successful will need to be freed */ \
        expectation_counter = 0; \
        SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(TEST_SF_SERVICE_CONFIG_SETUP_EXPECTATION_FREE_IF_LESS, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__)); \
        STRICT_EXPECTED_CALL(MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _Release)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name))) \
            .CallCannotFail(); \
        STRICT_EXPECTED_CALL(free(IGNORED_ARG)); \
    } \
    static void TEST_SF_SERVICE_CONFIG_EXPECT_FREE_STRINGS(name)(void) \
    { \
        SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(TEST_SF_SERVICE_CONFIG_EXPECT_FREE, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__)); \
    } \
    static void TEST_SF_SERVICE_CONFIG_EXPECT_DESTROY(name)(void) \
    { \
        TEST_SF_SERVICE_CONFIG_EXPECT_FREE_STRINGS(name)(); \
        STRICT_EXPECTED_CALL(MU_C2A(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), _Release)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name))) \
            .CallCannotFail(); \
        STRICT_EXPECTED_CALL(free(IGNORED_ARG)); \
    } \


// The following are internal helpers for the above defines

#define TEST_SF_SERVICE_CONFIG_DEFINE_CONFIGURATION_READER_HOOK(type, config_name, ...) \
    static int MU_C4(hook_, config_name, _configuration_reader_get_, type)(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, type * value) \
    { \
        int result; \
        (void)activation_context; \
        (void)config_package_name; \
        (void)section_name; \
        (void)value; /*maybe not set, e.g. if there are no configs of this type */ \
        if (parameter_name == NULL || parameter_name[0] == L'\0') \
        { \
            result = MU_FAILURE; \
            ASSERT_FAIL("Unexpected missing parameter_name"); \
        } \
        SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(MU_C2(TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH_, type), SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__)) \
        else \
        { \
            result = MU_FAILURE; \
            ASSERT_FAIL("Unknown parameter name %ls (did you forget to update the test?)", parameter_name); \
        } \
        return result; \
    }

#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH(name) \
    else if (wcscmp(parameter_name, SF_SERVICE_CONFIG_PARAMETER_NAME(name)) == 0) \
    { \
        *value = TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name); \
        result = 0; \
    }

#define TEST_SF_SERVICE_CONFIG_DO_COPY_STRING_IF_MATCH(name) \
    else if (wcscmp(parameter_name, SF_SERVICE_CONFIG_PARAMETER_NAME(name)) == 0) \
    { \
        if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) != NULL) \
        { \
            size_t len = strlen(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)); \
            *value = (char*)my_gballoc_malloc(len + 1); \
            ASSERT_IS_NOT_NULL(*value); \
            (void)strcpy(*value, TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)); \
        } \
        result = 0; \
    }

#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_THANDLE_IF_MATCH(name) \
    else if (wcscmp(parameter_name, SF_SERVICE_CONFIG_PARAMETER_NAME(name)) == 0) \
    { \
        THANDLE_ASSIGN(real_RC_STRING)(value, TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name).x); \
        result = 0; \
    }

// Used by the mock to do a lookup to the default or overridden value of the parameter (wide string copies)
#define TEST_SF_SERVICE_CONFIG_DO_COPY_WSTRING_IF_MATCH(name) \
    else if (wcscmp(parameter_name, SF_SERVICE_CONFIG_PARAMETER_NAME(name)) == 0) \
    { \
        if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) != NULL) \
        { \
            size_t len = wcslen(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)); \
            *value = (wchar_t*)my_gballoc_malloc((len + 1) * sizeof(wchar_t)); \
            ASSERT_IS_NOT_NULL(*value); \
            (void)wcscpy(*value, TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)); \
        } \
        result = 0; \
    }

#define TEST_SF_SERVICE_CONFIG_TYPE_IS_bool_bool 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_bool__Bool 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_bool(type) MU_IF(MU_C2(TEST_SF_SERVICE_CONFIG_TYPE_IS_bool_,type), 0, 1)
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH__Bool(type, name) MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_bool(type), TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH(name), )

#define TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT32_uint32_t 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT32(type) MU_IF(MU_C2(TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT32_,type), 0, 1)
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH_uint32_t(type, name) MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT32(type), TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH(name), )

#define TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT64_uint64_t 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT64(type) MU_IF(MU_C2(TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT64_,type), 0, 1)
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH_uint64_t(type, name) MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_UINT64(type), TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH(name), )

#define TEST_SF_SERVICE_CONFIG_TYPE_IS_CHAR_PTR_char_ptr 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_CHAR_PTR(type) MU_IF(MU_C2(TEST_SF_SERVICE_CONFIG_TYPE_IS_CHAR_PTR_,type), 0, 1)
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH_char_ptr(type, name) MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_CHAR_PTR(type), TEST_SF_SERVICE_CONFIG_DO_COPY_STRING_IF_MATCH(name), )

#define TEST_SF_SERVICE_CONFIG_TYPE_IS_WCHAR_PTR_wchar_ptr 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_WCHAR_PTR(type) MU_IF(MU_C2(TEST_SF_SERVICE_CONFIG_TYPE_IS_WCHAR_PTR_,type), 0, 1)
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH_wchar_ptr(type, name) MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_WCHAR_PTR(type), TEST_SF_SERVICE_CONFIG_DO_COPY_WSTRING_IF_MATCH(name), )

#define TEST_SF_SERVICE_CONFIG_TYPE_IS_ANY_CHAR_PTR_char_ptr 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_ANY_CHAR_PTR_wchar_ptr 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_ANY_CHAR_PTR(type) MU_IF(MU_C2(TEST_SF_SERVICE_CONFIG_TYPE_IS_ANY_CHAR_PTR_,type), 0, 1)

#define TEST_SF_SERVICE_CONFIG_TYPE_IS_THANDLE_thandle_rc_string 0
#define TEST_SF_SERVICE_CONFIG_TYPE_IS_THANDLE(type) MU_IF(MU_C2A(TEST_SF_SERVICE_CONFIG_TYPE_IS_THANDLE_,type), 0, 1)
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGN_IF_MATCH_thandle_rc_string(type, name) MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_THANDLE(type), TEST_SF_SERVICE_CONFIG_DO_ASSIGN_THANDLE_IF_MATCH(name), )

#define TEST_SF_SERVICE_CONFIG_THANDLE_MEMBER_DECL(type, name) type name

#define TEST_SF_SERVICE_CONFIG_THANDLE_STATIC(type, name, member) \
    static struct MU_C2B(name, _TAG) { \
        TEST_SF_SERVICE_CONFIG_THANDLE_MEMBER_DECL(type, member); \
    } name;

#define TEST_SF_SERVICE_CONFIG_TYPE_FOR_ASSERT_uint32_t uint32_t
#define TEST_SF_SERVICE_CONFIG_TYPE_FOR_ASSERT_uint64_t uint64_t
#define TEST_SF_SERVICE_CONFIG_TYPE_FOR_ASSERT_char_ptr char_ptr
#define TEST_SF_SERVICE_CONFIG_TYPE_FOR_ASSERT_wchar_ptr wchar_ptr
#define TEST_SF_SERVICE_CONFIG_TYPE_FOR_ASSERT_thandle_rc_string TEST_THANDLE_RC_STRING

#define TEST_SF_SERVICE_CONFIG_TYPE_FOR_ASSERT(type) MU_C2(TEST_SF_SERVICE_CONFIG_TYPE_FOR_ASSERT_, type)

#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_DEFAULT(type, name) static type TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name);
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_THANDLE(type, name) TEST_SF_SERVICE_CONFIG_THANDLE_STATIC(type, TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name), x)

#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE__Bool TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_DEFAULT
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_bool TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_DEFAULT
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_uint32_t TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_DEFAULT
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_uint64_t TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_DEFAULT
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_char_ptr TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_DEFAULT
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_wchar_ptr TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_DEFAULT
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_thandle_rc_string TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_THANDLE

#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE(type, name) MU_C2A(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_DEFINE_, type)(type, name)

#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE__Bool(type, name) static const bool TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) = true;
#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE_bool(type, name) static const bool TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) = true;
#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE_uint32_t(type, name) static const uint32_t TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) = 42;
#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE_uint64_t(type, name) static const uint64_t TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) = 4242;
#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE_char_ptr(type, name) static const char* TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) = MU_TOSTRING(name);
#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE_wchar_ptr(type, name) static const wchar_t* TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) = MU_C2(L, MU_TOSTRING(name));
#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE_thandle_rc_string(type, name) static const char* TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name) = MU_TOSTRING(name);

#define TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE(type, name) MU_C2A(TEST_SF_SERVICE_CONFIG_DEFAULT_DEFINE_, type)(type, name)

#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_DEFAULT(lval, rval) lval = rval;
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_THANDLE(lval, rval) THANDLE_ASSIGN(real_RC_STRING)(&lval, rval);

#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT__Bool TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_DEFAULT
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_uint32_t TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_DEFAULT
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_uint64_t TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_DEFAULT
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_char_ptr TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_DEFAULT
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_wchar_ptr TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_DEFAULT
#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_thandle_rc_string TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_THANDLE

#define TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT(type, lval, rval) MU_C2(TEST_SF_SERVICE_CONFIG_DO_ASSIGNMENT_, type)(lval, rval)

#define TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG_MACRO_COUNTER_NAME(name) MU_C3(test_sf_service_config_, name, _index)
#define TEST_SF_SERVICE_CONFIG_DEFINE_CONFIG_INDEX_VALUE(counter, type, name) static uint32_t TEST_SF_SERVICE_CONFIG_INDEX_OF_CONFIG_MACRO_COUNTER_NAME(name) = counter;

#define TEST_SF_SERVICE_CONFIG_DEFINE_INDEX_OF(name, ...) \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_COUNTED(TEST_SF_SERVICE_CONFIG_DEFINE_CONFIG_INDEX_VALUE, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__)) \
    static uint32_t MU_C3(test_sf_service_config_, name, _index_of)(uint32_t macro_counter_value) \
    { \
        return (MU_COUNT_ARG(__VA_ARGS__)) - macro_counter_value / 2; \
    }

// Causes all mock calls to return the defaults (defined above)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT_bool(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name);
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT__Bool(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name);
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT_uint32_t(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name);
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT_uint64_t(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name);
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT_char_ptr(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = (char*)TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name);
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT_wchar_ptr(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = (wchar_t*)TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name);
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT_thandle_rc_string(name) \
    THANDLE(RC_STRING) MU_C2(name, _temp) = real_rc_string_create(TEST_SF_SERVICE_CONFIG_DEFAULT_VALUE(name)); \
    ASSERT_IS_NOT_NULL(MU_C2(name, _temp)); \
    THANDLE_MOVE(real_RC_STRING)(&TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name).x, &MU_C2(name, _temp));

#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(type, name) MU_C2(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT_, type)(name)

#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX_bool(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = false;
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX__Bool(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = false;
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX_uint32_t(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = UINT32_MAX - 1;
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX_uint64_t(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) = UINT64_MAX - 1;
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX_char_ptr(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(char_ptr, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX_wchar_ptr(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(wchar_ptr, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX_thandle_rc_string(name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(thandle_rc_string, name)

// Causes all numerical values to report the maximum (but leaves strings default)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX(type, name) MU_C2(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_MAX_, type)(name)

// Cleanup defaults
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP_bool(type, name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(type, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP__Bool(type, name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(type, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP_uint32_t(type, name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(type, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP_uint64_t(type, name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(type, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP_char_ptr(type, name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(type, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP_wchar_ptr(type, name) TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_INIT_DEFAULT(type, name)
#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP_thandle_rc_string(type, name) \
    THANDLE_ASSIGN(real_RC_STRING)(&TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name).x, NULL);

#define TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP(type, name) MU_C2(TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN_CLEANUP_, type)(type, name)



#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_IF_EMPTY_STRING(name) \
    if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) != NULL && TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)[0] == '\0') \
    { \
        STRICT_EXPECTED_CALL(free(IGNORED_ARG)); \
    }

#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_IF_EMPTY_WSTRING(name) \
    if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) != NULL && TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)[0] == L'\0') \
    { \
        STRICT_EXPECTED_CALL(free(IGNORED_ARG)); \
    }

#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_IF_EMPTY_THANDLE_RC_STRING(name) \
    if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name).x != NULL && TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name).x->string[0] == '\0') \
    { \
        STRICT_EXPECTED_CALL(THANDLE_ASSIGN(RC_STRING)(IGNORED_ARG, NULL)); \
    }

#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_bool(name)
#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE__Bool(name)
#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_uint32_t(name)
#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_uint64_t(name)
#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_char_ptr(name) \
    if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) != NULL && TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)[0] != '\0') \
    { \
        STRICT_EXPECTED_CALL(free(IGNORED_ARG)); \
    }
#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_wchar_ptr(name) \
    if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name) != NULL && TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name)[0] != L'\0') \
    { \
        STRICT_EXPECTED_CALL(free(IGNORED_ARG)); \
    }
#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE_thandle_rc_string(name) \
    if (TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name).x != NULL && TEST_SF_SERVICE_CONFIG_VALUE_TO_RETURN(name).x->string[0] != '\0') \
    { \
        STRICT_EXPECTED_CALL(THANDLE_ASSIGN(RC_STRING)(IGNORED_ARG, NULL)); \
    }

#define TEST_SF_SERVICE_CONFIG_EXPECT_FREE(type, name) MU_C2(TEST_SF_SERVICE_CONFIG_EXPECT_FREE_, type)(name)

#define TEST_SF_SERVICE_CONFIG_EXPECT_INIT_THANDLE(name) STRICT_EXPECTED_CALL(THANDLE_INITIALIZE(RC_STRING)(IGNORED_ARG, NULL));

#define TEST_SF_SERVICE_CONFIG_EXPECT_INIT_ON_FAILURE(type, name) \
    MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_THANDLE(type), TEST_SF_SERVICE_CONFIG_EXPECT_INIT_THANDLE(name), )

#define TEST_SF_SERVICE_CONFIG_SETUP_EXPECTATION(name, field_type, field_name) \
    MU_C3(name, _expect_read_, field_type)(SF_SERVICE_CONFIG_PARAMETER_NAME(field_name)); \
    MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_CHAR_PTR(field_type), TEST_SF_SERVICE_CONFIG_EXPECT_FREE_IF_EMPTY_STRING(field_name), ) \
    MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_WCHAR_PTR(field_type), TEST_SF_SERVICE_CONFIG_EXPECT_FREE_IF_EMPTY_WSTRING(field_name), ) \
    MU_IF(TEST_SF_SERVICE_CONFIG_TYPE_IS_THANDLE(field_type), TEST_SF_SERVICE_CONFIG_EXPECT_FREE_IF_EMPTY_THANDLE_RC_STRING(field_name), )


#define TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION_uint32_t configuration_reader_get_uint32_t
#define TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION_uint64_t configuration_reader_get_uint64_t
#define TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION__Bool configuration_reader_get_bool
#define TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION_char_ptr configuration_reader_get_char_string
#define TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION_wchar_ptr configuration_reader_get_wchar_string
#define TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION_thandle_rc_string configuration_reader_get_thandle_rc_string

#define TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION(type) MU_C2(TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION_, type)

#define TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ(name, sf_config_name, sf_parameters_section_name, type) \
    static void MU_C3(name, _expect_read_, type)(const wchar_t* parameter) \
    { \
        STRICT_EXPECTED_CALL(TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION(type)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), sf_config_name, sf_parameters_section_name, parameter, IGNORED_ARG)); \
    }

#define TEST_SF_SERVICE_CONFIG_DEFINE_EXPECT_READ_THANDLE_RC_STRING(name, sf_config_name, sf_parameters_section_name) \
    static void MU_C2(name, _expect_read_thandle_rc_string)(const wchar_t* parameter) \
    { \
        STRICT_EXPECTED_CALL(THANDLE_INITIALIZE(RC_STRING)(IGNORED_ARG, NULL)); \
        STRICT_EXPECTED_CALL(TEST_SF_SERVICE_CONFIG_CONFIGURATION_READER_GET_FUNCTION(thandle_rc_string)(TEST_SF_SERVICE_CONFIG_ACTIVATION_CONTEXT(name), sf_config_name, sf_parameters_section_name, parameter, IGNORED_ARG)); \
    }

#define TEST_SF_SERVICE_CONFIG_SETUP_EXPECTATION_IF_LESS(name, field_type, field_name) \
    if (expectation_counter <= up_to_index) \
    { \
        TEST_SF_SERVICE_CONFIG_SETUP_EXPECTATION(name, field_type, field_name) \
    } \
    else \
    { \
        TEST_SF_SERVICE_CONFIG_EXPECT_INIT_ON_FAILURE(field_type, field_name); \
    } \
    expectation_counter++;

#define TEST_SF_SERVICE_CONFIG_SETUP_EXPECTATION_FREE_IF_LESS(type, name) \
    if (expectation_counter <= up_to_index) \
    { \
        TEST_SF_SERVICE_CONFIG_EXPECT_FREE(type, name) \
    } \
    expectation_counter++;

#endif /* SF_SERVICE_CONFIG_UT_HELPERS_H */