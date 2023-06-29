// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdbool.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "com_wrapper/com_wrapper.h"

#include "service_config.h"

#include "sf_c_util/common_argc_argv.h"
#include "sf_c_util/sf_service_config.h"
#include "sf_c_util/fc_activation_context_com.h"
#include "sf_c_util/fc_activation_context.h"

TEST_DEFINE_ENUM_TYPE(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
}

TEST_FUNCTION_INITIALIZE(method_init)
{
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
}

TEST_FUNCTION(sf_config_can_parse_all_optional_configs_present)
{
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        SF_CONFIG_NAME_DEFINE,
        SECTION_NAME_DEFINE,
        SF_SECTION_NAME_DEFINE,

        /*followed by a list of parameters...*/
        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_1,
        "1",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_2,
        "2",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_3,
        "3",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_some_flag, 
        "true",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle, 
        "awesome",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option, 
        "simpleString",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option, 
        "WAAAAAAAAAAAAAAA",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle_optional,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_optional, 
        "snowflake",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option_optional, 
        "vincent",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_another_flag,
        "false"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int argc_consumed;
    FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_activation_context);
    ASSERT_ARE_EQUAL(int, argc, argc_consumed);
    IFabricCodePackageActivationContext* activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, fc_activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(activation_context);

    ///act
    THANDLE(SF_SERVICE_CONFIG(my_mocked_config)) config = SF_SERVICE_CONFIG_CREATE(my_mocked_config)(activation_context);

    ///assert (very e2e-ish)
    ASSERT_IS_NOT_NULL(config);
    ASSERT_ARE_EQUAL(uint64_t, 1, SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_1)(config));
    ASSERT_ARE_EQUAL(uint64_t, 2, SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_2)(config));
    ASSERT_ARE_EQUAL(uint64_t, 3, SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_3)(config));
    ASSERT_IS_TRUE(SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_some_flag)(config));

    {
        THANDLE(RC_STRING) temp = SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option_in_thandle)(config);
        ASSERT_IS_NOT_NULL(temp);
        ASSERT_ARE_EQUAL(char_ptr, "awesome", temp->string);
        THANDLE_ASSIGN(RC_STRING)(&temp, NULL);
    }

    ASSERT_ARE_EQUAL(char_ptr, "simpleString", SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option)(config));
    ASSERT_ARE_EQUAL(wchar_ptr, L"WAAAAAAAAAAAAAAA", SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_wide_string_option)(config));
    
    {
        THANDLE(RC_STRING) temp = SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option_in_thandle_optional)(config);
        ASSERT_IS_NOT_NULL(temp);
        ASSERT_ARE_EQUAL(char_ptr, "zuzu", temp->string);
        THANDLE_ASSIGN(RC_STRING)(&temp, NULL);
    }

    ASSERT_ARE_EQUAL(char_ptr, "snowflake", SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option_optional)(config));
    ASSERT_ARE_EQUAL(char_ptr, L"vincent", SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_wide_string_option_optional)(config));
    
    ASSERT_IS_FALSE(SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_another_flag)(config));

    ///clean
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_mocked_config))(&config, NULL);
    activation_context->lpVtbl->Release(activation_context);
}

TEST_FUNCTION(sf_config_can_parse_no_optional_configs_present)
{
    char* argv[] =
    {
        CONFIGURATION_PACKAGE_NAME,
        SF_CONFIG_NAME_DEFINE,
        SECTION_NAME_DEFINE,
        SF_SECTION_NAME_DEFINE,

        /*followed by a list of parameters...*/
        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_1,
        "1",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_2,
        "2",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_3,
        "3",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_some_flag,
        "true",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle,
        "awesome",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option,
        "simpleString",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option,
        "WAAAAAAAAAAAAAAA",

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle_optional,
        "", /*used to be zuzu in the previous test*/

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_optional,
        "", /*used to be snowflake in the previous test*/

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option_optional,
        "", /*used to be vincent in the previous test*/

        SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_another_flag,
        "false"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int argc_consumed;
    FC_ACTIVATION_CONTEXT_HANDLE fc_activation_context = fc_activation_context_create(argc, argv, &argc_consumed);
    ASSERT_IS_NOT_NULL(fc_activation_context);
    ASSERT_ARE_EQUAL(int, argc, argc_consumed);
    IFabricCodePackageActivationContext* activation_context = COM_WRAPPER_CREATE(FC_ACTIVATION_CONTEXT_HANDLE, IFabricCodePackageActivationContext, fc_activation_context, fc_activation_context_destroy);
    ASSERT_IS_NOT_NULL(activation_context);

    ///act
    THANDLE(SF_SERVICE_CONFIG(my_mocked_config)) config = SF_SERVICE_CONFIG_CREATE(my_mocked_config)(activation_context);

    ///assert (very e2e-ish)
    ASSERT_IS_NOT_NULL(config);
    ASSERT_ARE_EQUAL(uint64_t, 1, SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_1)(config));
    ASSERT_ARE_EQUAL(uint64_t, 2, SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_2)(config));
    ASSERT_ARE_EQUAL(uint64_t, 3, SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_parameter_3)(config));
    ASSERT_IS_TRUE(SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_some_flag)(config));

    {
        THANDLE(RC_STRING) temp = SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option_in_thandle)(config);
        ASSERT_IS_NOT_NULL(temp);
        ASSERT_ARE_EQUAL(char_ptr, "awesome", temp->string);
        THANDLE_ASSIGN(RC_STRING)(&temp, NULL);
    }

    ASSERT_ARE_EQUAL(char_ptr, "simpleString", SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option)(config));
    ASSERT_ARE_EQUAL(wchar_ptr, L"WAAAAAAAAAAAAAAA", SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_wide_string_option)(config));

    {
        THANDLE(RC_STRING) temp = SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option_in_thandle_optional)(config);
        ASSERT_IS_NULL(temp); /*optional without a value*/
    }

    ASSERT_IS_NULL(SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_string_option_optional)(config));
    ASSERT_IS_NULL(SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_wide_string_option_optional)(config));

    ASSERT_IS_FALSE(SF_SERVICE_CONFIG_GETTER(my_mocked_config, mocked_another_flag)(config));

    ///clean
    THANDLE_ASSIGN(SF_SERVICE_CONFIG(my_mocked_config))(&config, NULL);
    activation_context->lpVtbl->Release(activation_context);
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
