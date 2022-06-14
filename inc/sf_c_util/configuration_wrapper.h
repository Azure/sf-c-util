// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef CONFIGURATION_WRAPPER_H
#define CONFIGURATION_WRAPPER_H


#ifdef __cplusplus
#include <cinttypes>
#else
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#endif

#include "windows.h"
#include "fabricruntime.h"

#include "c_logging/xlogging.h"

#include "c_util/rc_string.h"
#include "c_util/thandle.h"

#include "c_pal/string_utils.h"

#include "macro_utils/macro_utils.h"

#include "sf_c_util/configuration_reader.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
Currently supports the following types:
   bool
   uint32_t
   uint64_t
   char* (char_ptr)
   wchar_t* (wchar_ptr)
   THANDLE(RC_STRING) (thandle_rc_string)
*/

typedef char* char_ptr;
typedef wchar_t* wchar_ptr;
typedef THANDLE(RC_STRING) thandle_rc_string;

// Names

/*Codes_SRS_CONFIGURATION_WRAPPER_42_007: [ CONFIGURATION_WRAPPER shall expand to the name of the configuration module by appending the suffix _CONFIGURATION. ]*/
#define CONFIGURATION_WRAPPER(name) MU_C2(name, _CONFIGURATION)
/*Codes_SRS_CONFIGURATION_WRAPPER_42_008: [ CONFIGURATION_WRAPPER shall expand to the name of the create function for the configuration module by appending the suffix _configuration_create. ]*/
#define CONFIGURATION_WRAPPER_CREATE(name) MU_C2(name, _configuration_create)
/*Codes_SRS_CONFIGURATION_WRAPPER_42_043: [ CONFIGURATION_WRAPPER_GETTER shall expand to the name of the getter function for the configuration module and the given param by concatenating the name, the string _configuration_get, and the param. ]*/
#define CONFIGURATION_WRAPPER_GETTER(name, param) MU_C3(name, _configuration_get_, param)

// Helpers to declare parameters

#define CONFIGURATION_WRAPPER_PARAMETER_NAME(name) MU_C2(CONFIGURATION_WRAPPER_PARAMETER_NAME_, name)

// Declare configuration wrapper (for header)

#define DECLARE_CONFIGURATION_WRAPPER(name, ...) \
    /*Codes_SRS_CONFIGURATION_WRAPPER_42_001: [ DECLARE_CONFIGURATION_WRAPPER shall generate a THANDLE declaration of type CONFIGURATION_WRAPPER(name). ]*/ \
    typedef struct MU_C2(name, _CONFIGURATION_TAG) CONFIGURATION_WRAPPER(name); \
    THANDLE_TYPE_DECLARE(CONFIGURATION_WRAPPER(name)); \
    /*Codes_SRS_CONFIGURATION_WRAPPER_42_002: [ DECLARE_CONFIGURATION_WRAPPER shall generate a mockable create function CONFIGURATION_WRAPPER_CREATE(name) which takes an IFabricCodePackageActivationContext* and produces the THANDLE. ]*/ \
    MOCKABLE_FUNCTION(, THANDLE(CONFIGURATION_WRAPPER(name)), CONFIGURATION_WRAPPER_CREATE(name), IFabricCodePackageActivationContext*, activation_context); \
    /*Codes_SRS_CONFIGURATION_WRAPPER_42_003: [ DECLARE_CONFIGURATION_WRAPPER shall generate mockable getter functions CONFIGURATION_WRAPPER_GETTER(name, param) for each of the configurations provided. ]*/ \
    CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_KEEP_1(DECLARE_CONFIGURATION_WRAPPER_GETTER, name, CONFIGURATION_WRAPPER_EXPAND_PARAMS(__VA_ARGS__))


// Define configuration wrapper (for .c file)

#define DEFINE_CONFIGURATION_WRAPPER(name, sf_config_name, sf_parameters_section_name, ...) \
    /*Codes_SRS_CONFIGURATION_WRAPPER_42_004: [ DEFINE_CONFIGURATION_WRAPPER shall generate the CONFIGURATION_WRAPPER(name) struct. ]*/ \
    DEFINE_CONFIGURATION_WRAPPER_STRUCT(CONFIGURATION_WRAPPER(name), sf_config_name, sf_parameters_section_name, CONFIGURATION_WRAPPER_EXPAND_PARAMS(__VA_ARGS__)); \
    THANDLE_TYPE_DEFINE(CONFIGURATION_WRAPPER(name)); \
    DEFINE_CONFIGURATION_WRAPPER_DISPOSE(name, __VA_ARGS__) \
    /*Codes_SRS_CONFIGURATION_WRAPPER_42_005: [ DEFINE_CONFIGURATION_WRAPPER shall generate the implementation of CONFIGURATION_WRAPPER_CREATE(name). ]*/ \
    CONFIGURATION_WRAPPER_DEFINE_CREATE(name, sf_config_name, sf_parameters_section_name, __VA_ARGS__) \
    /*Codes_SRS_CONFIGURATION_WRAPPER_42_006: [ DECLARE_CONFIGURATION_WRAPPER shall generate the implementation of the getter functions CONFIGURATION_WRAPPER_GETTER(name, param) for each of the configurations provided. ]*/ \
    CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_KEEP_1(CONFIGURATION_WRAPPER_DEFINE_GETTER, name, CONFIGURATION_WRAPPER_EXPAND_PARAMS(__VA_ARGS__))


// Implementation details

#define CONFIGURATION_WRAPPER_EXPAND_PARAM_CONFIG_OPTIONAL(field_type, field_name) field_type, field_name
#define CONFIGURATION_WRAPPER_EXPAND_PARAM_CONFIG_REQUIRED(field_type, field_name) field_type, field_name
#define CONFIGURATION_WRAPPER_EXPAND_PARAM(count, field) MU_C2(CONFIGURATION_WRAPPER_EXPAND_PARAM_, field) MU_IFCOMMALOGIC(MU_DEC(count))

// This will strip the REQUIRED and OPTIONAL parts of the fields

#define CONFIGURATION_WRAPPER_EXPAND_PARAMS(...) MU_FOR_EACH_1_COUNTED(CONFIGURATION_WRAPPER_EXPAND_PARAM, __VA_ARGS__)

// When using CONFIGURATION_WRAPPER_EXPAND_PARAMS, we need these layers of indirection so it expands before calling the FOR_EACH

#define CONFIGURATION_WRAPPER_EXPAND_MACRO_HELPER(OUTER, arg1, ...) OUTER(arg1, __VA_ARGS__)
#define CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2(MACRO, ...) CONFIGURATION_WRAPPER_EXPAND_MACRO_HELPER(MU_FOR_EACH_2, MACRO, __VA_ARGS__)
#define CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_COUNTED(MACRO, ...) CONFIGURATION_WRAPPER_EXPAND_MACRO_HELPER(MU_FOR_EACH_2_COUNTED, MACRO, __VA_ARGS__)
#define CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_KEEP_1(MACRO, keep, ...) CONFIGURATION_WRAPPER_EXPAND_MACRO_HELPER(MU_FOR_EACH_2_KEEP_1, MACRO, keep, __VA_ARGS__)
#define CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_KEEP_2(MACRO, keep1, keep2, ...) CONFIGURATION_WRAPPER_EXPAND_MACRO_HELPER(MU_FOR_EACH_2_KEEP_2, MACRO, keep1, keep2, __VA_ARGS__)

// The following will filter the list out to just the optional values or just the required values, so they can be handled differently
// This is done because we can't expand things like "CONFIG_REQUIRED(char_ptr, foo)" to "DO_SOMETHING_CONFIG_REQUIRED(something_else, char_ptr, foo)"

#define CONFIGURATION_WRAPPER_EXPAND_REQUIRED_CONFIG_OPTIONAL(field_type, field_name) SKIP , SKIP
#define CONFIGURATION_WRAPPER_EXPAND_REQUIRED_CONFIG_REQUIRED(field_type, field_name) field_type, field_name
#define CONFIGURATION_WRAPPER_EXPAND_REQUIRED(count, field) MU_C2(CONFIGURATION_WRAPPER_EXPAND_REQUIRED_,field) MU_IFCOMMALOGIC(MU_DEC(count))

#define CONFIGURATION_WRAPPER_EXPAND_REQUIRED_ONLY(...) MU_FOR_EACH_1_COUNTED(CONFIGURATION_WRAPPER_EXPAND_REQUIRED, __VA_ARGS__)

#define CONFIGURATION_WRAPPER_EXPAND_OPTIONAL_CONFIG_OPTIONAL(field_type, field_name) field_type, field_name
#define CONFIGURATION_WRAPPER_EXPAND_OPTIONAL_CONFIG_REQUIRED(field_type, field_name) SKIP , SKIP
#define CONFIGURATION_WRAPPER_EXPAND_OPTIONAL(count, field) MU_C2(CONFIGURATION_WRAPPER_EXPAND_OPTIONAL_,field) MU_IFCOMMALOGIC(MU_DEC(count))

#define CONFIGURATION_WRAPPER_EXPAND_OPTIONAL_ONLY(...) MU_FOR_EACH_1_COUNTED(CONFIGURATION_WRAPPER_EXPAND_OPTIONAL, __VA_ARGS__)

// Helpers for Declare

#define DECLARE_CONFIGURATION_WRAPPER_GETTER(name, type, param) \
    MOCKABLE_FUNCTION(, CONFIGURATION_WRAPPER_RETURN_TYPE(type), CONFIGURATION_WRAPPER_GETTER(name, param), THANDLE(CONFIGURATION_WRAPPER(name)), handle);

// Helpers for Define

// Internal struct

// Need extra layer of indirection so the CONFIGURATION_WRAPPER_EXPAND_PARAMS() macro expands first
#define DEFINE_CONFIGURATION_WRAPPER_STRUCT(name, sf_config_name, sf_parameters_section_name, ...) \
    MU_DEFINE_STRUCT(name, \
        IFabricCodePackageActivationContext*, activation_context,\
        const wchar_t*, sf_config_name_string, \
        const wchar_t*, sf_parameters_section_name_string, \
        __VA_ARGS__)

// Type helpers

#define PRI_uint32_t PRIu32
#define PRI_uint64_t PRIu64

#define CONFIGURATION_READER_FUNCTION_char_ptr configuration_reader_get_char_string
#define CONFIGURATION_READER_FUNCTION_wchar_ptr configuration_reader_get_wchar_string

#define CONFIGURATION_WRAPPER_STR_PREFIX_char_ptr
#define CONFIGURATION_WRAPPER_STR_PREFIX_wchar_ptr L
#define CONFIGURATION_WRAPPER_STR_PREFIX(type) MU_C2(CONFIGURATION_WRAPPER_STR_PREFIX_, type)

#define PRI_char_ptr "s"
#define PRI_wchar_ptr "ls"

#define CONFIGURATION_WRAPPER_P_OR_NULL_char_ptr(value) MU_P_OR_NULL(value)
#define CONFIGURATION_WRAPPER_P_OR_NULL_wchar_ptr(value) MU_WP_OR_NULL(value)
#define CONFIGURATION_WRAPPER_P_OR_NULL(type, value) MU_C2(CONFIGURATION_WRAPPER_P_OR_NULL_, type)(value)

// By default, nothing needs to be free'd
#define CONFIGURATION_WRAPPER_MUST_CLEANUP_TYPE_char_ptr 0
#define CONFIGURATION_WRAPPER_MUST_CLEANUP_TYPE_wchar_ptr 0
#define CONFIGURATION_WRAPPER_MUST_CLEANUP_TYPE_thandle_rc_string 0
#define CONFIGURATION_WRAPPER_MUST_CLEANUP_TYPE(type) MU_IF(MU_C2(CONFIGURATION_WRAPPER_MUST_CLEANUP_TYPE_,type), 0, 1)

/*Codes_SRS_CONFIGURATION_WRAPPER_42_036: [ If the type is char_ptr then MU_C2A(CONFIGURATION_WRAPPER(name), _dispose) shall free the string. ]*/
#define CONFIGURATION_WRAPPER_CLEANUP_FUNCTION_char_ptr(handle, field_name) free(handle->field_name); handle->field_name = NULL
/*Codes_SRS_CONFIGURATION_WRAPPER_42_038: [ If the type is wchar_ptr then MU_C2A(CONFIGURATION_WRAPPER(name), _dispose) shall free the string. ]*/
#define CONFIGURATION_WRAPPER_CLEANUP_FUNCTION_wchar_ptr(handle, field_name) free(handle->field_name); handle->field_name = NULL
/*Codes_SRS_CONFIGURATION_WRAPPER_42_040: [ If the type is thandle_rc_string then MU_C2A(CONFIGURATION_WRAPPER(name), _dispose) shall assign the THANDLE to NULL. ]*/
#define CONFIGURATION_WRAPPER_CLEANUP_FUNCTION_thandle_rc_string(handle, field_name) THANDLE_ASSIGN(RC_STRING)(&handle->field_name, NULL)
#define CONFIGURATION_WRAPPER_CLEANUP_FUNCTION(handle, field_type, field_name) MU_C2(CONFIGURATION_WRAPPER_CLEANUP_FUNCTION_, field_type)(handle, field_name)

// Helpers to read config values
// Note that "error_occurred" is defined below "by convention"

/*Codes_SRS_CONFIGURATION_WRAPPER_42_014: [ If the type is bool then: ]*/
#define CONFIGURATION_WRAPPER_DO_READ__Bool(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null) \
    result_value = false; \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_015: [ CONFIGURATION_WRAPPER_CREATE(name) shall call configuration_reader_get_bool with the activation_context, sf_config_name, sf_parameters_section_name, and CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name. ]*/ \
        if (configuration_reader_get_bool(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_034: [ If there are any errors then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("configuration_reader_get_bool (\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            LogVerbose("Config loaded: %ls = %" PRI_BOOL, parameter_string, MU_BOOL_VALUE(result_value)); \
        } \
    }

#define CONFIGURATION_WRAPPER_DO_READ_integer_type(type, max_value, config, parameter_string, result_value, error_occurred_flag) \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_017: [ CONFIGURATION_WRAPPER_CREATE(name) shall call configuration_reader_get_uint32_t with the activation_context, sf_config_name, sf_parameters_section_name, and CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name. ]*/ \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_020: [ CONFIGURATION_WRAPPER_CREATE(name) shall call configuration_reader_get_uint64_t with the activation_context, sf_config_name, sf_parameters_section_name, and CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name. ]*/ \
        if (MU_C2(configuration_reader_get_, type)(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_034: [ If there are any errors then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            LogError(MU_TOSTRING(MU_C2(configuration_reader_get_, type)) "(\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else if (result_value == max_value) \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_018: [ If the result is UINT32_MAX then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_021: [ If the result is UINT64_MAX then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("Invalid %ls=%" MU_C2(PRI_, type), parameter_string, result_value); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            LogVerbose("Config loaded: %ls = %" MU_C2(PRI_, type), parameter_string, result_value); \
        } \
    }

/*Codes_SRS_CONFIGURATION_WRAPPER_42_016: [ If the type is uint32_t then: ]*/
#define CONFIGURATION_WRAPPER_DO_READ_uint32_t(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null) \
    CONFIGURATION_WRAPPER_DO_READ_integer_type(uint32_t, UINT32_MAX, config, parameter_string, result_value, error_occurred_flag)

/*Codes_SRS_CONFIGURATION_WRAPPER_42_019: [ If the type is uint64_t then: ]*/
#define CONFIGURATION_WRAPPER_DO_READ_uint64_t(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null) \
    CONFIGURATION_WRAPPER_DO_READ_integer_type(uint64_t, UINT64_MAX, config, parameter_string, result_value, error_occurred_flag)

#define CONFIGURATION_WRAPPER_DO_READ_any_char_ptr(config, type, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null) \
    result_value = NULL; \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_023: [ CONFIGURATION_WRAPPER_CREATE(name) shall call configuration_reader_get_char_string with the activation_context, sf_config_name, sf_parameters_section_name, and CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name. ]*/ \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_027: [ CONFIGURATION_WRAPPER_CREATE(name) shall call configuration_reader_get_wchar_string with the activation_context, sf_config_name, sf_parameters_section_name, and CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name. ]*/ \
        if (MU_C2(CONFIGURATION_READER_FUNCTION_, type)(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_034: [ If there are any errors then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            LogError(MU_TOSTRING(MU_C2(CONFIGURATION_READER_FUNCTION_, type))"(\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            if (result_value != NULL && result_value[0] == MU_C2(CONFIGURATION_WRAPPER_STR_PREFIX(type),'\0')) \
            { \
                /*Codes_SRS_CONFIGURATION_WRAPPER_42_024: [ If the value is an empty string then CONFIGURATION_WRAPPER_CREATE(name) shall free the string and set it to NULL. ]*/ \
                /*Codes_SRS_CONFIGURATION_WRAPPER_42_028: [ If the value is an empty string then CONFIGURATION_WRAPPER_CREATE(name) shall free the string and set it to NULL. ]*/ \
                free(result_value); \
                result_value = NULL; \
            } \
            MU_IF(fail_if_null, \
                if (result_value == NULL) \
                { \
                    /*Codes_SRS_CONFIGURATION_WRAPPER_42_025: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
                    /*Codes_SRS_CONFIGURATION_WRAPPER_42_029: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
                    LogError("Invalid %ls=%" MU_C2(PRI_, type), parameter_string, CONFIGURATION_WRAPPER_P_OR_NULL(type, result_value)); \
                    error_occurred_flag = true; \
                } \
                else \
            , \
            ) \
            { \
                LogVerbose("Config loaded: %ls = %" MU_C2(PRI_, type), parameter_string, CONFIGURATION_WRAPPER_P_OR_NULL(type, result_value)); \
            } \
        } \
    }

/*Codes_SRS_CONFIGURATION_WRAPPER_42_022: [ If the type is char_ptr then: ]*/
#define CONFIGURATION_WRAPPER_DO_READ_char_ptr(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null) \
    CONFIGURATION_WRAPPER_DO_READ_any_char_ptr(config, char_ptr, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null)

/*Codes_SRS_CONFIGURATION_WRAPPER_42_026: [ If the type is wchar_ptr then: ]*/
#define CONFIGURATION_WRAPPER_DO_READ_wchar_ptr(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null) \
    CONFIGURATION_WRAPPER_DO_READ_any_char_ptr(config, wchar_ptr, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null)

/*Codes_SRS_CONFIGURATION_WRAPPER_42_030: [ If the type is thandle_rc_string then: ]*/
#define CONFIGURATION_WRAPPER_DO_READ_thandle_rc_string(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null) \
    THANDLE_INITIALIZE(RC_STRING)(&result_value, NULL); \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_031: [ CONFIGURATION_WRAPPER_CREATE(name) shall call configuration_reader_get_thandle_rc_string with the activation_context, sf_config_name, sf_parameters_section_name, and CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name. ]*/ \
        if (configuration_reader_get_thandle_rc_string(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_034: [ If there are any errors then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("configuration_reader_get_thandle_rc_string(\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            if (result_value != NULL && result_value->string[0] == '\0') \
            { \
                /*Codes_SRS_CONFIGURATION_WRAPPER_42_032: [ If the value is an empty string then CONFIGURATION_WRAPPER_CREATE(name) shall free the string and set it to NULL. ]*/ \
                THANDLE_ASSIGN(RC_STRING)(&result_value, NULL); \
            } \
            MU_IF(fail_if_null, \
                if (result_value == NULL) \
                { \
                    /*Codes_SRS_CONFIGURATION_WRAPPER_42_033: [ If the configuration value is CONFIG_REQUIRED and the value is NULL then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
                    LogError("Invalid %ls=%" PRI_RC_STRING, parameter_string, RC_STRING_VALUE_OR_NULL(result_value)); \
                    error_occurred_flag = true; \
                } \
                else \
            , \
            ) \
            { \
                LogVerbose("Config loaded: %ls = %" PRI_RC_STRING, parameter_string, RC_STRING_VALUE_OR_NULL(result_value)); \
            } \
        } \
    }

#define CONFIGURATION_WRAPPER_IS_TYPE_SKIP_SKIP 0
#define CONFIGURATION_WRAPPER_IS_TYPE_SKIP(type) MU_IF(MU_C2(CONFIGURATION_WRAPPER_IS_TYPE_SKIP_, type), 0, 1)

#define CONFIGURATION_WRAPPER_DO_READ_REQUIRED(handle, field_type, field_name) \
    MU_IF(CONFIGURATION_WRAPPER_IS_TYPE_SKIP(field_type),, \
        MU_C2(CONFIGURATION_WRAPPER_DO_READ_, field_type)(handle, field_name, CONFIGURATION_WRAPPER_PARAMETER_NAME(field_name), handle->field_name, error_occurred, 1) \
    )
#define CONFIGURATION_WRAPPER_DO_READ_OPTIONAL(handle, field_type, field_name) \
    MU_IF(CONFIGURATION_WRAPPER_IS_TYPE_SKIP(field_type),, \
        MU_C2(CONFIGURATION_WRAPPER_DO_READ_, field_type)(handle, field_name, CONFIGURATION_WRAPPER_PARAMETER_NAME(field_name), handle->field_name, error_occurred, 0) \
    )

#define CONFIGURATION_WRAPPER_DEFINE_CREATE(name, sf_config_name, sf_parameters_section_name, ...) \
    static int MU_C2(name, _read_all_config_values)(CONFIGURATION_WRAPPER(name)* handle) \
    { \
        int result; \
        bool error_occurred = false; \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_013: [ For each configuration value with name config_name: ]*/ \
        CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_KEEP_1(CONFIGURATION_WRAPPER_DO_READ_REQUIRED, handle, CONFIGURATION_WRAPPER_EXPAND_REQUIRED_ONLY(__VA_ARGS__)); \
        CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_KEEP_1(CONFIGURATION_WRAPPER_DO_READ_OPTIONAL, handle, CONFIGURATION_WRAPPER_EXPAND_OPTIONAL_ONLY(__VA_ARGS__)); \
        if (error_occurred) \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_034: [ If there are any errors then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            MU_C2A(CONFIGURATION_WRAPPER(name), _cleanup_fields)(handle); \
            result = MU_FAILURE; \
        } \
        else \
        { \
            result = 0; \
        } \
        return result; \
    } \
    THANDLE(CONFIGURATION_WRAPPER(name)) CONFIGURATION_WRAPPER_CREATE(name)(IFabricCodePackageActivationContext* activation_context) \
    { \
        THANDLE(CONFIGURATION_WRAPPER(name)) result = NULL; \
        if ( \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_009: [ If activation_context is NULL then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
            activation_context == NULL \
            ) \
        { \
            LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p", \
                activation_context); \
        } \
        else \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_010: [ CONFIGURATION_WRAPPER_CREATE(name) shall allocate the THANDLE(CONFIGURATION_WRAPPER(name)) with MU_C2A(CONFIGURATION_WRAPPER(name), _dispose) as the dispose function. ]*/ \
            THANDLE(CONFIGURATION_WRAPPER(name)) temp_config = THANDLE_MALLOC(CONFIGURATION_WRAPPER(name))(MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)); \
            if (temp_config == NULL) \
            { \
                /*Codes_SRS_CONFIGURATION_WRAPPER_42_034: [ If there are any errors then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
                LogError("THANDLE_MALLOC(" MU_TOSTRING(CONFIGURATION_WRAPPER(name)) ") failed"); \
            } \
            else \
            { \
                CONFIGURATION_WRAPPER(name)* temp_config_obj = THANDLE_GET_T(CONFIGURATION_WRAPPER(name))(temp_config); \
                /*Codes_SRS_CONFIGURATION_WRAPPER_42_011: [ CONFIGURATION_WRAPPER_CREATE(name) shall call AddRef and store the activation_context. ]*/ \
                temp_config_obj->activation_context = activation_context; \
                (void)temp_config_obj->activation_context->lpVtbl->AddRef(temp_config_obj->activation_context); \
                /*Codes_SRS_CONFIGURATION_WRAPPER_42_012: [ CONFIGURATION_WRAPPER_CREATE(name) shall store the sf_config_name and sf_parameters_section_name. ]*/ \
                temp_config_obj->sf_config_name_string = sf_config_name; \
                temp_config_obj->sf_parameters_section_name_string = sf_parameters_section_name; \
                \
                if (MU_C2(name, _read_all_config_values)(temp_config_obj) != 0) \
                { \
                    /*Codes_SRS_CONFIGURATION_WRAPPER_42_034: [ If there are any errors then CONFIGURATION_WRAPPER_CREATE(name) shall fail and return NULL. ]*/ \
                } \
                else \
                { \
                    THANDLE_INITIALIZE_MOVE(CONFIGURATION_WRAPPER(name))(&result, &temp_config); \
                    goto all_ok; \
                } \
                (void)temp_config_obj->activation_context->lpVtbl->Release(temp_config_obj->activation_context); \
                THANDLE_FREE(CONFIGURATION_WRAPPER(name))(temp_config_obj); \
            } \
        } \
    all_ok: \
        return result; \
    }


// Cleanup

#define CONFIGURATION_WRAPPER_DO_CLEANUP_POINTER(handle, field_type, field_name) \
    if (handle->field_name != NULL) \
    { \
        CONFIGURATION_WRAPPER_CLEANUP_FUNCTION(handle, field_type, field_name); \
    }

#define DO_CLEANUP_IF_NEEDED(handle, field_type, field_name) \
    MU_IF(CONFIGURATION_WRAPPER_MUST_CLEANUP_TYPE(field_type), CONFIGURATION_WRAPPER_DO_CLEANUP_POINTER(handle, field_type, field_name), )

#define DEFINE_CONFIGURATION_WRAPPER_DISPOSE(name, ...) \
    static void MU_C2A(CONFIGURATION_WRAPPER(name), _cleanup_fields)(CONFIGURATION_WRAPPER(name)* handle) \
    { \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_035: [ For each config value: ]*/ \
        CONFIGURATION_WRAPPER_EXPANDED_MU_FOR_EACH_2_KEEP_1(DO_CLEANUP_IF_NEEDED, handle, CONFIGURATION_WRAPPER_EXPAND_PARAMS(__VA_ARGS__)) \
    } \
    static void MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)(CONFIGURATION_WRAPPER(name)* handle) \
    { \
        MU_C2A(CONFIGURATION_WRAPPER(name), _cleanup_fields)(handle); \
        /*Codes_SRS_CONFIGURATION_WRAPPER_42_042: [ MU_C2A(CONFIGURATION_WRAPPER(name), _dispose) shall Release the activation_context. ]*/ \
        (void)handle->activation_context->lpVtbl->Release(handle->activation_context); \
    }

// Getters

#define CONFIGURATION_WRAPPER_RETURN_TYPE__Bool bool
#define CONFIGURATION_WRAPPER_RETURN_TYPE_uint32_t uint32_t
#define CONFIGURATION_WRAPPER_RETURN_TYPE_uint64_t uint64_t
#define CONFIGURATION_WRAPPER_RETURN_TYPE_char_ptr const char*
#define CONFIGURATION_WRAPPER_RETURN_TYPE_wchar_ptr const wchar_t*
#define CONFIGURATION_WRAPPER_RETURN_TYPE_thandle_rc_string THANDLE(RC_STRING)

#define CONFIGURATION_WRAPPER_RETURN_TYPE(type) MU_C2A(CONFIGURATION_WRAPPER_RETURN_TYPE_, type)

#define CONFIGURATION_WRAPPER_INIT_RETURN__Bool
#define CONFIGURATION_WRAPPER_INIT_RETURN_uint32_t
#define CONFIGURATION_WRAPPER_INIT_RETURN_uint64_t
#define CONFIGURATION_WRAPPER_INIT_RETURN_char_ptr
#define CONFIGURATION_WRAPPER_INIT_RETURN_wchar_ptr
#define CONFIGURATION_WRAPPER_INIT_RETURN_thandle_rc_string = NULL

#define CONFIGURATION_WRAPPER_INIT_RETURN(type) MU_C2A(CONFIGURATION_WRAPPER_INIT_RETURN_, type)

/*Codes_SRS_CONFIGURATION_WRAPPER_42_045: [ ...false if the type is bool ]*/
#define CONFIGURATION_WRAPPER_GETTER_ERROR__Bool false
/*Codes_SRS_CONFIGURATION_WRAPPER_42_046: [ ...UINT32_MAX if the type is uint32_t ]*/
#define CONFIGURATION_WRAPPER_GETTER_ERROR_uint32_t UINT32_MAX
/*Codes_SRS_CONFIGURATION_WRAPPER_42_047: [ ...UINT64_MAX if the type is uint64_t ]*/
#define CONFIGURATION_WRAPPER_GETTER_ERROR_uint64_t UINT64_MAX
/*Codes_SRS_CONFIGURATION_WRAPPER_42_048: [ ...NULL otherwise ]*/
#define CONFIGURATION_WRAPPER_GETTER_ERROR_char_ptr NULL
#define CONFIGURATION_WRAPPER_GETTER_ERROR_wchar_ptr NULL
#define CONFIGURATION_WRAPPER_GETTER_ERROR_thandle_rc_string NULL

#define CONFIGURATION_WRAPPER_GETTER_ERROR(type) MU_C2(CONFIGURATION_WRAPPER_GETTER_ERROR_, type)

#define CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN__Bool(lval, rval) lval = rval
#define CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN_uint32_t(lval, rval) lval = rval
#define CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN_uint64_t(lval, rval) lval = rval
#define CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN_char_ptr(lval, rval) lval = rval
#define CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN_wchar_ptr(lval, rval) lval = rval
/*Codes_SRS_CONFIGURATION_WRAPPER_42_049: [ If the type is thandle_rc_string then the returned value will be set using THANDLE_INITIALIZE and the caller will have a reference they must free. ]*/
#define CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN_thandle_rc_string(lval, rval) THANDLE_INITIALIZE(RC_STRING)(&lval, rval)

#define CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN(field_type, lval, rval) MU_C2(CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN_, field_type)(lval, rval)

#define CONFIGURATION_WRAPPER_DEFINE_GETTER(name, field_type, field_name) \
    CONFIGURATION_WRAPPER_RETURN_TYPE(field_type) CONFIGURATION_WRAPPER_GETTER(name, field_name)(THANDLE(CONFIGURATION_WRAPPER(name)) handle) \
    { \
        CONFIGURATION_WRAPPER_RETURN_TYPE(field_type) result CONFIGURATION_WRAPPER_INIT_RETURN(field_type); \
        if (handle == NULL) \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_044: [ If handle is NULL then CONFIGURATION_WRAPPER_GETTER(name, field_name) shall fail and return... ]*/ \
            LogError("Invalid args: THANDLE(" MU_TOSTRING(CONFIGURATION_WRAPPER(name)) ") handle = %p", handle); \
            CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN(field_type, result, CONFIGURATION_WRAPPER_GETTER_ERROR(field_type)); \
        } \
        else \
        { \
            /*Codes_SRS_CONFIGURATION_WRAPPER_42_050: [ CONFIGURATION_WRAPPER_GETTER(name, field_name) shall return the configuration value for field_name. ]*/ \
            CONFIGURATION_WRAPPER_GETTER_DO_ASSIGN(field_type, result, handle->field_name); \
        } \
        return result; \
    } \

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_WRAPPER_H */
