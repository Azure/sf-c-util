// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SAMPLE_CONFIG_CONSUMER_H
#define SAMPLE_CONFIG_CONSUMER_H

#include "c_pal/thandle.h"

#include "sample_service_config.h"

#include "umock_c/umock_c_prod.h"

MOCKABLE_FUNCTION(, THANDLE(SF_SERVICE_CONFIG(sample_service)), sample_config_consumer_create, int, argc, char**, argv);

#endif /* SAMPLE_CONFIG_CONSUMER_H */
