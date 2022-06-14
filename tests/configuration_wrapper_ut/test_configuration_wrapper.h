// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_CONFIGURATION_WRAPPER_H
#define TEST_CONFIGURATION_WRAPPER_H

#ifdef __cplusplus
#include <cinttypes>
#else
#include <inttypes.h>
#include <stdbool.h>
#include <wchar.h>
#endif

#include "sf_c_util/configuration_wrapper.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CONFIGURATION_WRAPPER_PARAMETER_NAME_parameter_1 L"Parameter1"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_parameter_2 L"Parameter2"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_parameter_3 L"Parameter3WithLongerName"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_some_flag L"SomeFlag"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_string_option_in_thandle L"StringOptionThandle"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_string_option L"MyString"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_wide_string_option L"MyWideString"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_string_option_in_thandle_optional L"OptionalStringOptionThandle"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_string_option_optional L"OptionalStringOption"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_wide_string_option_optional L"OptionalWideStringOption"
#define CONFIGURATION_WRAPPER_PARAMETER_NAME_another_flag L"AnotherFlag"

#define MY_CONFIG_TEST_PARAMS \
    CONFIG_REQUIRED(uint64_t, parameter_1), \
    CONFIG_REQUIRED(uint64_t, parameter_2), \
    CONFIG_REQUIRED(uint32_t, parameter_3), \
    CONFIG_REQUIRED(bool, some_flag), \
    CONFIG_REQUIRED(thandle_rc_string, string_option_in_thandle), \
    CONFIG_REQUIRED(char_ptr, string_option), \
    CONFIG_REQUIRED(wchar_ptr, wide_string_option), \
    CONFIG_OPTIONAL(thandle_rc_string, string_option_in_thandle_optional), \
    CONFIG_OPTIONAL(char_ptr, string_option_optional), \
    CONFIG_OPTIONAL(wchar_ptr, wide_string_option_optional), \
    CONFIG_REQUIRED(bool, another_flag) \

//DECLARE_CONFIGURATION_WRAPPER(my_config, MY_CONFIG_TEST_PARAMS)

    typedef struct my_config_CONFIGURATION_TAG my_config_CONFIGURATION;
    typedef const my_config_CONFIGURATION
        * const volatile CONST_P2_CONST_my_config_CONFIGURATION;
    ;
    void my_config_CONFIGURATION_ASSIGN(CONST_P2_CONST_my_config_CONFIGURATION* t1,
        CONST_P2_CONST_my_config_CONFIGURATION t2);
    ;
    void my_config_CONFIGURATION_INITIALIZE(
        CONST_P2_CONST_my_config_CONFIGURATION* t1,
        CONST_P2_CONST_my_config_CONFIGURATION t2);
    ;
    void my_config_CONFIGURATION_MOVE(CONST_P2_CONST_my_config_CONFIGURATION* t1,
        CONST_P2_CONST_my_config_CONFIGURATION* t2);
    ;
    void my_config_CONFIGURATION_INITIALIZE_MOVE(
        CONST_P2_CONST_my_config_CONFIGURATION* t1,
        CONST_P2_CONST_my_config_CONFIGURATION* t2);
    ;
    ;
    CONST_P2_CONST_my_config_CONFIGURATION my_config_configuration_create(
        IFabricCodePackageActivationContext* activation_context);
    ;
    uint64_t my_config_configuration_get_parameter_1(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    uint64_t my_config_configuration_get_parameter_2(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    uint32_t my_config_configuration_get_parameter_3(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    _Bool my_config_configuration_get_some_flag(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    CONST_P2_CONST_RC_STRING my_config_configuration_get_string_option_in_thandle(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    const char* my_config_configuration_get_string_option(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    const wchar_t* my_config_configuration_get_wide_string_option(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    CONST_P2_CONST_RC_STRING
        my_config_configuration_get_string_option_in_thandle_optional(
            CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    const char* my_config_configuration_get_string_option_optional(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    const wchar_t* my_config_configuration_get_wide_string_option_optional(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;
    _Bool my_config_configuration_get_another_flag(
        CONST_P2_CONST_my_config_CONFIGURATION handle);
    ;


#ifdef __cplusplus
}
#endif

#endif /* TEST_CONFIGURATION_WRAPPER_H */