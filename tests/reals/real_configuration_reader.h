// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef REAL_CONFIGURATION_READER_H
#define REAL_CONFIGURATION_READER_H

#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_CONFIGURATION_READER_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        configuration_reader_get_uint8_t, \
        configuration_reader_get_uint32_t, \
        configuration_reader_get_uint64_t, \
        configuration_reader_get_double, \
        configuration_reader_get_char_string, \
        configuration_reader_get_thandle_rc_string, \
        configuration_reader_get_wchar_string, \
        configuration_reader_get_bool \
)

#include "sf_c_util/configuration_reader.h"

int real_configuration_reader_get_uint8_t(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, uint8_t* value);
int real_configuration_reader_get_uint32_t(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, uint32_t* value);
int real_configuration_reader_get_uint64_t(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, uint64_t* value);
int real_configuration_reader_get_double(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, double* value);
int real_configuration_reader_get_char_string(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, char** value);
int real_configuration_reader_get_thandle_rc_string(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, THANDLE(RC_STRING)* value);
int real_configuration_reader_get_wchar_string(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, wchar_t** value);
int real_configuration_reader_get_bool(IFabricCodePackageActivationContext* activation_context, const wchar_t* config_package_name, const wchar_t* section_name, const wchar_t* parameter_name, bool* value);

#endif //REAL_CONFIGURATION_READER_H
