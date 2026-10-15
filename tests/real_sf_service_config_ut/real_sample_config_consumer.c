// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "real_gballoc_hl_renames.h" // IWYU pragma: keep
#include "real_interlocked_renames.h" // IWYU pragma: keep

#include "real_fc_activation_context_renames.h" // IWYU pragma: keep
#include "real_fc_activation_context_com_renames.h" // IWYU pragma: keep

#include "real_sample_service_config_renames.h" // IWYU pragma: keep

#define sample_config_consumer_create real_sample_config_consumer_create

#include "sample_config_consumer.c"
