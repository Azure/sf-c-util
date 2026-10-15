// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/sf_service_config.h"

#include "sample_service_config.h"

DEFINE_SF_SERVICE_CONFIG(sample_service, MU_C2(L, SAMPLE_SERVICE_CONFIG_PACKAGE_NAME), MU_C2(L, SAMPLE_SERVICE_SECTION_NAME), SAMPLE_SERVICE_PARAMS);
