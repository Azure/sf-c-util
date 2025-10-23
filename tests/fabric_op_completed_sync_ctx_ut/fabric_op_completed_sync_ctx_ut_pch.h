// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Precompiled header for fabric_op_completed_sync_ctx_ut

#ifndef FABRIC_OP_COMPLETED_SYNC_CTX_UT_PCH_H
#define FABRIC_OP_COMPLETED_SYNC_CTX_UT_PCH_H

#include <stdlib.h>
#include <stdint.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umock_c_negative_tests.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_stdint.h"
#include "c_logging/logger.h"

#include "fabriccommon.h"
#include "unknwn.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#include "umock_c/umock_c_ENABLE_MOCKS.h" // ============================== ENABLE_MOCKS

#include "c_pal/gballoc_hl.h"

#define GBALLOC_HL_REDIRECT_H
#include "test_callback.h"
#include "test_callback_com.h"
#include "com_wrapper/com_wrapper.h"
#undef GBALLOC_HL_REDIRECT_H

#include "c_pal/gballoc_hl_redirect.h"
#include "umock_c/umock_c_DISABLE_MOCKS.h" // ============================== DISABLE_MOCKS

#include "real_gballoc_hl.h"

#include "sf_c_util/fabric_op_completed_sync_ctx.h"

#endif // FABRIC_OP_COMPLETED_SYNC_CTX_UT_PCH_H
