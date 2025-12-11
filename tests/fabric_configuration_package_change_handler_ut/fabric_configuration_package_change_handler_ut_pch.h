// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Precompiled header for fabric_configuration_package_change_handler_ut

#ifndef FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_UT_PCH_H
#define FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_UT_PCH_H

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h*/

#include "umock_c/umock_c_ENABLE_MOCKS.h" // ============================== ENABLE_MOCKS

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "umock_c/umock_c_DISABLE_MOCKS.h" // ============================== DISABLE_MOCKS

#include "real_gballoc_hl.h"

#include "sf_c_util/fabric_configuration_package_change_handler.h"

#endif // FABRIC_CONFIGURATION_PACKAGE_CHANGE_HANDLER_UT_PCH_H
