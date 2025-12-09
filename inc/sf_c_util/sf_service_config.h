// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SF_SERVICE_CONFIG_H
#define SF_SERVICE_CONFIG_H


#ifdef __cplusplus
#include <cinttypes>
#else
#include <float.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#endif

#include "windows.h"
#include "fabricruntime.h"

#include "c_logging/logger.h"

#include "c_util/rc_string.h"
#include "c_pal/thandle.h"
#include "c_pal/srw_lock_ll.h"

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
   double
   uint8_t
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

/*Codes_SRS_SF_SERVICE_CONFIG_42_007: [ SF_SERVICE_CONFIG shall expand to the name of the configuration module by appending the suffix _CONFIGURATION. ]*/
#define SF_SERVICE_CONFIG(name) MU_C2(name, _CONFIGURATION)
/*Codes_SRS_SF_SERVICE_CONFIG_42_008: [ SF_SERVICE_CONFIG_CREATE shall expand to the name of the create function for the configuration module by appending the suffix _configuration_create. ]*/
#define SF_SERVICE_CONFIG_CREATE(name) MU_C2(name, _configuration_create)
/*Codes_SRS_SF_SERVICE_CONFIG_42_043: [ SF_SERVICE_CONFIG_GETTER shall expand to the name of the getter function for the configuration module and the given param by concatenating the name, the string _configuration_get, and the param. ]*/
#define SF_SERVICE_CONFIG_GETTER(name, param) MU_C3(name, _configuration_get_, param)
/*Codes_SRS_SF_SERVICE_CONFIG_88_001: [ SF_SERVICE_CONFIG_REFRESH shall expand to the name of the refresh function for the configuration module by appending the suffix _configuration_refresh. ]*/
#define SF_SERVICE_CONFIG_REFRESH(name) MU_C2(name, _configuration_refresh)

// Helpers to declare parameters

#define SF_SERVICE_CONFIG_PARAMETER_NAME(name) MU_C2(SF_SERVICE_CONFIG_PARAMETER_NAME_, name)

#define DECLARE_SF_SERVICE_CONFIG_HANDLE(name, ...) \
    /*Codes_SRS_SF_SERVICE_CONFIG_42_001: [ DECLARE_SF_SERVICE_CONFIG_HANDLE shall generate a THANDLE declaration of type SF_SERVICE_CONFIG(name). ]*/ \
    typedef struct MU_C2(name, _CONFIGURATION_TAG) SF_SERVICE_CONFIG(name); \
    THANDLE_TYPE_DECLARE(SF_SERVICE_CONFIG(name)); \
    /*Codes_SRS_SF_SERVICE_CONFIG_42_002: [ DECLARE_SF_SERVICE_CONFIG_GETTERS shall generate a mockable create function SF_SERVICE_CONFIG_CREATE(name) which takes an IFabricCodePackageActivationContext* and produces the THANDLE. ]*/ \
    MOCKABLE_FUNCTION(, THANDLE(SF_SERVICE_CONFIG(name)), SF_SERVICE_CONFIG_CREATE(name), IFabricCodePackageActivationContext*, activation_context); \
    /*Codes_SRS_SF_SERVICE_CONFIG_42_003: [ DECLARE_SF_SERVICE_CONFIG_GETTERS shall generate mockable getter functions SF_SERVICE_CONFIG_GETTER(name, param) for each of the configurations provided. ]*/ \

// separated macro so that it can be used directly in order to allow more options to be declared/defined
#define DECLARE_SF_SERVICE_CONFIG_GETTERS(name, ...) \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_KEEP_2(DECLARE_SF_SERVICE_CONFIG_GETTER, name, dummy, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__))

// Declare refresh function
/*Codes_SRS_SF_SERVICE_CONFIG_88_003: [ DECLARE_SF_SERVICE_CONFIG_REFRESH shall generate a mockable refresh function SF_SERVICE_CONFIG_REFRESH(name) which takes a THANDLE and returns int. ]*/
#define DECLARE_SF_SERVICE_CONFIG_REFRESH(name) \
    MOCKABLE_FUNCTION(, int, SF_SERVICE_CONFIG_REFRESH(name), THANDLE(SF_SERVICE_CONFIG(name)), handle);

// Declare configuration (for header)

#define DECLARE_SF_SERVICE_CONFIG(name, ...) \
    DECLARE_SF_SERVICE_CONFIG_HANDLE(name) \
    DECLARE_SF_SERVICE_CONFIG_GETTERS(name, __VA_ARGS__) \
    DECLARE_SF_SERVICE_CONFIG_REFRESH(name)

// Define configuration (for .c file)

#define DEFINE_SF_SERVICE_CONFIG_HANDLE(name, sf_config_name, sf_parameters_section_name, ...) \
    /*Codes_SRS_SF_SERVICE_CONFIG_42_004: [ DEFINE_SF_SERVICE_CONFIG shall generate the SF_SERVICE_CONFIG(name) struct. ]*/ \
    DEFINE_SF_SERVICE_CONFIG_STRUCT(SF_SERVICE_CONFIG(name), sf_config_name, sf_parameters_section_name, __VA_ARGS__); \
    THANDLE_TYPE_DEFINE(SF_SERVICE_CONFIG(name)); \
    DEFINE_SF_SERVICE_CONFIG_DISPOSE(name, __VA_ARGS__) \
    /*Codes_SRS_SF_SERVICE_CONFIG_42_005: [ DEFINE_SF_SERVICE_CONFIG shall generate the implementation of SF_SERVICE_CONFIG_CREATE(name). ]*/ \
    SF_SERVICE_CONFIG_DEFINE_CREATE(name, sf_config_name, sf_parameters_section_name, __VA_ARGS__)

// separated macro so that it can be used directly in order to allow more options to be declared/defined
#define DEFINE_SF_SERVICE_CONFIG_GETTERS(name, ...) \
    /*Codes_SRS_SF_SERVICE_CONFIG_42_006: [ DECLARE_SF_SERVICE_CONFIG shall generate the implementation of the getter functions SF_SERVICE_CONFIG_GETTER(name, param) for each of the configurations provided. ]*/ \
    SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_KEEP_1(SF_SERVICE_CONFIG_DEFINE_GETTER, name, SF_SERVICE_CONFIG_EXPAND_PARAMS(__VA_ARGS__))

// Define refresh function
#define DEFINE_SF_SERVICE_CONFIG_REFRESH(name, ...) \
    SF_SERVICE_CONFIG_DEFINE_REFRESH(name, __VA_ARGS__)

// Define configuration (for .c file)

#define DEFINE_SF_SERVICE_CONFIG(name, sf_config_name, sf_parameters_section_name, ...) \
    DEFINE_SF_SERVICE_CONFIG_HANDLE(name, sf_config_name, sf_parameters_section_name,  __VA_ARGS__) \
    DEFINE_SF_SERVICE_CONFIG_GETTERS(name, __VA_ARGS__) \
    DEFINE_SF_SERVICE_CONFIG_REFRESH(name, __VA_ARGS__)

// Implementation details

#define SF_SERVICE_CONFIG_EXPAND_PARAM_CONFIG_OPTIONAL(field_type, field_name) field_type, field_name
#define SF_SERVICE_CONFIG_EXPAND_PARAM_CONFIG_REQUIRED(field_type, field_name) field_type, field_name
// No logging flavors
#define SF_SERVICE_CONFIG_EXPAND_PARAM_CONFIG_REQUIRED_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_EXPAND_PARAM_CONFIG_REQUIRED(field_type, field_name)
#define SF_SERVICE_CONFIG_EXPAND_PARAM_CONFIG_OPTIONAL_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_EXPAND_PARAM_CONFIG_OPTIONAL(field_type, field_name)
#define SF_SERVICE_CONFIG_EXPAND_PARAM(count, field) MU_C2(SF_SERVICE_CONFIG_EXPAND_PARAM_, field) MU_IFCOMMALOGIC(MU_DEC(count))

#define SF_SERVICE_CONFIG_EXPAND_PARAM_WITH_REQUIRED_FLAG_CONFIG_OPTIONAL(field_type, field_name) field_type, field_name, 0, 0
#define SF_SERVICE_CONFIG_EXPAND_PARAM_WITH_REQUIRED_FLAG_CONFIG_REQUIRED(field_type, field_name) field_type, field_name, 1, 0
// No logging flavors, the last param is "no logging", set to 1 to not log the value when reading it
#define SF_SERVICE_CONFIG_EXPAND_PARAM_WITH_REQUIRED_FLAG_CONFIG_OPTIONAL_NO_LOGGING(field_type, field_name) field_type, field_name, 0, 1
#define SF_SERVICE_CONFIG_EXPAND_PARAM_WITH_REQUIRED_FLAG_CONFIG_REQUIRED_NO_LOGGING(field_type, field_name) field_type, field_name, 1, 1
#define SF_SERVICE_CONFIG_EXPAND_PARAM_WITH_REQUIRED_FLAG(field) MU_C2(SF_SERVICE_CONFIG_EXPAND_PARAM_WITH_REQUIRED_FLAG_, field)

// This will strip the REQUIRED and OPTIONAL parts of the fields

#define SF_SERVICE_CONFIG_EXPAND_PARAMS(...) MU_FOR_EACH_1_COUNTED(SF_SERVICE_CONFIG_EXPAND_PARAM, __VA_ARGS__)

// When using SF_SERVICE_CONFIG_EXPAND_PARAMS, we need these layers of indirection so it expands before calling the FOR_EACH

#define SF_SERVICE_CONFIG_EXPAND_MACRO_HELPER(OUTER, arg1, ...) OUTER(arg1, __VA_ARGS__)
#define SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2(MACRO, ...) SF_SERVICE_CONFIG_EXPAND_MACRO_HELPER(MU_FOR_EACH_2, MACRO, __VA_ARGS__)
#define SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_COUNTED(MACRO, ...) SF_SERVICE_CONFIG_EXPAND_MACRO_HELPER(MU_FOR_EACH_2_COUNTED, MACRO, __VA_ARGS__)
#define SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_KEEP_1(MACRO, keep, ...) SF_SERVICE_CONFIG_EXPAND_MACRO_HELPER(MU_FOR_EACH_2_KEEP_1, MACRO, keep, __VA_ARGS__)
#define SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_KEEP_2(MACRO, keep1, keep2, ...) SF_SERVICE_CONFIG_EXPAND_MACRO_HELPER(MU_FOR_EACH_2_KEEP_2, MACRO, keep1, keep2, __VA_ARGS__)

// The following will filter the list out to just the optional values or just the required values, so they can be handled differently
// This is done because we can't expand things like "CONFIG_REQUIRED(char_ptr, foo)" to "DO_SOMETHING_CONFIG_REQUIRED(something_else, char_ptr, foo)"

#define SF_SERVICE_CONFIG_EXPAND_REQUIRED_CONFIG_OPTIONAL(field_type, field_name) SKIP , SKIP
#define SF_SERVICE_CONFIG_EXPAND_REQUIRED_CONFIG_REQUIRED(field_type, field_name) field_type, field_name
#define SF_SERVICE_CONFIG_EXPAND_REQUIRED_CONFIG_OPTIONAL_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_EXPAND_REQUIRED_CONFIG_OPTIONAL(field_type, field_name)
#define SF_SERVICE_CONFIG_EXPAND_REQUIRED_CONFIG_REQUIRED_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_EXPAND_REQUIRED_CONFIG_REQUIRED(field_type, field_name)
#define SF_SERVICE_CONFIG_EXPAND_REQUIRED(count, field) MU_C2(SF_SERVICE_CONFIG_EXPAND_REQUIRED_,field) MU_IFCOMMALOGIC(MU_DEC(count))

#define SF_SERVICE_CONFIG_EXPAND_REQUIRED_ONLY(...) MU_FOR_EACH_1_COUNTED(SF_SERVICE_CONFIG_EXPAND_REQUIRED, __VA_ARGS__)

#define SF_SERVICE_CONFIG_EXPAND_OPTIONAL_CONFIG_OPTIONAL(field_type, field_name) field_type, field_name
#define SF_SERVICE_CONFIG_EXPAND_OPTIONAL_CONFIG_REQUIRED(field_type, field_name) SKIP , SKIP
#define SF_SERVICE_CONFIG_EXPAND_OPTIONAL_CONFIG_OPTIONAL_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_EXPAND_OPTIONAL_CONFIG_OPTIONAL(field_type, field_name)
#define SF_SERVICE_CONFIG_EXPAND_OPTIONAL_CONFIG_REQUIRED_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_EXPAND_OPTIONAL_CONFIG_REQUIRED(field_type, field_name)
#define SF_SERVICE_CONFIG_EXPAND_OPTIONAL(count, field) MU_C2(SF_SERVICE_CONFIG_EXPAND_OPTIONAL_,field) MU_IFCOMMALOGIC(MU_DEC(count))

#define SF_SERVICE_CONFIG_EXPAND_OPTIONAL_ONLY(...) MU_FOR_EACH_1_COUNTED(SF_SERVICE_CONFIG_EXPAND_OPTIONAL, __VA_ARGS__)

// Helpers for Declare

// NOTE that we have a dummy variable here so we can use MU_FOR_EACH_2_KEEP_2 instead of MU_FOR_EACH_2_KEEP_1.
// The reason is that MOCKABLE_FUNCTION uses MU_FOR_EACH_2_KEEP_1, and we cannot nest the same macro name

#define DECLARE_SF_SERVICE_CONFIG_GETTER(name, dummy, type, param) \
    MOCKABLE_FUNCTION(, SF_SERVICE_CONFIG_RETURN_TYPE(type), SF_SERVICE_CONFIG_GETTER(name, param), THANDLE(SF_SERVICE_CONFIG(name)), handle);

// Helpers for Define

// Internal struct

#define SF_SERVICE_CONFIG_STRUCT_FIELD_CONFIG_OPTIONAL(field_type, field_name) field_type field_name;
#define SF_SERVICE_CONFIG_STRUCT_FIELD_CONFIG_REQUIRED(field_type, field_name) field_type field_name;
#define SF_SERVICE_CONFIG_STRUCT_FIELD_CONFIG_OPTIONAL_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_STRUCT_FIELD_CONFIG_OPTIONAL(field_type, field_name)
#define SF_SERVICE_CONFIG_STRUCT_FIELD_CONFIG_REQUIRED_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_STRUCT_FIELD_CONFIG_REQUIRED(field_type, field_name)
#define SF_SERVICE_CONFIG_STRUCT_FIELD(field) MU_C2(SF_SERVICE_CONFIG_STRUCT_FIELD_, field)

// Need extra layer of indirection so the SF_SERVICE_CONFIG_EXPAND_PARAMS() macro expands first
#define DEFINE_SF_SERVICE_CONFIG_STRUCT(name, sf_config_name, sf_parameters_section_name, ...) \
    typedef struct MU_C2(name, _TAG) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_88_002: [ The SF_SERVICE_CONFIG(name) struct shall include an SRW_LOCK_LL lock field for thread-safe access. ]*/ \
            SRW_LOCK_LL lock; \
            IFabricCodePackageActivationContext* activation_context; \
            const wchar_t* sf_config_name_string; \
            const wchar_t* sf_parameters_section_name_string; \
            MU_FOR_EACH_1(SF_SERVICE_CONFIG_STRUCT_FIELD, __VA_ARGS__) \
        } name;

// Type helpers

#define PRI_uint8_t PRIu8
#define PRI_uint32_t PRIu32
#define PRI_uint64_t PRIu64

#define CONFIGURATION_READER_FUNCTION_char_ptr configuration_reader_get_char_string
#define CONFIGURATION_READER_FUNCTION_wchar_ptr configuration_reader_get_wchar_string

#define SF_SERVICE_CONFIG_STR_PREFIX_char_ptr
#define SF_SERVICE_CONFIG_STR_PREFIX_wchar_ptr L
#define SF_SERVICE_CONFIG_STR_PREFIX(type) MU_C2(SF_SERVICE_CONFIG_STR_PREFIX_, type)

#define PRI_char_ptr "s"
#define PRI_wchar_ptr "ls"

#define SF_SERVICE_CONFIG_P_OR_NULL_char_ptr(value) MU_P_OR_NULL(value)
#define SF_SERVICE_CONFIG_P_OR_NULL_wchar_ptr(value) MU_WP_OR_NULL(value)
#define SF_SERVICE_CONFIG_P_OR_NULL(type, value) MU_C2(SF_SERVICE_CONFIG_P_OR_NULL_, type)(value)

// By default, nothing needs to be free'd
#define SF_SERVICE_CONFIG_MUST_CLEANUP_TYPE_char_ptr 0
#define SF_SERVICE_CONFIG_MUST_CLEANUP_TYPE_wchar_ptr 0
#define SF_SERVICE_CONFIG_MUST_CLEANUP_TYPE_thandle_rc_string 0
#define SF_SERVICE_CONFIG_MUST_CLEANUP_TYPE(type) MU_IF(MU_C2(SF_SERVICE_CONFIG_MUST_CLEANUP_TYPE_,type), 0, 1)

/*Codes_SRS_SF_SERVICE_CONFIG_42_036: [ If the type is char_ptr then MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall free the string. ]*/
#define SF_SERVICE_CONFIG_CLEANUP_FUNCTION_char_ptr(handle, field_name) free(handle->field_name); handle->field_name = NULL
/*Codes_SRS_SF_SERVICE_CONFIG_42_038: [ If the type is wchar_ptr then MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall free the string. ]*/
#define SF_SERVICE_CONFIG_CLEANUP_FUNCTION_wchar_ptr(handle, field_name) free(handle->field_name); handle->field_name = NULL
/*Codes_SRS_SF_SERVICE_CONFIG_42_040: [ If the type is thandle_rc_string then MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall assign the THANDLE to NULL. ]*/
#define SF_SERVICE_CONFIG_CLEANUP_FUNCTION_thandle_rc_string(handle, field_name) THANDLE_ASSIGN(RC_STRING)(&handle->field_name, NULL)
#define SF_SERVICE_CONFIG_CLEANUP_FUNCTION(handle, field_type, field_name) MU_C2(SF_SERVICE_CONFIG_CLEANUP_FUNCTION_, field_type)(handle, field_name)

// Helpers to read config values
// Note that "error_occurred" is defined below "by convention"

/*Codes_SRS_SF_SERVICE_CONFIG_42_014: [ If the type is bool then: ]*/
#define SF_SERVICE_CONFIG_DO_READ__Bool(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    result_value = false; \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_015: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_bool with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        if (configuration_reader_get_bool(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("configuration_reader_get_bool (\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            if (no_logging) \
            { \
                LogVerbose("Config loaded: %ls = ***", parameter_string); \
            } \
            else \
            { \
                LogVerbose("Config loaded: %ls = %" PRI_BOOL "", parameter_string, MU_BOOL_VALUE(result_value)); \
            } \
        } \
    }

/*Codes_SRS_SF_SERVICE_CONFIG_22_001: [ If the type is double then: ]*/
#define SF_SERVICE_CONFIG_DO_READ_double(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_SF_SERVICE_CONFIG_22_002: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_double with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        if (configuration_reader_get_double(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("configuration_reader_get_double (\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else if (result_value == DBL_MAX) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_22_003: [ If the result is DBL_MAX then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("Invalid %ls=%lf", parameter_string, result_value); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            if (no_logging) \
            { \
                LogVerbose("Config loaded: %ls = ***", parameter_string); \
            } \
            else \
            { \
                LogVerbose("Config loaded: %ls = %lf", parameter_string, result_value); \
            } \
        } \
    }

#define SF_SERVICE_CONFIG_DO_READ_integer_type(type, max_value, config, parameter_string, result_value, error_occurred_flag, no_logging) \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_SF_SERVICE_CONFIG_01_002: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_uint8_t with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_017: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_uint32_t with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_020: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_uint64_t with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        if (MU_C2(configuration_reader_get_, type)(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            LogError(MU_TOSTRING(MU_C2(configuration_reader_get_, type)) "(\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else if (result_value == max_value) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_01_003: [ If the result is UINT8_MAX then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_018: [ If the result is UINT32_MAX then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_021: [ If the result is UINT64_MAX then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("Invalid %ls=%" MU_C2(PRI_, type), parameter_string, result_value); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            if (no_logging) \
            { \
                LogVerbose("Config loaded: %ls = ***", parameter_string); \
            } \
            else \
            { \
                LogVerbose("Config loaded: %ls = %" MU_C2(PRI_, type), parameter_string, result_value); \
            } \
        } \
    }

/*Codes_SRS_SF_SERVICE_CONFIG_01_001: [ If the type is uint8_t then: ]*/
#define SF_SERVICE_CONFIG_DO_READ_uint8_t(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    SF_SERVICE_CONFIG_DO_READ_integer_type(uint8_t, UINT8_MAX, config, parameter_string, result_value, error_occurred_flag, no_logging)

/*Codes_SRS_SF_SERVICE_CONFIG_42_016: [ If the type is uint32_t then: ]*/
#define SF_SERVICE_CONFIG_DO_READ_uint32_t(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    SF_SERVICE_CONFIG_DO_READ_integer_type(uint32_t, UINT32_MAX, config, parameter_string, result_value, error_occurred_flag, no_logging)

/*Codes_SRS_SF_SERVICE_CONFIG_42_019: [ If the type is uint64_t then: ]*/
#define SF_SERVICE_CONFIG_DO_READ_uint64_t(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    SF_SERVICE_CONFIG_DO_READ_integer_type(uint64_t, UINT64_MAX, config, parameter_string, result_value, error_occurred_flag, no_logging)

#define SF_SERVICE_CONFIG_DO_READ_any_char_ptr(config, type, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    result_value = NULL; \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_023: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_char_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_027: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_wchar_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        if (MU_C2(CONFIGURATION_READER_FUNCTION_, type)(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            LogError(MU_TOSTRING(MU_C2(CONFIGURATION_READER_FUNCTION_, type))"(\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            if (result_value != NULL && result_value[0] == MU_C2(SF_SERVICE_CONFIG_STR_PREFIX(type),'\0')) \
            { \
                /*Codes_SRS_SF_SERVICE_CONFIG_42_024: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/ \
                /*Codes_SRS_SF_SERVICE_CONFIG_42_028: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/ \
                free(result_value); \
                result_value = NULL; \
            } \
            MU_IF(fail_if_null, \
                if (result_value == NULL) \
                { \
                    /*Codes_SRS_SF_SERVICE_CONFIG_42_025: [ If the configuration value is CONFIG_REQUIRED or CONFIG_REQUIRED_NO_LOGGING and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
                    /*Codes_SRS_SF_SERVICE_CONFIG_42_029: [ If the configuration value is CONFIG_REQUIRED or CONFIG_REQUIRED_NO_LOGGING and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
                    LogError("Invalid %ls=%" MU_C2(PRI_, type), parameter_string, SF_SERVICE_CONFIG_P_OR_NULL(type, result_value)); \
                    error_occurred_flag = true; \
                } \
                else \
            , \
            ) \
            { \
                if (no_logging) \
                { \
                    LogVerbose("Config loaded: %ls = ***", parameter_string); \
                } \
                else \
                { \
                    LogVerbose("Config loaded: %ls = %" MU_C2(PRI_, type), parameter_string, SF_SERVICE_CONFIG_P_OR_NULL(type, result_value)); \
                } \
            } \
        } \
    }

/*Codes_SRS_SF_SERVICE_CONFIG_42_022: [ If the type is char_ptr then: ]*/
#define SF_SERVICE_CONFIG_DO_READ_char_ptr(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    SF_SERVICE_CONFIG_DO_READ_any_char_ptr(config, char_ptr, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging)

/*Codes_SRS_SF_SERVICE_CONFIG_42_026: [ If the type is wchar_ptr then: ]*/
#define SF_SERVICE_CONFIG_DO_READ_wchar_ptr(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    SF_SERVICE_CONFIG_DO_READ_any_char_ptr(config, wchar_ptr, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging)

/*Codes_SRS_SF_SERVICE_CONFIG_42_030: [ If the type is thandle_rc_string then: ]*/
#define SF_SERVICE_CONFIG_DO_READ_thandle_rc_string(config, field_name, parameter_string, result_value, error_occurred_flag, fail_if_null, no_logging) \
    THANDLE_INITIALIZE(RC_STRING)(&result_value, NULL); \
    if (!error_occurred_flag) \
    { \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_031: [ SF_SERVICE_CONFIG_CREATE(name) shall call configuration_reader_get_thandle_rc_string with the activation_context, sf_config_name, sf_parameters_section_name, and SF_SERVICE_CONFIG_PARAMETER_NAME_config_name. ]*/ \
        if (configuration_reader_get_thandle_rc_string(config->activation_context, config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string, &result_value) != 0) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            LogError("configuration_reader_get_thandle_rc_string(\"%ls\", \"%ls\", \"%ls\") failed", \
                config->sf_config_name_string, config->sf_parameters_section_name_string, parameter_string); \
            error_occurred_flag = true; \
        } \
        else \
        { \
            if (result_value != NULL && result_value->string[0] == '\0') \
            { \
                /*Codes_SRS_SF_SERVICE_CONFIG_42_032: [ If the value is an empty string then SF_SERVICE_CONFIG_CREATE(name) shall free the string and set it to NULL. ]*/ \
                THANDLE_ASSIGN(RC_STRING)(&result_value, NULL); \
            } \
            MU_IF(fail_if_null, \
                if (result_value == NULL) \
                { \
                    /*Codes_SRS_SF_SERVICE_CONFIG_42_033: [ If the configuration value is CONFIG_REQUIRED or CONFIG_REQUIRED_NO_LOGGING and the value is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
                    LogError("Invalid %ls=%" PRI_RC_STRING, parameter_string, RC_STRING_VALUE_OR_NULL(result_value)); \
                    error_occurred_flag = true; \
                } \
                else \
            , \
            ) \
            { \
                if (no_logging) \
                { \
                    LogVerbose("Config loaded: %ls = ***", parameter_string); \
                } \
                else \
                { \
                    LogVerbose("Config loaded: %ls = %" PRI_RC_STRING, parameter_string, RC_STRING_VALUE_OR_NULL(result_value)); \
                } \
            } \
        } \
    }

#define SF_SERVICE_CONFIG_IS_TYPE_SKIP_SKIP 0
#define SF_SERVICE_CONFIG_IS_TYPE_SKIP(type) MU_IF(MU_C2(SF_SERVICE_CONFIG_IS_TYPE_SKIP_, type), 0, 1)

#define SF_SERVICE_CONFIG_DO_READ_REQUIRED(handle, field_type, field_name) \
    MU_IF(SF_SERVICE_CONFIG_IS_TYPE_SKIP(field_type),, \
        MU_C2(SF_SERVICE_CONFIG_DO_READ_, field_type)(handle, field_name, SF_SERVICE_CONFIG_PARAMETER_NAME(field_name), handle->field_name, error_occurred, 1) \
    )
#define SF_SERVICE_CONFIG_DO_READ_OPTIONAL(handle, field_type, field_name) \
    MU_IF(SF_SERVICE_CONFIG_IS_TYPE_SKIP(field_type),, \
        MU_C2(SF_SERVICE_CONFIG_DO_READ_, field_type)(handle, field_name, SF_SERVICE_CONFIG_PARAMETER_NAME(field_name), handle->field_name, error_occurred, 0) \
    )
#define SF_SERVICE_CONFIG_DO_READ_EITHER(handle, field_type, field_name, is_required, no_logging) \
    MU_IF(SF_SERVICE_CONFIG_IS_TYPE_SKIP(field_type),, \
        MU_C2(SF_SERVICE_CONFIG_DO_READ_, field_type)(handle, field_name, SF_SERVICE_CONFIG_PARAMETER_NAME(field_name), handle->field_name, error_occurred, is_required, no_logging) \
    )

#define SF_SERVICE_CONFIG_DO_READ(handle, config) SF_SERVICE_CONFIG_EXPAND_MACRO_HELPER(SF_SERVICE_CONFIG_DO_READ_EITHER, handle, SF_SERVICE_CONFIG_EXPAND_PARAM_WITH_REQUIRED_FLAG(config))

#define SF_SERVICE_CONFIG_DEFINE_CREATE(name, sf_config_name, sf_parameters_section_name, ...) \
    static int MU_C2(name, _read_all_config_values)(SF_SERVICE_CONFIG(name)* handle) \
    { \
        int result; \
        bool error_occurred = false; \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_013: [ For each configuration value with name config_name: ]*/ \
        MU_FOR_EACH_1_KEEP_1(SF_SERVICE_CONFIG_DO_READ, handle, __VA_ARGS__) \
        if (error_occurred) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            MU_C2A(SF_SERVICE_CONFIG(name), _cleanup_fields)(handle); \
            result = MU_FAILURE; \
        } \
        else \
        { \
            result = 0; \
        } \
        return result; \
    } \
    THANDLE(SF_SERVICE_CONFIG(name)) SF_SERVICE_CONFIG_CREATE(name)(IFabricCodePackageActivationContext* activation_context) \
    { \
        THANDLE(SF_SERVICE_CONFIG(name)) result = NULL; \
        if ( \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_009: [ If activation_context is NULL then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
            activation_context == NULL \
            ) \
        { \
            LogError("Invalid args: IFabricCodePackageActivationContext* activation_context = %p", \
                activation_context); \
        } \
        else \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_010: [ SF_SERVICE_CONFIG_CREATE(name) shall allocate the THANDLE(SF_SERVICE_CONFIG(name)) with MU_C2A(SF_SERVICE_CONFIG(name), _dispose) as the dispose function. ]*/ \
            THANDLE(SF_SERVICE_CONFIG(name)) temp_config = THANDLE_MALLOC(SF_SERVICE_CONFIG(name))(MU_C2A(SF_SERVICE_CONFIG(name), _dispose)); \
            if (temp_config == NULL) \
            { \
                /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
                LogError("THANDLE_MALLOC(" MU_TOSTRING(SF_SERVICE_CONFIG(name)) ") failed"); \
            } \
            else \
            { \
                SF_SERVICE_CONFIG(name)* temp_config_obj = THANDLE_GET_T(SF_SERVICE_CONFIG(name))(temp_config); \
                /*Codes_SRS_SF_SERVICE_CONFIG_88_004: [ SF_SERVICE_CONFIG_CREATE(name) shall call srw_lock_ll_init to initialize the SRW lock. ]*/ \
                if (srw_lock_ll_init(&temp_config_obj->lock) != 0) \
                { \
                    /*Codes_SRS_SF_SERVICE_CONFIG_88_005: [ If srw_lock_ll_init fails then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
                    LogError("srw_lock_ll_init failed"); \
                    THANDLE_FREE(SF_SERVICE_CONFIG(name))(temp_config_obj); \
                } \
                else \
                { \
                    /*Codes_SRS_SF_SERVICE_CONFIG_42_011: [ SF_SERVICE_CONFIG_CREATE(name) shall call AddRef and store the activation_context. ]*/ \
                    temp_config_obj->activation_context = activation_context; \
                    (void)temp_config_obj->activation_context->lpVtbl->AddRef(temp_config_obj->activation_context); \
                    /*Codes_SRS_SF_SERVICE_CONFIG_42_012: [ SF_SERVICE_CONFIG_CREATE(name) shall store the sf_config_name and sf_parameters_section_name. ]*/ \
                    temp_config_obj->sf_config_name_string = sf_config_name; \
                    temp_config_obj->sf_parameters_section_name_string = sf_parameters_section_name; \
                    \
                    if (MU_C2(name, _read_all_config_values)(temp_config_obj) != 0) \
                    { \
                        /*Codes_SRS_SF_SERVICE_CONFIG_42_034: [ If there are any errors then SF_SERVICE_CONFIG_CREATE(name) shall fail and return NULL. ]*/ \
                    } \
                    else \
                    { \
                        THANDLE_INITIALIZE_MOVE(SF_SERVICE_CONFIG(name))(&result, &temp_config); \
                        goto all_ok; \
                    } \
                    (void)temp_config_obj->activation_context->lpVtbl->Release(temp_config_obj->activation_context); \
                    srw_lock_ll_deinit(&temp_config_obj->lock); \
                    THANDLE_FREE(SF_SERVICE_CONFIG(name))(temp_config_obj); \
                } \
            } \
        } \
    all_ok: \
        return result; \
    }


// Cleanup

#define SF_SERVICE_CONFIG_DO_CLEANUP_POINTER(handle, field) \
    if (handle->SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD(field) != NULL) \
    { \
        SF_SERVICE_CONFIG_CLEANUP_FUNCTION(handle, SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD(field), SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD(field)); \
    }

#define SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD_CONFIG_OPTIONAL(field_type, field_name) field_type
#define SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD_CONFIG_REQUIRED(field_type, field_name) field_type
#define SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD_CONFIG_OPTIONAL_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD_CONFIG_OPTIONAL(field_type, field_name)
#define SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD_CONFIG_REQUIRED_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD_CONFIG_REQUIRED(field_type, field_name)
#define SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD(field) MU_C2(SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD_, field)

#define SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD_CONFIG_OPTIONAL(field_type, field_name) field_name
#define SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD_CONFIG_REQUIRED(field_type, field_name) field_name
#define SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD_CONFIG_OPTIONAL_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD_CONFIG_OPTIONAL(field_type, field_name)
#define SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD_CONFIG_REQUIRED_NO_LOGGING(field_type, field_name) SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD_CONFIG_REQUIRED(field_type, field_name)
#define SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD(field) MU_C2(SF_SERVICE_CONFIG_FIELD_NAME_FROM_FIELD_, field)

#define DO_CLEANUP_IF_NEEDED(handle, field) \
    MU_IF(SF_SERVICE_CONFIG_MUST_CLEANUP_TYPE(SF_SERVICE_CONFIG_FIELD_TYPE_FROM_FIELD(field)), SF_SERVICE_CONFIG_DO_CLEANUP_POINTER(handle, field), )

#define DEFINE_SF_SERVICE_CONFIG_DISPOSE(name, ...) \
    static void MU_C2A(SF_SERVICE_CONFIG(name), _cleanup_fields)(SF_SERVICE_CONFIG(name)* handle) \
    { \
        (void)handle; /*handle can be unused when none of the parameters need cleaning, that is: SF_SERVICE_CONFIG_EXPANDED_MU_FOR_EACH_2_KEEP_1(DO_CLEANUP_IF_NEEDED, ... ) below expands to nothing*/ \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_035: [ For each config value: ]*/ \
        MU_FOR_EACH_1_KEEP_1(DO_CLEANUP_IF_NEEDED, handle, __VA_ARGS__) \
    } \
    static void MU_C2A(SF_SERVICE_CONFIG(name), _dispose)(SF_SERVICE_CONFIG(name)* handle) \
    { \
        MU_C2A(SF_SERVICE_CONFIG(name), _cleanup_fields)(handle); \
        /*Codes_SRS_SF_SERVICE_CONFIG_88_006: [ MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall call srw_lock_ll_deinit to deinitialize the SRW lock. ]*/ \
        srw_lock_ll_deinit(&handle->lock); \
        /*Codes_SRS_SF_SERVICE_CONFIG_42_042: [ MU_C2A(SF_SERVICE_CONFIG(name), _dispose) shall Release the activation_context. ]*/ \
        (void)handle->activation_context->lpVtbl->Release(handle->activation_context); \
    }

// Getters

#define SF_SERVICE_CONFIG_RETURN_TYPE__Bool bool
#define SF_SERVICE_CONFIG_RETURN_TYPE_double double
#define SF_SERVICE_CONFIG_RETURN_TYPE_uint8_t uint8_t
#define SF_SERVICE_CONFIG_RETURN_TYPE_uint32_t uint32_t
#define SF_SERVICE_CONFIG_RETURN_TYPE_uint64_t uint64_t
#define SF_SERVICE_CONFIG_RETURN_TYPE_char_ptr const char*
#define SF_SERVICE_CONFIG_RETURN_TYPE_wchar_ptr const wchar_t*
#define SF_SERVICE_CONFIG_RETURN_TYPE_thandle_rc_string THANDLE(RC_STRING)

#define SF_SERVICE_CONFIG_RETURN_TYPE(type) MU_C2A(SF_SERVICE_CONFIG_RETURN_TYPE_, type)

#define SF_SERVICE_CONFIG_INIT_RETURN__Bool
#define SF_SERVICE_CONFIG_INIT_RETURN_double
#define SF_SERVICE_CONFIG_INIT_RETURN_uint8_t
#define SF_SERVICE_CONFIG_INIT_RETURN_uint32_t
#define SF_SERVICE_CONFIG_INIT_RETURN_uint64_t
#define SF_SERVICE_CONFIG_INIT_RETURN_char_ptr
#define SF_SERVICE_CONFIG_INIT_RETURN_wchar_ptr
#define SF_SERVICE_CONFIG_INIT_RETURN_thandle_rc_string = NULL

#define SF_SERVICE_CONFIG_INIT_RETURN(type) MU_C2A(SF_SERVICE_CONFIG_INIT_RETURN_, type)

/*Codes_SRS_SF_SERVICE_CONFIG_42_045: [ ...false if the type is bool ]*/
#define SF_SERVICE_CONFIG_GETTER_ERROR__Bool false
/*Codes_SRS_SF_SERVICE_CONFIG_22_004: [ ...DBL_MAX if the type is double ]*/
#define SF_SERVICE_CONFIG_GETTER_ERROR_double DBL_MAX
/*Codes_SRS_SF_SERVICE_CONFIG_01_004: [ ...UINT8_MAX if the type is uint8_t ]*/
#define SF_SERVICE_CONFIG_GETTER_ERROR_uint8_t UINT8_MAX
/*Codes_SRS_SF_SERVICE_CONFIG_42_046: [ ...UINT32_MAX if the type is uint32_t ]*/
#define SF_SERVICE_CONFIG_GETTER_ERROR_uint32_t UINT32_MAX
/*Codes_SRS_SF_SERVICE_CONFIG_42_047: [ ...UINT64_MAX if the type is uint64_t ]*/
#define SF_SERVICE_CONFIG_GETTER_ERROR_uint64_t UINT64_MAX
/*Codes_SRS_SF_SERVICE_CONFIG_42_048: [ ...NULL otherwise ]*/
#define SF_SERVICE_CONFIG_GETTER_ERROR_char_ptr NULL
#define SF_SERVICE_CONFIG_GETTER_ERROR_wchar_ptr NULL
#define SF_SERVICE_CONFIG_GETTER_ERROR_thandle_rc_string NULL

#define SF_SERVICE_CONFIG_GETTER_ERROR(type) MU_C2(SF_SERVICE_CONFIG_GETTER_ERROR_, type)

#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN__Bool(lval, rval) lval = rval
#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_double(lval, rval) lval = rval
#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_uint8_t(lval, rval) lval = rval
#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_uint32_t(lval, rval) lval = rval
#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_uint64_t(lval, rval) lval = rval
#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_char_ptr(lval, rval) lval = rval
#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_wchar_ptr(lval, rval) lval = rval
/*Codes_SRS_SF_SERVICE_CONFIG_42_049: [ If the type is thandle_rc_string then the returned value will be set using THANDLE_INITIALIZE and the caller will have a reference they must free. ]*/
#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_thandle_rc_string(lval, rval) THANDLE_INITIALIZE(RC_STRING)(&lval, rval)

#define SF_SERVICE_CONFIG_GETTER_DO_ASSIGN(field_type, lval, rval) MU_C2(SF_SERVICE_CONFIG_GETTER_DO_ASSIGN_, field_type)(lval, rval)

#define SF_SERVICE_CONFIG_DEFINE_GETTER(name, field_type, field_name) \
    SF_SERVICE_CONFIG_RETURN_TYPE(field_type) SF_SERVICE_CONFIG_GETTER(name, field_name)(THANDLE(SF_SERVICE_CONFIG(name)) handle) \
    { \
        SF_SERVICE_CONFIG_RETURN_TYPE(field_type) result SF_SERVICE_CONFIG_INIT_RETURN(field_type); \
        if (handle == NULL) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_044: [ If handle is NULL then SF_SERVICE_CONFIG_GETTER(name, field_name) shall fail and return... ]*/ \
            LogError("Invalid args: THANDLE(" MU_TOSTRING(SF_SERVICE_CONFIG(name)) ") handle = %p", handle); \
            SF_SERVICE_CONFIG_GETTER_DO_ASSIGN(field_type, result, SF_SERVICE_CONFIG_GETTER_ERROR(field_type)); \
        } \
        else \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_88_007: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall acquire the shared SRW lock by calling srw_lock_ll_acquire_shared. ]*/ \
            srw_lock_ll_acquire_shared(&((SF_SERVICE_CONFIG(name)*)handle)->lock); \
            /*Codes_SRS_SF_SERVICE_CONFIG_42_050: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall return the configuration value for field_name. ]*/ \
            SF_SERVICE_CONFIG_GETTER_DO_ASSIGN(field_type, result, handle->field_name); \
            /*Codes_SRS_SF_SERVICE_CONFIG_88_008: [ SF_SERVICE_CONFIG_GETTER(name, field_name) shall release the shared SRW lock by calling srw_lock_ll_release_shared. ]*/ \
            srw_lock_ll_release_shared(&((SF_SERVICE_CONFIG(name)*)handle)->lock); \
        } \
        return result; \
    } \

// Refresh function

#define SF_SERVICE_CONFIG_DEFINE_REFRESH(name, ...) \
    int SF_SERVICE_CONFIG_REFRESH(name)(THANDLE(SF_SERVICE_CONFIG(name)) handle) \
    { \
        int result; \
        if (handle == NULL) \
        { \
            /*Codes_SRS_SF_SERVICE_CONFIG_88_009: [ If handle is NULL then SF_SERVICE_CONFIG_REFRESH(name) shall fail and return a non-zero value. ]*/ \
            LogError("Invalid args: THANDLE(" MU_TOSTRING(SF_SERVICE_CONFIG(name)) ") handle = %p", handle); \
            result = MU_FAILURE; \
        } \
        else \
        { \
            SF_SERVICE_CONFIG(name)* handle_obj = THANDLE_GET_T(SF_SERVICE_CONFIG(name))(handle); \
            /*Codes_SRS_SF_SERVICE_CONFIG_88_010: [ SF_SERVICE_CONFIG_REFRESH(name) shall acquire the exclusive SRW lock by calling srw_lock_ll_acquire_exclusive. ]*/ \
            srw_lock_ll_acquire_exclusive(&handle_obj->lock); \
            /*Codes_SRS_SF_SERVICE_CONFIG_88_011: [ SF_SERVICE_CONFIG_REFRESH(name) shall clean up the existing configuration field values. ]*/ \
            MU_C2A(SF_SERVICE_CONFIG(name), _cleanup_fields)(handle_obj); \
            /*Codes_SRS_SF_SERVICE_CONFIG_88_012: [ SF_SERVICE_CONFIG_REFRESH(name) shall re-read all configuration values from the activation_context. ]*/ \
            if (MU_C2(name, _read_all_config_values)(handle_obj) != 0) \
            { \
                /*Codes_SRS_SF_SERVICE_CONFIG_88_013: [ If re-reading any configuration value fails, SF_SERVICE_CONFIG_REFRESH(name) shall release the exclusive lock and return a non-zero value. ]*/ \
                LogError("Failed to re-read configuration values"); \
                result = MU_FAILURE; \
            } \
            else \
            { \
                /*Codes_SRS_SF_SERVICE_CONFIG_88_014: [ SF_SERVICE_CONFIG_REFRESH(name) shall release the exclusive SRW lock by calling srw_lock_ll_release_exclusive and return 0. ]*/ \
                result = 0; \
            } \
            srw_lock_ll_release_exclusive(&handle_obj->lock); \
        } \
        return result; \
    }

#ifdef __cplusplus
}
#endif

#endif /* SF_SERVICE_CONFIG_H */
