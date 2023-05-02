// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef CONFIGURATION_READER_H
#define CONFIGURATION_READER_H


#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#endif

#include "windows.h"
#include "fabricruntime.h"

#include "c_util/rc_string.h"
#include "c_pal/thandle.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint8_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint8_t*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint32_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint32_t*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint64_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint64_t*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_double, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, double*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_char_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, char**, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_thandle_rc_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, THANDLE(RC_STRING)*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_wchar_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, wchar_t**, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_bool, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, bool*, value)(0, MU_FAILURE);

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_READER_H */
