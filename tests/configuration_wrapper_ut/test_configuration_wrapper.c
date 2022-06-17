// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/configuration_wrapper.h"

#include "test_configuration_wrapper.h"

// Avoid mock calls from the THANDLE (no other way to do this right now)
#include "real_interlocked_renames.h"
#include "real_interlocked.h"

DEFINE_CONFIGURATION_WRAPPER(my_config, L"default_config", L"MyConfigSectionName", MY_CONFIG_TEST_PARAMS);
