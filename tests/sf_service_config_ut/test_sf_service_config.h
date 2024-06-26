// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_CONFIGURATION_WRAPPER_H
#define TEST_CONFIGURATION_WRAPPER_H

#include <float.h>
#include <inttypes.h>
#include <stdbool.h>
#include <wchar.h>

#include "sf_c_util/sf_service_config.h"

#include "umock_c/umock_c_prod.h"

#define SF_SERVICE_CONFIG_PARAMETER_NAME_parameter_1 L"Parameter1"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_parameter_2 L"Parameter2"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_parameter_3 L"Parameter3WithLongerName"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_parameter_4 L"Parameter4uint8_t"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_parameter_5 L"Parameter5double"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_some_flag L"SomeFlag"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_string_option_in_thandle L"StringOptionThandle"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_string_option L"MyString"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_wide_string_option L"MyWideString"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_string_option_in_thandle_optional L"OptionalStringOptionThandle"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_string_option_optional L"OptionalStringOption"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_wide_string_option_optional L"OptionalWideStringOption"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_another_flag L"AnotherFlag"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_a_secret L"ASecret"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_a_secret_uint8_t L"ASecretUint8t"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_a_secret_char_ptr L"ASecretCharPtr"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_a_secret_double L"ASecretDouble"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_a_secret_bool L"ASecretBool"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_another_secret L"AnotherSecret"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_another_secret_uint8_t L"AnotherSecretUint8t"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_another_secret_char_ptr L"AnotherSecretCharPtr"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_another_secret_double L"AnotherSecretDouble"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_another_secret_bool L"AnotherSecretBool"

#define MY_CONFIG_TEST_PARAMS \
    CONFIG_REQUIRED(uint64_t, parameter_1), \
    CONFIG_REQUIRED(uint64_t, parameter_2), \
    CONFIG_REQUIRED(uint32_t, parameter_3), \
    CONFIG_REQUIRED(uint8_t, parameter_4), \
    CONFIG_REQUIRED(double, parameter_5), \
    CONFIG_REQUIRED(bool, some_flag), \
    CONFIG_REQUIRED(thandle_rc_string, string_option_in_thandle), \
    CONFIG_REQUIRED(char_ptr, string_option), \
    CONFIG_REQUIRED(wchar_ptr, wide_string_option), \
    CONFIG_OPTIONAL(thandle_rc_string, string_option_in_thandle_optional), \
    CONFIG_OPTIONAL(char_ptr, string_option_optional), \
    CONFIG_OPTIONAL(wchar_ptr, wide_string_option_optional), \
    CONFIG_REQUIRED(bool, another_flag), \
    CONFIG_REQUIRED_NO_LOGGING(thandle_rc_string, a_secret), \
    CONFIG_REQUIRED_NO_LOGGING(uint8_t, a_secret_uint8_t), \
    CONFIG_REQUIRED_NO_LOGGING(char_ptr, a_secret_char_ptr), \
    CONFIG_REQUIRED_NO_LOGGING(double, a_secret_double), \
    CONFIG_REQUIRED_NO_LOGGING(bool, a_secret_bool), \
    CONFIG_OPTIONAL_NO_LOGGING(thandle_rc_string, another_secret), \
    CONFIG_OPTIONAL_NO_LOGGING(uint8_t, another_secret_uint8_t), \
    CONFIG_OPTIONAL_NO_LOGGING(char_ptr, another_secret_char_ptr), \
    CONFIG_OPTIONAL_NO_LOGGING(double, another_secret_double), \
    CONFIG_OPTIONAL_NO_LOGGING(bool, another_secret_bool) \

DECLARE_SF_SERVICE_CONFIG(my_config, MY_CONFIG_TEST_PARAMS)

#endif /* TEST_CONFIGURATION_WRAPPER_H */