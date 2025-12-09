// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Precompiled header for configuration_package_change_handler_ut

#ifndef CONFIGURATION_PACKAGE_CHANGE_HANDLER_UT_PCH_H
#define CONFIGURATION_PACKAGE_CHANGE_HANDLER_UT_PCH_H

#include <stdlib.h>

#include "windows.h"

#include "fabricruntime.h"

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h*/

#include "umock_c/umock_c_ENABLE_MOCKS.h" // ============================== ENABLE_MOCKS

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "com_wrapper/com_wrapper.h"

#include "umock_c/umock_c_DISABLE_MOCKS.h" // ============================== DISABLE_MOCKS

#include "real_gballoc_hl.h"

#include "sf_c_util/configuration_package_change_handler.h"

#endif // CONFIGURATION_PACKAGE_CHANGE_HANDLER_UT_PCH_H
