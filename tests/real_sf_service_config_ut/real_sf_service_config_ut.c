// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "real_sf_service_config_ut_pch.h"

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

#define TEST_BOOL_VALUE_STRING "True"
#define TEST_DOUBLE_VALUE 2.5
#define TEST_UINT8_T_VALUE 42
#define TEST_UINT32_T_VALUE 4242
#define TEST_UINT64_T_VALUE 424242424242
#define TEST_CHAR_PTR_VALUE "a char string"
#define TEST_WCHAR_PTR_VALUE "a wchar string"
#define TEST_THANDLE_RC_STRING_VALUE "an rc string"

static char* test_argv[] =
{
    CONFIGURATION_PACKAGE_NAME, SAMPLE_SERVICE_CONFIG_PACKAGE_NAME,
    SECTION_NAME_DEFINE, SAMPLE_SERVICE_SECTION_NAME,
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_bool_value, TEST_BOOL_VALUE_STRING,
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_double_value, MU_TOSTRING(TEST_DOUBLE_VALUE),
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint8_t_value, MU_TOSTRING(TEST_UINT8_T_VALUE),
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint32_t_value, MU_TOSTRING(TEST_UINT32_T_VALUE),
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint64_t_value, MU_TOSTRING(TEST_UINT64_T_VALUE),
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_char_ptr_value, TEST_CHAR_PTR_VALUE,
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_wchar_ptr_value, TEST_WCHAR_PTR_VALUE,
    SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_thandle_rc_string_value, TEST_THANDLE_RC_STRING_VALUE
};

#define TEST_ARGC ((int)MU_COUNT_ARRAY_ITEMS(test_argv))

static IFabricCodePackageActivationContext* create_real_activation_context(void)
{
    IFabricCodePackageActivationContext* result;
    int argc_consumed;
    FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context = real_fc_activation_context_create(TEST_ARGC, test_argv, &argc_consumed);
    if (fc_activation_context == NULL)
    {
        LogError("failure in real_fc_activation_context_create(TEST_ARGC=%d, test_argv=%p, &argc_consumed)", TEST_ARGC, test_argv);
        result = NULL;
    }
    else
    {
        result = COM_WRAPPER_CREATE(real_FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, fc_activation_context, real_fc_activation_context_destroy);
        if (result == NULL)
        {
            LogError("failure in COM_WRAPPER_CREATE(real_FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, ...)");
            real_fc_activation_context_destroy(fc_activation_context);
        }
        else
        {
            /* return as is */
        }
    }
    return result;
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error));
    ASSERT_ARE_EQUAL(int, 0, umocktypes_bool_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_windows_register_types());

    REGISTER_UMOCK_ALIAS_TYPE(FC_ACTIVATION_CONTEXT_HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(FC_PACKAGE_HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(FABRIC_STRING_LIST_RESULT_HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(THANDLE(RC_STRING), void*);
    REGISTER_UMOCK_ALIAS_TYPE(THANDLE(SF_SERVICE_CONFIG(sample_service)), void*);

    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();
    REGISTER_INTERLOCKED_GLOBAL_MOCK_HOOK();
    REGISTER_SRW_LOCK_LL_GLOBAL_MOCK_HOOK();
    REGISTER_RC_STRING_GLOBAL_MOCK_HOOKS();
    REGISTER_FABRIC_CONFIGURATION_PACKAGE_GLOBAL_MOCK_HOOK();
    REGISTER_FABRIC_STRING_LIST_RESULT_GLOBAL_MOCK_HOOK();
    REGISTER_FABRIC_CONFIGURATION_ACTIVATION_CONTEXT_GLOBAL_MOCK_HOOK();
    REGISTER_CONFIGURATION_READER_GLOBAL_MOCK_HOOK();
    REGISTER_SF_SERVICE_CONFIG_GLOBAL_MOCK_HOOK(sample_service, SAMPLE_SERVICE_PARAMS);
    REGISTER_GLOBAL_MOCK_HOOK(sample_config_consumer_create, real_sample_config_consumer_create);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    umock_c_reset_all_calls();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
}

TEST_FUNCTION(sf_service_config_created_from_argv_reads_every_type_with_reals) // no-srs
{
    ///arrange
    int argc_consumed;
    STRICT_EXPECTED_CALL(fc_activation_context_create(TEST_ARGC, test_argv, IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_CREATE(sample_service)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, bool_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, double_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, uint8_t_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, uint32_t_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, uint64_t_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, char_ptr_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, wchar_ptr_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, thandle_rc_string_value)(IGNORED_ARG));

    ///act
    FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context = fc_activation_context_create(TEST_ARGC, test_argv, &argc_consumed);
    IFabricCodePackageActivationContext* activation_context = COM_WRAPPER_CREATE(real_FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, fc_activation_context, real_fc_activation_context_destroy);
    THANDLE(SF_SERVICE_CONFIG(sample_service)) config = SF_SERVICE_CONFIG_CREATE(sample_service)(activation_context);
    bool bool_value = SF_SERVICE_CONFIG_GETTER(sample_service, bool_value)(config);
    double double_value = SF_SERVICE_CONFIG_GETTER(sample_service, double_value)(config);
    uint8_t uint8_t_value = SF_SERVICE_CONFIG_GETTER(sample_service, uint8_t_value)(config);
    uint32_t uint32_t_value = SF_SERVICE_CONFIG_GETTER(sample_service, uint32_t_value)(config);
    uint64_t uint64_t_value = SF_SERVICE_CONFIG_GETTER(sample_service, uint64_t_value)(config);
    const char* char_ptr_value = SF_SERVICE_CONFIG_GETTER(sample_service, char_ptr_value)(config);
    const wchar_t* wchar_ptr_value = SF_SERVICE_CONFIG_GETTER(sample_service, wchar_ptr_value)(config);
    THANDLE(RC_STRING) thandle_rc_string_value = SF_SERVICE_CONFIG_GETTER(sample_service, thandle_rc_string_value)(config);

    ///assert
    ASSERT_ARE_EQUAL(int, TEST_ARGC, argc_consumed);
    ASSERT_IS_NOT_NULL(activation_context);
    ASSERT_IS_NOT_NULL(config);
    ASSERT_IS_TRUE(bool_value);
    ASSERT_ARE_EQUAL(double, TEST_DOUBLE_VALUE, double_value);
    ASSERT_ARE_EQUAL(uint8_t, TEST_UINT8_T_VALUE, uint8_t_value);
    ASSERT_ARE_EQUAL(uint32_t, TEST_UINT32_T_VALUE, uint32_t_value);
    ASSERT_ARE_EQUAL(uint64_t, TEST_UINT64_T_VALUE, uint64_t_value);
    ASSERT_ARE_EQUAL(char_ptr, TEST_CHAR_PTR_VALUE, char_ptr_value);
    ASSERT_ARE_EQUAL(wchar_ptr, MU_C2(L, TEST_WCHAR_PTR_VALUE), wchar_ptr_value);
    ASSERT_IS_NOT_NULL(thandle_rc_string_value);
    ASSERT_ARE_EQUAL(char_ptr, TEST_THANDLE_RC_STRING_VALUE, thandle_rc_string_value->string);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    THANDLE_ASSIGN(RC_STRING)(&thandle_rc_string_value, NULL);
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(sample_service))(&config, NULL);
    (void)activation_context->lpVtbl->Release(activation_context);
}

TEST_FUNCTION(sf_service_config_created_from_argv_is_released_with_reals) // no-srs
{
    ///arrange
    IFabricCodePackageActivationContext* activation_context = create_real_activation_context();
    ASSERT_IS_NOT_NULL(activation_context);
    THANDLE(SF_SERVICE_CONFIG(sample_service)) config = real_sample_service_configuration_create(activation_context);
    ASSERT_IS_NOT_NULL(config);

    STRICT_EXPECTED_CALL(THANDLE_ASSIGN(SF_SERVICE_CONFIG(sample_service))(IGNORED_ARG, NULL));

    ///act
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(sample_service))(&config, NULL);
    (void)activation_context->lpVtbl->Release(activation_context);

    ///assert
    ASSERT_IS_NULL(config);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

TEST_FUNCTION(sf_service_config_created_from_argv_is_refreshed_with_reals) // no-srs
{
    ///arrange
    IFabricCodePackageActivationContext* activation_context = create_real_activation_context();
    ASSERT_IS_NOT_NULL(activation_context);
    THANDLE(SF_SERVICE_CONFIG(sample_service)) config = real_sample_service_configuration_create(activation_context);
    ASSERT_IS_NOT_NULL(config);

    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_REFRESH(sample_service)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, uint64_t_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, char_ptr_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, thandle_rc_string_value)(IGNORED_ARG));

    ///act
    int result = SF_SERVICE_CONFIG_REFRESH(sample_service)(config);
    uint64_t uint64_t_value = SF_SERVICE_CONFIG_GETTER(sample_service, uint64_t_value)(config);
    const char* char_ptr_value = SF_SERVICE_CONFIG_GETTER(sample_service, char_ptr_value)(config);
    THANDLE(RC_STRING) thandle_rc_string_value = SF_SERVICE_CONFIG_GETTER(sample_service, thandle_rc_string_value)(config);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(uint64_t, TEST_UINT64_T_VALUE, uint64_t_value);
    ASSERT_ARE_EQUAL(char_ptr, TEST_CHAR_PTR_VALUE, char_ptr_value);
    ASSERT_IS_NOT_NULL(thandle_rc_string_value);
    ASSERT_ARE_EQUAL(char_ptr, TEST_THANDLE_RC_STRING_VALUE, thandle_rc_string_value->string);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    THANDLE_ASSIGN(RC_STRING)(&thandle_rc_string_value, NULL);
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(sample_service))(&config, NULL);
    (void)activation_context->lpVtbl->Release(activation_context);
}

TEST_FUNCTION(sf_service_config_created_by_a_real_consumer_is_read_and_released_with_reals) // no-srs
{
    ///arrange
    STRICT_EXPECTED_CALL(sample_config_consumer_create(TEST_ARGC, test_argv));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, uint32_t_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(SF_SERVICE_CONFIG_GETTER(sample_service, uint64_t_value)(IGNORED_ARG));
    STRICT_EXPECTED_CALL(THANDLE_ASSIGN(SF_SERVICE_CONFIG(sample_service))(IGNORED_ARG, NULL));

    ///act
    THANDLE(SF_SERVICE_CONFIG(sample_service)) config = sample_config_consumer_create(TEST_ARGC, test_argv);
    uint32_t uint32_t_value = SF_SERVICE_CONFIG_GETTER(sample_service, uint32_t_value)(config);
    uint64_t uint64_t_value = SF_SERVICE_CONFIG_GETTER(sample_service, uint64_t_value)(config);
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(sample_service))(&config, NULL);

    ///assert
    ASSERT_ARE_EQUAL(uint32_t, TEST_UINT32_T_VALUE, uint32_t_value);
    ASSERT_ARE_EQUAL(uint64_t, TEST_UINT64_T_VALUE, uint64_t_value);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
