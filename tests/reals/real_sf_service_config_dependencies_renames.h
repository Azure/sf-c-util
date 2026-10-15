// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/*DEFINE_SF_SERVICE_CONFIG generates its code in the consumer's translation unit, so sf_c_util cannot rename it.
This header renames every function that generated code calls. real_sf_service_config.h shows how a consumer builds the real of its configuration.*/

#include "real_gballoc_hl_renames.h" // IWYU pragma: keep
#include "real_interlocked_renames.h" // IWYU pragma: keep
#include "real_srw_lock_ll_renames.h" // IWYU pragma: keep
#include "real_rc_string_renames.h" // IWYU pragma: keep

#include "real_configuration_reader_renames.h" // IWYU pragma: keep
