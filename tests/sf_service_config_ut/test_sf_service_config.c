// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/sf_service_config.h"

#include "test_sf_service_config.h"

// Avoid mock calls from the THANDLE (no other way to do this right now)
#include "real_interlocked_renames.h"
#include "real_interlocked.h"

DEFINE_SF_SERVICE_CONFIG(my_config, L"default_config", L"MyConfigSectionName", MY_CONFIG_TEST_PARAMS);
