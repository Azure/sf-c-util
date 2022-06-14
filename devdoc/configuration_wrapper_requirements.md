# `configuration_wrapper` requirements

## Overview

`configuration_wrapper` is a series of macros which can be used to generate code to read configurations from Service Fabric and expose the values as getters.

The following types are supported:
 - `bool` (must be the string "True" for `true` or "False" or empty for `false`)
 - `uint32_t`
 - `uint64_t`
 - `char*` (`char_ptr`)
 - `wchar_t*` (`wchar_ptr`)
 - `THANDLE(RC_STRING)` (`thandle_rc_string`)

Any of the string types may be required (must be present in the config or create will fail) or optional (`NULL` or empty strings are allowed). Integer and bool types do not behave differently for optional and required.

## Example Usage

```c
// In header:
#define MY_CONFIGURATION_PARAMETER_NAME_parameter_1 L"ParameterName1"
#define MY_CONFIGURATION_PARAMETER_NAME_other_option L"OtherOption"
#define MY_CONFIGURATION_PARAMETER_NAME_foo L"FooNameInXML"

#define MY_CONFIGURATION_VALUES \
    CONFIG_REQUIRED(uint64_t, parameter_1), \
    CONFIG_REQUIRED(bool, parameter_2), \
    CONFIG_REQUIRED(thandle_rc_string, other_option), \
    CONFIG_OPTIONAL(thandle_rc_string, foo) \

DECLARE_CONFIGURATION_WRAPPER(MY, MY_CONFIGURATION_VALUES)

// In source file:
DEFINE_CONFIGURATION_WRAPPER(MY, L"default_config", L"MyParameterSection", MY_CONFIGURATION_VALUES)
```

SF Application Manifest

```xml
<?xml version="1.0" encoding="utf-8"?>
<ApplicationManifest ApplicationTypeName="my_application_type_name" ApplicationTypeVersion="1.42" xmlns="http://schemas.microsoft.com/2011/01/fabric" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <Parameters>
    <Parameter Name="ParameterName1Value" DefaultValue="42" />
    <Parameter Name="ParameterName2Value" DefaultValue="True" />
    <Parameter Name="OtherOptionValue" DefaultValue="some string" />
    <Parameter Name="FooNameInXMLValue" DefaultValue="" />
  </Parameters>
  <ServiceManifestImport>
    <ServiceManifestRef ServiceManifestName="MyServiceManifestName" ServiceManifestVersion="1.42" />
    <ConfigOverrides>
      <ConfigOverride Name="default_config">
        <Settings>
          <Section Name="MyParameterSection">
            <Parameter Name="ParameterName1" Value="[ParameterName1Value]" />
            <Parameter Name="ParameterName2" Value="[ParameterName2Value]" />
            <Parameter Name="OtherOption" Value="[OtherOptionValueValue]" />
            <Parameter Name="FooNameInXML" Value="[FooNameInXMLValue]" />
          </Section>
        </Settings>
      </ConfigOverride>
    </ConfigOverrides>
  </ServiceManifestImport>
</ApplicationManifest>
```

SF Config XML

```xml
<?xml version="1.0" encoding="utf-8"?>
<Settings xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://schemas.microsoft.com/2011/01/fabric">
   <Section Name="MyParameterSection">
      <Parameter Name="ParameterName1" Value="42" />
      <Parameter Name="ParameterName2" Value="True" />
      <Parameter Name="OtherOption" Value="some string" />
      <Parameter Name="FooNameInXML" Value="" />
    </Section>
</Settings>
```

## Exposed API

```c
#define CONFIGURATION_WRAPPER(name) MU_C2(name, _CONFIGURATION)
#define CONFIGURATION_WRAPPER_CREATE(name) MU_C2(name, _configuration_create)
#define CONFIGURATION_WRAPPER_GETTER(name, param) MU_C3(name, _configuration_get_, param)

#define DECLARE_CONFIGURATION_WRAPPER(name, ...) \
    //...

#define DEFINE_CONFIGURATION_WRAPPER(name, sf_config_name, sf_parameters_section_name, ...) \
    //...
```

### DECLARE_CONFIGURATION_WRAPPER

```c
#define DECLARE_CONFIGURATION_WRAPPER(name, ...)
```

Creates all declarations for the configuration type `CONFIGURATION_WRAPPER(name)`. This includes the mockable create, which produces a `THANDLE`, and all of the mockable getters for the parameters specified.

Each parameter must be in the form `CONFIG_REQUIRED(type, config_name)` or `CONFIG_OPTIONAL(type, config_name)`, for configurations that are required or optional, respectively. Required configs must be found in the SF configuration or the configuration create will fail, optional configs may be absent and a default (e.g. empty string) will be used.

Each of these parameters with name `config_name` must also have a corresponding `CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name` defined with a wide string for the name of the config value as specified in the SF XML.

**SRS_CONFIGURATION_WRAPPER_42_001: [** `DECLARE_CONFIGURATION_WRAPPER` shall generate a `THANDLE` declaration of type `CONFIGURATION_WRAPPER(name)`. **]**

**SRS_CONFIGURATION_WRAPPER_42_002: [** `DECLARE_CONFIGURATION_WRAPPER` shall generate a mockable create function `CONFIGURATION_WRAPPER_CREATE(name)` which takes an `IFabricCodePackageActivationContext*` and produces the `THANDLE`. **]**

**SRS_CONFIGURATION_WRAPPER_42_003: [** `DECLARE_CONFIGURATION_WRAPPER` shall generate mockable getter functions `CONFIGURATION_WRAPPER_GETTER(name, param)` for each of the configurations provided. **]**

### DEFINE_CONFIGURATION_WRAPPER

```c
#define DEFINE_CONFIGURATION_WRAPPER(name, sf_config_name, sf_parameters_section_name, ...)
```

Creates the implementation of the configuration wrapper for the type `CONFIGURATION_WRAPPER(name)`. This includes the underlying struct which will hold all of the configuration values specified.

The `sf_config_name` argument must be a wide string for the configuration values to read from Service Fabric. This is passed to `configuration_reader` as `config_package_name`.

The `sf_parameters_section_name` argument must be a wide string for the section name holding the configuration to read from Service Fabric. This is passed to `configuration_reader` as `section_name`.

**SRS_CONFIGURATION_WRAPPER_42_004: [** `DEFINE_CONFIGURATION_WRAPPER` shall generate the `CONFIGURATION_WRAPPER(name)` struct. **]**

**SRS_CONFIGURATION_WRAPPER_42_005: [** `DEFINE_CONFIGURATION_WRAPPER` shall generate the implementation of `CONFIGURATION_WRAPPER_CREATE(name)`. **]**

**SRS_CONFIGURATION_WRAPPER_42_006: [** `DECLARE_CONFIGURATION_WRAPPER` shall generate the implementation of the getter functions `CONFIGURATION_WRAPPER_GETTER(name, param)` for each of the configurations provided. **]**

### CONFIGURATION_WRAPPER

```c
#define CONFIGURATION_WRAPPER(name) MU_C2(name, _CONFIGURATION)
```

Get the name of the configuration type, e.g. `MY_CONFIGURATION`.

**SRS_CONFIGURATION_WRAPPER_42_007: [** `CONFIGURATION_WRAPPER` shall expand to the name of the configuration module by appending the suffix `_CONFIGURATION`. **]**

### CONFIGURATION_WRAPPER_CREATE

```c
#define CONFIGURATION_WRAPPER_CREATE(name) MU_C2(name, _configuration_create)
```

Get the name of the function to create the configuration object, e.g. `MY_configuration_create`.

**SRS_CONFIGURATION_WRAPPER_42_008: [** `CONFIGURATION_WRAPPER` shall expand to the name of the create function for the configuration module by appending the suffix `_configuration_create`. **]**

```c
THANDLE(CONFIGURATION_WRAPPER(name)) CONFIGURATION_WRAPPER_CREATE(name)(IFabricCodePackageActivationContext* activation_context)
```

**SRS_CONFIGURATION_WRAPPER_42_009: [** If `activation_context` is `NULL` then `CONFIGURATION_WRAPPER_CREATE(name)` shall fail and return `NULL`. **]**

**SRS_CONFIGURATION_WRAPPER_42_010: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall allocate the `THANDLE(CONFIGURATION_WRAPPER(name))` with `MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)` as the dispose function. **]**

**SRS_CONFIGURATION_WRAPPER_42_011: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall call `AddRef` and store the `activation_context`. **]**

**SRS_CONFIGURATION_WRAPPER_42_012: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall store the `sf_config_name` and `sf_parameters_section_name`. **]**

**SRS_CONFIGURATION_WRAPPER_42_013: [** For each configuration value with name `config_name`: **]**

 - **SRS_CONFIGURATION_WRAPPER_42_014: [** If the type is `bool` then: **]**

   - **SRS_CONFIGURATION_WRAPPER_42_015: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall call `configuration_reader_get_bool` with the `activation_context`, `sf_config_name`, `sf_parameters_section_name`, and `CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name`. **]**

 - **SRS_CONFIGURATION_WRAPPER_42_016: [** If the type is `uint32_t` then: **]**

   - **SRS_CONFIGURATION_WRAPPER_42_017: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall call `configuration_reader_get_uint32_t` with the `activation_context`, `sf_config_name`, `sf_parameters_section_name`, and `CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_018: [** If the result is `UINT32_MAX` then `CONFIGURATION_WRAPPER_CREATE(name)` shall fail and return `NULL`. **]**

 - **SRS_CONFIGURATION_WRAPPER_42_019: [** If the type is `uint64_t` then: **]**

   - **SRS_CONFIGURATION_WRAPPER_42_020: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall call `configuration_reader_get_uint64_t` with the `activation_context`, `sf_config_name`, `sf_parameters_section_name`, and `CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_021: [** If the result is `UINT64_MAX` then `CONFIGURATION_WRAPPER_CREATE(name)` shall fail and return `NULL`. **]**

 - **SRS_CONFIGURATION_WRAPPER_42_022: [** If the type is `char_ptr` then: **]**

   - **SRS_CONFIGURATION_WRAPPER_42_023: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall call `configuration_reader_get_char_string` with the `activation_context`, `sf_config_name`, `sf_parameters_section_name`, and `CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_024: [** If the value is an empty string then `CONFIGURATION_WRAPPER_CREATE(name)` shall free the string and set it to `NULL`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_025: [** If the configuration value is `CONFIG_REQUIRED` and the value is `NULL` then `CONFIGURATION_WRAPPER_CREATE(name)` shall fail and return `NULL`. **]**

 - **SRS_CONFIGURATION_WRAPPER_42_026: [** If the type is `wchar_ptr` then: **]**

   - **SRS_CONFIGURATION_WRAPPER_42_027: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall call `configuration_reader_get_wchar_string` with the `activation_context`, `sf_config_name`, `sf_parameters_section_name`, and `CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_028: [** If the value is an empty string then `CONFIGURATION_WRAPPER_CREATE(name)` shall free the string and set it to `NULL`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_029: [** If the configuration value is `CONFIG_REQUIRED` and the value is `NULL` then `CONFIGURATION_WRAPPER_CREATE(name)` shall fail and return `NULL`. **]**

 - **SRS_CONFIGURATION_WRAPPER_42_030: [** If the type is `thandle_rc_string` then: **]**

   - **SRS_CONFIGURATION_WRAPPER_42_031: [** `CONFIGURATION_WRAPPER_CREATE(name)` shall call `configuration_reader_get_thandle_rc_string` with the `activation_context`, `sf_config_name`, `sf_parameters_section_name`, and `CONFIGURATION_WRAPPER_PARAMETER_NAME_config_name`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_032: [** If the value is an empty string then `CONFIGURATION_WRAPPER_CREATE(name)` shall free the string and set it to `NULL`. **]**

   - **SRS_CONFIGURATION_WRAPPER_42_033: [** If the configuration value is `CONFIG_REQUIRED` and the value is `NULL` then `CONFIGURATION_WRAPPER_CREATE(name)` shall fail and return `NULL`. **]**

**SRS_CONFIGURATION_WRAPPER_42_034: [** If there are any errors then `CONFIGURATION_WRAPPER_CREATE(name)` shall fail and return `NULL`. **]**

### Dispose

```c
static void MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)(CONFIGURATION_WRAPPER(name)* handle)
```

The dispose function is called when the last `THANDLE` reference is released.

**SRS_CONFIGURATION_WRAPPER_42_035: [** For each config value: **]**

 - **SRS_CONFIGURATION_WRAPPER_42_036: [** If the type is `char_ptr` then `MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)` shall free the string. **]**

 - **SRS_CONFIGURATION_WRAPPER_42_038: [** If the type is `wchar_ptr` then `MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)` shall free the string. **]**

 - **SRS_CONFIGURATION_WRAPPER_42_040: [** If the type is `thandle_rc_string` then `MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)` shall assign the `THANDLE` to `NULL`. **]**

**SRS_CONFIGURATION_WRAPPER_42_042: [** `MU_C2A(CONFIGURATION_WRAPPER(name), _dispose)` shall `Release` the `activation_context`. **]**

### CONFIGURATION_WRAPPER_GETTER

```c
#define CONFIGURATION_WRAPPER_GETTER(name, param) MU_C3(name, _configuration_get_, param)
```

Get the name of the function to get a parameter, e.g. `MY_configuration_get_foo`.

**SRS_CONFIGURATION_WRAPPER_42_043: [** `CONFIGURATION_WRAPPER_GETTER` shall expand to the name of the getter function for the configuration module and the given `param` by concatenating the `name`, the string `_configuration_get`, and the `param`. **]**

```c
CONFIGURATION_WRAPPER_RETURN_TYPE(field_type) CONFIGURATION_WRAPPER_GETTER(name, field_name)(THANDLE(CONFIGURATION_WRAPPER(name)) handle)
```

Each getter function returns the value read from the config. The integer values are copied, string values are pointers back into this structure (and thus their lifetime depends on this configuration wrapper), and `thandle_rc_string` results are reference counted.

**SRS_CONFIGURATION_WRAPPER_42_044: [** If `handle` is `NULL` then `CONFIGURATION_WRAPPER_GETTER(name, field_name)` shall fail and return... **]**

 -  **SRS_CONFIGURATION_WRAPPER_42_045: [** ...`false` if the type is `bool` **]**

 -  **SRS_CONFIGURATION_WRAPPER_42_046: [** ...`UINT32_MAX` if the type is `uint32_t` **]**

 -  **SRS_CONFIGURATION_WRAPPER_42_047: [** ...`UINT64_MAX` if the type is `uint64_t` **]**

 -  **SRS_CONFIGURATION_WRAPPER_42_048: [** ...`NULL` otherwise **]**

**SRS_CONFIGURATION_WRAPPER_42_049: [** If the type is `thandle_rc_string` then the returned value will be set using `THANDLE_INITIALIZE` and the caller will have a reference they must free. **]**

**SRS_CONFIGURATION_WRAPPER_42_050: [** `CONFIGURATION_WRAPPER_GETTER(name, field_name)` shall return the configuration value for `field_name`. **]**
