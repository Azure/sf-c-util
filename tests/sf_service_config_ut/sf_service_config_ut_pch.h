// Copyright (c) Microsoft. All rights reserved.

// Precompiled header for sf_service_config_ut

#ifndef SF_SERVICE_CONFIG_UT_PCH_H
#define SF_SERVICE_CONFIG_UT_PCH_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

#include "fabricruntime.h"
#include "fabrictypes.h"

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_stdint.h"
#include "umock_c/umocktypes_charptr.h"
#include "umock_c/umocktypes_wcharptr.h"
#include "umock_c/umock_c_negative_tests.h"

#include "umock_c/umock_c_ENABLE_MOCKS.h" // ============================== ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"
#include "c_pal/srw_lock_ll.h"
#include "c_util/rc_string.h"
#include "c_pal/thandle.h"
#include "sf_c_util/configuration_reader.h"
#include "umock_c/umock_c_DISABLE_MOCKS.h" // ============================== DISABLE_MOCKS

// Must include umock_c_prod so mocks are not expanded in real_rc_string
#include "umock_c/umock_c_prod.h"

#include "real_gballoc_hl.h"
#include "real_rc_string.h"
#include "real_string_utils.h"
#include "c_util_test_helpers/rc_string_test_decl.h"
#include "c_util_test_helpers/rc_string_test_type.h"

#include "sf_service_config_ut_helpers.h"
#include "test_sf_service_config.h"

#include "sf_c_util/sf_service_config.h"

#endif // SF_SERVICE_CONFIG_UT_PCH_H
