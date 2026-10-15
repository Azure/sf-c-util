// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SAMPLE_SERVICE_CONFIG_H
#define SAMPLE_SERVICE_CONFIG_H

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "macro_utils/macro_utils.h"

#include "sf_c_util/sf_service_config.h"

#include "umock_c/umock_c_prod.h"

#define SAMPLE_SERVICE_CONFIG_PACKAGE_NAME "sample_config_package"
#define SAMPLE_SERVICE_SECTION_NAME "SampleSection"

#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_bool_value "BoolValue"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_double_value "DoubleValue"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint8_t_value "Uint8Value"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint32_t_value "Uint32Value"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint64_t_value "Uint64Value"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_char_ptr_value "CharPtrValue"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_wchar_ptr_value "WcharPtrValue"
#define SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_thandle_rc_string_value "ThandleRcStringValue"

#define SF_SERVICE_CONFIG_PARAMETER_NAME_bool_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_bool_value)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_double_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_double_value)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_uint8_t_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint8_t_value)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_uint32_t_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint32_t_value)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_uint64_t_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_uint64_t_value)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_char_ptr_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_char_ptr_value)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_wchar_ptr_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_wchar_ptr_value)
#define SF_SERVICE_CONFIG_PARAMETER_NAME_thandle_rc_string_value MU_C2C(L, SF_SERVICE_CONFIG_PARAMETER_NAME_NARROW_thandle_rc_string_value)

#define SAMPLE_SERVICE_PARAMS \
    CONFIG_REQUIRED(bool, bool_value), \
    CONFIG_REQUIRED(double, double_value), \
    CONFIG_REQUIRED(uint8_t, uint8_t_value), \
    CONFIG_REQUIRED(uint32_t, uint32_t_value), \
    CONFIG_REQUIRED(uint64_t, uint64_t_value), \
    CONFIG_REQUIRED(char_ptr, char_ptr_value), \
    CONFIG_REQUIRED(wchar_ptr, wchar_ptr_value), \
    CONFIG_REQUIRED(thandle_rc_string, thandle_rc_string_value)

DECLARE_SF_SERVICE_CONFIG(sample_service, SAMPLE_SERVICE_PARAMS)

#endif /* SAMPLE_SERVICE_CONFIG_H */
