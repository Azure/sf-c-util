// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SERVICE_CONFIG_H
#define SERVICE_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "sf_c_util/sf_service_config.h"

#define SF_CONFIG_NAME_DEFINE "default_config"
#define SF_CONFIG_NAME_WIDE_DEFINE MU_C2(L, SF_CONFIG_NAME_DEFINE)

#define SF_SECTION_NAME_DEFINE "MyConfigSectionName"
#define SF_SECTION_NAME_WIDE_DEFINE MU_C2(L,"MyConfigSectionName")




/*uses the same config as sf_service_config_ut.c, except we need the parameter names both as narrow and wide strings*/

#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_1 "Parameter1"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_2 "Parameter2"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_3 "Parameter3WithLongerName"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_some_flag "SomeFlag"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle "StringOptionThandle"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option "MyString"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option "MyWideString"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle_optional "OptionalStringOptionThandle"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_optional "OptionalStringOption"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option_optional "OptionalWideStringOption"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_another_flag "AnotherFlag"


#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_parameter_1                             MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_1)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_parameter_2                             MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_2)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_parameter_3                             MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_parameter_3)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_some_flag                               MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_some_flag)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option_in_thandle                MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option                           MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_wide_string_option                      MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option_in_thandle_optional       MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_in_thandle_optional)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_string_option_optional                  MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_string_option_optional)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_wide_string_option_optional             MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_wide_string_option_optional)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_mocked_another_flag                            MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_mocked_another_flag)

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

#endif /* SERVICE_CONFIG_H */
