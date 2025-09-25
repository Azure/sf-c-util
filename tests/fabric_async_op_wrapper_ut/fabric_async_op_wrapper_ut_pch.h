// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Precompiled header for fabric_async_op_wrapper_ut

#include <stdlib.h>
#include <stdint.h>


#include "fabriccommon.h"

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_stdint.h"
#include "umock_c/umock_c_negative_tests.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#define ENABLE_MOCKS

#include "c_pal/gballoc_hl.h"

#define GBALLOC_HL_REDIRECT_H
#include "sf_c_util/fabric_async_op_cb.h"
#include "com_wrapper/com_wrapper.h"
#include "sf_c_util/fabric_async_op_cb_com.h"
#include "test_fabric_async_operation.h"
#include "test_fabric_async_operation_com.h"
#include "test_async_operation_context.h"
#include "test_async_operation_context_com.h"
#undef GBALLOC_HL_REDIRECT_H

#include "c_pal/gballoc_hl_redirect.h"
#undef ENABLE_MOCKS

#include "real_gballoc_hl.h"


#include "test_fabric_async_operation_wrapper.h"
