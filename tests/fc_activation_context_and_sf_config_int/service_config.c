// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/thandle.h"

#include "sf_c_util/sf_service_config.h"
#include "service_config.h"

DEFINE_SF_SERVICE_CONFIG(my_mocked_config, SF_CONFIG_NAME_WIDE_DEFINE, SF_SECTION_NAME_WIDE_DEFINE, MY_MOCKED_CONFIG_TEST_PARAMS);
