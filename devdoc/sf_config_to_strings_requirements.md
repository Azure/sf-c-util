`sf_config_to_strings` requirements
================

## Overview

`sf_config_to_strings` is a module that collects all of the necessary service fabric configuration values as strings that can be accessed via getters.

The pointers returned are only valid while the configuration handle exists.

The data exposed by this module is:
 - `node_name`
 - `partition_id` (string)
 - `replica_id` (string)
 - `code_package_path`

## Exposed API

```c
typedef struct SF_CONFIG_TO_STRINGS_TAG* SF_CONFIG_TO_STRINGS_HANDLE;

MOCKABLE_FUNCTION(, SF_CONFIG_TO_STRINGS_HANDLE, sf_config_to_strings_create, IFabricCodePackageActivationContext*, activation_context, const FABRIC_PARTITION_ID*, fabric_partition_id, FABRIC_REPLICA_ID, fabric_replica_id, THANDLE(SF_SERVICE_CONFIG(bs2sf_configuration)), config);
MOCKABLE_FUNCTION(, void, sf_config_to_strings_destroy, SF_CONFIG_TO_STRINGS_HANDLE, handle);

MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_node_name, SF_CONFIG_TO_STRINGS_HANDLE, handle);
MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_partition_id, SF_CONFIG_TO_STRINGS_HANDLE, handle);
MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_replica_id, SF_CONFIG_TO_STRINGS_HANDLE, handle);
MOCKABLE_FUNCTION(, THANDLE(RC_STRING), sf_config_to_strings_get_code_package_path, SF_CONFIG_TO_STRINGS_HANDLE, handle);
```

### sf_config_to_strings_create

```c
MOCKABLE_FUNCTION(, SF_CONFIG_TO_STRINGS_HANDLE, sf_config_to_strings_create, IFabricCodePackageActivationContext*, activation_context, const FABRIC_PARTITION_ID*, fabric_partition_id, FABRIC_REPLICA_ID, fabric_replica_id, THANDLE(SF_SERVICE_CONFIG(bs2sf_configuration)), config);
```

`sf_config_to_strings_create` converts the values to strings or copies the strings to its own memory.

**SRS_SF_CONFIG_TO_STRINGS_42_001: [** If `activation_context` is `NULL` then `sf_config_to_strings_create` shall fail and return `NULL`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_063: [** If `fabric_partition_id` is `NULL` then `sf_config_to_strings_create` shall fail and return `NULL`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_002: [** `sf_config_to_strings_create` shall allocate memory for the `SF_CONFIG_TO_STRINGS_HANDLE`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_064: [** `sf_config_to_strings_create` shall call `UuidToStringA` to convert the `fabric_partition_id` to a string. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_082: [** `sf_config_to_strings_create` shall copy the result of `UuidToStringA` to a string. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_027: [** `sf_config_to_strings_create` shall call `FabricGetNodeContext` to get the fabric node context result. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_028: [** `sf_config_to_strings_create` shall call `get_NodeContext` on the fabric node context result to get the fabric node context. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_029: [** `sf_config_to_strings_create` shall copy the `NodeName` from the fabric node context. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_068: [** `sf_config_to_strings_create` shall call `GetCodePackage` on the `activation_context` to get the `code_package`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_069: [** `sf_config_to_strings_create` shall call `get_Path` on the `code_package_path` and copy the string. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_074: [** `sf_config_to_strings_create` shall generate a string from the `fabric_replica_id` and store it as `replica_id`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_030: [** If there are any other failures then `sf_config_to_strings_create` shall fail and return `NULL`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_031: [** `sf_config_to_strings_create` shall return a handle to the configuration. **]**

### sf_config_to_strings_destroy

```c
MOCKABLE_FUNCTION(, void, sf_config_to_strings_destroy, SF_CONFIG_TO_STRINGS_HANDLE, handle);
```

`sf_config_to_strings_destroy` frees all of the data that was loaded.

**SRS_SF_CONFIG_TO_STRINGS_42_032: [** If `handle` is `NULL` then `sf_config_to_strings_destroy` shall fail and return. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_033: [** `sf_config_to_strings_destroy` shall free all of the strings. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_034: [** `sf_config_to_strings_destroy` shall free the memory that was allocated to store the configuration. **]**

### sf_config_to_strings_get_node_name

```c
MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_node_name, SF_CONFIG_TO_STRINGS_HANDLE, handle);
```

`sf_config_to_strings_get_node_name` is a getter for the `node_name` that was loaded.

**SRS_SF_CONFIG_TO_STRINGS_42_059: [** If `handle` is `NULL` then `sf_config_to_strings_get_node_name` shall fail and return `NULL`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_060: [** `sf_config_to_strings_get_node_name` shall return the value that was read as `node_name`. **]**

### sf_config_to_strings_get_partition_id

```c
MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_partition_id, SF_CONFIG_TO_STRINGS_HANDLE, handle);
```

`sf_config_to_strings_get_partition_id` is a getter for the `partition_id` string that was copied.

**SRS_SF_CONFIG_TO_STRINGS_42_070: [** If `handle` is `NULL` then `sf_config_to_strings_get_partition_id` shall fail and return `NULL`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_071: [** `sf_config_to_strings_get_partition_id` shall return the `partition_id` string. **]**

### sf_config_to_strings_get_replica_id

```c

MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_replica_id, SF_CONFIG_TO_STRINGS_HANDLE, handle);
```

`sf_config_to_strings_get_replica_id` is a getter for the `replica_id` string that was copied.

**SRS_SF_CONFIG_TO_STRINGS_42_072: [** If `handle` is `NULL` then `sf_config_to_strings_get_replica_id` shall fail and return `NULL`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_073: [** `sf_config_to_strings_get_replica_id` shall return the value that was read as `replica_id`. **]**

### sf_config_to_strings_get_code_package_path

```c
MOCKABLE_FUNCTION(, THANDLE(RC_STRING), sf_config_to_strings_get_code_package_path, SF_CONFIG_TO_STRINGS_HANDLE, handle);
```

`sf_config_to_strings_get_code_package_path` is a getter for the `code_package_path` that was read.

**SRS_SF_CONFIG_TO_STRINGS_42_075: [** If `handle` is `NULL` then `sf_config_to_strings_get_code_package_path` shall fail and return `NULL`. **]**

**SRS_SF_CONFIG_TO_STRINGS_42_076: [** `sf_config_to_strings_get_code_package_path` shall return the value that was read as `code_package_path`. **]**


