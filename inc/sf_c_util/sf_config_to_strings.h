// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef SF_CONFIG_TO_STRINGS_H
#define SF_CONFIG_TO_STRINGS_H


#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#include <wchar.h>
#endif

#include "fabricruntime.h"
#include "fabrictypes.h"

#include "c_util/rc_string.h"
#include "c_pal/thandle.h"

#include "umock_c/umock_c_prod.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SF_CONFIG_TO_STRINGS_TAG* SF_CONFIG_TO_STRINGS_HANDLE;

MOCKABLE_FUNCTION(, SF_CONFIG_TO_STRINGS_HANDLE, sf_config_to_strings_create, IFabricCodePackageActivationContext*, activation_context, const FABRIC_PARTITION_ID*, fabric_partition_id, FABRIC_REPLICA_ID, fabric_replica_id); // , THANDLE(SF_SERVICE_CONFIG(bs2sf_configuration)), config);
MOCKABLE_FUNCTION(, void, sf_config_to_strings_destroy, SF_CONFIG_TO_STRINGS_HANDLE, handle);

MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_node_name, SF_CONFIG_TO_STRINGS_HANDLE, handle);
MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_partition_id, SF_CONFIG_TO_STRINGS_HANDLE, handle);
MOCKABLE_FUNCTION(, const char*, sf_config_to_strings_get_replica_id, SF_CONFIG_TO_STRINGS_HANDLE, handle);
MOCKABLE_FUNCTION(, THANDLE(RC_STRING), sf_config_to_strings_get_code_package_path, SF_CONFIG_TO_STRINGS_HANDLE, handle);


#ifdef __cplusplus
}
#endif

#endif /* SF_CONFIG_TO_STRINGS_H */
