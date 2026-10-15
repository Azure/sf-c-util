// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Precompiled header for real_sf_service_config_ut

#ifndef REAL_SF_SERVICE_CONFIG_UT_PCH_H
#define REAL_SF_SERVICE_CONFIG_UT_PCH_H

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

#include "windows.h"
#include "fabricruntime.h"

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_bool.h"
#include "umock_c/umocktypes_windows.h"

#include "c_logging/logger.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

#include "umock_c/umock_c_ENABLE_MOCKS.h" // ============================== ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/interlocked.h"
#include "c_pal/srw_lock_ll.h"
#include "c_pal/thandle.h"
#include "c_util/rc_string.h"
#include "sf_c_util/fc_package.h"
#include "sf_c_util/fabric_string_list_result.h"
#include "sf_c_util/fc_activation_context.h"
#include "sf_c_util/configuration_reader.h"
#include "sample_service_config.h"
#include "sample_config_consumer.h"
#include "umock_c/umock_c_DISABLE_MOCKS.h" // ============================== DISABLE_MOCKS

#include "com_wrapper/com_wrapper.h"

#include "sf_c_util/fc_activation_context_com.h"

#include "real_gballoc_hl.h"
#include "real_interlocked.h"
#include "real_srw_lock_ll.h"
#include "real_rc_string.h"

#include "real_fc_package.h"
#include "real_fabric_string_list_result.h"
#include "real_fc_activation_context.h"
#include "real_fc_activation_context_com.h"
#include "real_configuration_reader.h"
#include "real_sf_service_config.h"

DECLARE_REAL_SF_SERVICE_CONFIG(sample_service, SAMPLE_SERVICE_PARAMS)

THANDLE(SF_SERVICE_CONFIG(sample_service)) real_sample_config_consumer_create(int argc, char** argv);

#endif // REAL_SF_SERVICE_CONFIG_UT_PCH_H
