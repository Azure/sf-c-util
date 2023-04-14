# `configuration_reader` requirements

## Overview

`configuration_reader` is a module that reads the service fabric configuration for the code package.

## Exposed API

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint8_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint8_t*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint32_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint32_t*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint64_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint64_t*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_char_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, char**, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_thandle_rc_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, THANDLE(RC_STRING)*, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_wchar_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, wchar_t**, value)(0, MU_FAILURE);

MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_bool, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, bool*, value)(0, MU_FAILURE);
```

### configuration_reader_get_uint8_t

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint8_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint8_t*, value)(0, MU_FAILURE);
```

`configuration_reader_get_uint8_t` reads a configuration value from the code package and returns it as `uint8_t`.

**SRS_CONFIGURATION_READER_01_001: [** If `activation_context` is `NULL` then `configuration_reader_get_uint8_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_01_002: [** If `config_package_name` is `NULL` or empty then `configuration_reader_get_uint8_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_01_003: [** If `section_name` is `NULL` or empty then `configuration_reader_get_uint8_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_01_004: [** If `parameter_name` is `NULL` or empty then `configuration_reader_get_uint8_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_01_005: [** If `value` is `NULL` then `configuration_reader_get_uint8_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_01_006: [** `configuration_reader_get_uint8_t` shall call the `GetConfigurationPackage` function on `activation_context` with `config_package_name`. **]**

**SRS_CONFIGURATION_READER_01_007: [** `configuration_reader_get_uint8_t` shall call `GetValue` on the configuration package with `section_name` and `parameter_name`. **]**

**SRS_CONFIGURATION_READER_01_008: [** `configuration_reader_get_uint8_t` shall convert the value to `uint8_t` and store it in `value`. **]**

**SRS_CONFIGURATION_READER_01_009: [** If the value is outside the range of representable values then `configuration_reader_get_uint8_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_01_010: [** If there are any other failures then `configuration_reader_get_uint8_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_01_011: [** `configuration_reader_get_uint8_t` shall succeed and return 0. **]**

### configuration_reader_get_uint8_t

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint32_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint32_t*, value)(0, MU_FAILURE);
```

`configuration_reader_get_uint32_t` reads a configuration value from the code package and returns it as `uint32_t`.

**SRS_CONFIGURATION_READER_42_012: [** If `activation_context` is `NULL` then `configuration_reader_get_uint32_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_013: [** If `config_package_name` is `NULL` or empty then `configuration_reader_get_uint32_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_014: [** If `section_name` is `NULL` or empty then `configuration_reader_get_uint32_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_015: [** If `parameter_name` is `NULL` or empty then `configuration_reader_get_uint32_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_016: [** If `value` is `NULL` then `configuration_reader_get_uint32_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_017: [** `configuration_reader_get_uint32_t` shall call the `GetConfigurationPackage` function on `activation_context` with `config_package_name`. **]**

**SRS_CONFIGURATION_READER_42_018: [** `configuration_reader_get_uint32_t` shall call `GetValue` on the configuration package with `section_name` and `parameter_name`. **]**

**SRS_CONFIGURATION_READER_42_019: [** `configuration_reader_get_uint32_t` shall convert the value to `uint32_t` and store it in `value`. **]**

**SRS_CONFIGURATION_READER_42_020: [** If the value is outside the range of representable values then `configuration_reader_get_uint32_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_021: [** If there are any other failures then `configuration_reader_get_uint32_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_022: [** `configuration_reader_get_uint32_t` shall succeed and return 0. **]**

### configuration_reader_get_uint64_t

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_uint64_t, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, uint64_t*, value)(0, MU_FAILURE);
```

`configuration_reader_get_uint64_t` reads a configuration value from the code package and returns it as `uint64_t`.

**SRS_CONFIGURATION_READER_42_001: [** If `activation_context` is `NULL` then `configuration_reader_get_uint64_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_002: [** If `config_package_name` is `NULL` or empty then `configuration_reader_get_uint64_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_003: [** If `section_name` is `NULL` or empty then `configuration_reader_get_uint64_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_004: [** If `parameter_name` is `NULL` or empty then `configuration_reader_get_uint64_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_005: [** If `value` is `NULL` then `configuration_reader_get_uint64_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_006: [** `configuration_reader_get_uint64_t` shall call the `GetConfigurationPackage` function on `activation_context` with `config_package_name`. **]**

**SRS_CONFIGURATION_READER_42_007: [** `configuration_reader_get_uint64_t` shall call `GetValue` on the configuration package with `section_name` and `parameter_name`. **]**

**SRS_CONFIGURATION_READER_42_008: [** `configuration_reader_get_uint64_t` shall convert the value to `uint64_t` and store it in `value`. **]**

**SRS_CONFIGURATION_READER_42_009: [** If the value is outside the range of representable values then `configuration_reader_get_uint64_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_010: [** If there are any other failures then `configuration_reader_get_uint64_t` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_011: [** `configuration_reader_get_uint64_t` shall succeed and return 0. **]**

### configuration_reader_get_char_string

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_char_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, char**, value)(0, MU_FAILURE);
```

`configuration_reader_get_char_string` reads a configuration value from the code package and returns it as a string. The caller is responsible for freeing the string.

**SRS_CONFIGURATION_READER_42_023: [** If `activation_context` is `NULL` then `configuration_reader_get_char_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_024: [** If `config_package_name` is `NULL` or empty then `configuration_reader_get_char_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_025: [** If `section_name` is `NULL` or empty then `configuration_reader_get_char_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_026: [** If `parameter_name` is `NULL` or empty then `configuration_reader_get_char_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_027: [** If `value` is `NULL` then `configuration_reader_get_char_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_028: [** `configuration_reader_get_char_string` shall call the `GetConfigurationPackage` function on `activation_context` with `config_package_name`. **]**

**SRS_CONFIGURATION_READER_42_029: [** `configuration_reader_get_char_string` shall call `GetValue` on the configuration package with `section_name` and `parameter_name`. **]**

**SRS_CONFIGURATION_READER_42_030: [** `configuration_reader_get_char_string` shall convert the value from a wide-character string to narrow-character string and store it in `value`. **]**

**SRS_CONFIGURATION_READER_42_031: [** If there are any other failures then `configuration_reader_get_char_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_032: [** `configuration_reader_get_char_string` shall succeed and return 0. **]**

### configuration_reader_get_thandle_rc_string

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_thandle_rc_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, THANDLE(RC_STRING)*, value)(0, MU_FAILURE);
```

`configuration_reader_get_thandle_rc_string` reads a configuration value from the code package and returns it as a `THANDLE(RC_STRING)`.

**SRS_CONFIGURATION_READER_42_043: [** If `activation_context` is `NULL` then `configuration_reader_get_thandle_rc_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_044: [** If `config_package_name` is `NULL` or empty then `configuration_reader_get_thandle_rc_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_045: [** If `section_name` is `NULL` or empty then `configuration_reader_get_thandle_rc_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_046: [** If `parameter_name` is `NULL` or empty then `configuration_reader_get_thandle_rc_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_047: [** If `value` is `NULL` then `configuration_reader_get_thandle_rc_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_048: [** `configuration_reader_get_thandle_rc_string` shall call the `GetConfigurationPackage` function on `activation_context` with `config_package_name`. **]**

**SRS_CONFIGURATION_READER_42_049: [** `configuration_reader_get_thandle_rc_string` shall call `GetValue` on the configuration package with `section_name` and `parameter_name`. **]**

**SRS_CONFIGURATION_READER_42_050: [** `configuration_reader_get_thandle_rc_string` shall convert the value from a wide-character string to narrow-character string. **]**

**SRS_CONFIGURATION_READER_42_051: [** `configuration_reader_get_thandle_rc_string` shall store the converted string in a `THANDLE(RC_STRING)`. **]**

**SRS_CONFIGURATION_READER_42_052: [** If there are any other failures then `configuration_reader_get_thandle_rc_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_053: [** `configuration_reader_get_thandle_rc_string` shall succeed and return 0. **]**

### configuration_reader_get_wchar_string

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_wchar_string, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, wchar_t**, value)(0, MU_FAILURE);
```

`configuration_reader_get_wchar_string` reads a configuration value from the code package and returns it as a wide-character string. The caller is responsible for freeing the string.

**SRS_CONFIGURATION_READER_42_033: [** If `activation_context` is `NULL` then `configuration_reader_get_wchar_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_034: [** If `config_package_name` is `NULL` or empty then `configuration_reader_get_wchar_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_035: [** If `section_name` is `NULL` or empty then `configuration_reader_get_wchar_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_036: [** If `parameter_name` is `NULL` or empty then `configuration_reader_get_wchar_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_037: [** If `value` is `NULL` then `configuration_reader_get_wchar_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_038: [** `configuration_reader_get_wchar_string` shall call the `GetConfigurationPackage` function on `activation_context` with `config_package_name`. **]**

**SRS_CONFIGURATION_READER_42_039: [** `configuration_reader_get_wchar_string` shall call `GetValue` on the configuration package with `section_name` and `parameter_name`. **]**

**SRS_CONFIGURATION_READER_42_040: [** `configuration_reader_get_wchar_string` shall copy the string and store it in `value`. **]**

**SRS_CONFIGURATION_READER_42_041: [** If there are any other failures then `configuration_reader_get_wchar_string` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_42_042: [** `configuration_reader_get_wchar_string` shall succeed and return 0. **]**


### configuration_reader_get_bool

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, int, configuration_reader_get_bool, IFabricCodePackageActivationContext*, activation_context, const wchar_t*, config_package_name, const wchar_t*, section_name, const wchar_t*, parameter_name, bool*, value)(0, MU_FAILURE);
```

`configuration_reader_get_bool` reads a configuration value from the code package and returns it as `bool`.

**SRS_CONFIGURATION_READER_03_001: [** If `activation_context` is `NULL` then `configuration_reader_get_bool` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_03_002: [** If `config_package_name` is `NULL` or empty then `configuration_reader_get_bool` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_03_003: [** If `section_name` is `NULL` or empty then `configuration_reader_get_bool` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_03_004: [** If `parameter_name` is `NULL` or empty then `configuration_reader_get_bool` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_03_005: [** If `value` is `NULL` then `configuration_reader_get_bool` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_03_006: [** `configuration_reader_get_bool` shall call the `GetConfigurationPackage` function on `activation_context` with `config_package_name`. **]**

**SRS_CONFIGURATION_READER_03_007: [** `configuration_reader_get_bool` shall call `GetValue` on the configuration package with `section_name` and `parameter_name`. **]**

**SRS_CONFIGURATION_READER_11_001: [** `configuration_reader_get_bool` shall do a case insensitive comparison of the string. **]**

**SRS_CONFIGURATION_READER_03_009: [** If the string is `False`, `configuration_reader_get_bool` shall set `value` to `false` and return 0. **]**

**SRS_CONFIGURATION_READER_03_010: [** If the string is `True`, `configuration_reader_get_bool` shall set `value` to `true` and return 0. **]**

**SRS_CONFIGURATION_READER_03_014: [** If the string is an empty string, `configuration_reader_get_bool` shall set `value` to `false` and return 0. **]**

**SRS_CONFIGURATION_READER_03_013: [** If the string is anything other than the above, `configuration_reader_get_bool` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_03_011: [** If there are any other failures then `configuration_reader_get_bool` shall fail and return a non-zero value. **]**

**SRS_CONFIGURATION_READER_03_012: [** `configuration_reader_get_bool` shall succeed and return 0. **]**

