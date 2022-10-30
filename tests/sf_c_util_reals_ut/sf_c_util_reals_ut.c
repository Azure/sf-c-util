// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>                   // for NULL

#include "testrunnerswitcher.h"

#define REGISTER_GLOBAL_MOCK_HOOK(original, real) \
    (original == real) ? (void)0 : (void)1;

#define REGISTER_GLOBAL_MOCK_FAIL_RETURN(X, return_value) \
    (X == NULL) ? (void)0 : (void)1;


#include "real_hresult_to_string.h"
#include "real_fc_parameter_argc_argv.h"
#include "real_fc_parameter_list_argc_argv.h"
#include "real_fc_section_argc_argv.h"
#include "real_fc_section_list_argc_argv.h"
#include "real_common_argc_argv.h"

#include "sf_c_util/hresult_to_string.h"
#include "sf_c_util/fc_parameter_argc_argv.h"
#include "sf_c_util/fc_parameter_list_argc_argv.h"
#include "sf_c_util/fc_section_argc_argv.h"
#include "sf_c_util/fc_section_list_argc_argv.h"
#include "sf_c_util/common_argc_argv.h"

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

// this test makes sure that the mappings work
// (there is a real_ function corresponding to the original)
TEST_FUNCTION(check_sf_c_util_reals)
{
    // arrange
    // act
    REGISTER_HRESULT_TO_STRING_GLOBAL_MOCK_HOOK();
    REGISTER_FABRIC_CONFIGURATION_PARAMETER_ARGC_ARGV_GLOBAL_MOCK_HOOK();
    REGISTER_FABRIC_CONFIGURATION_PARAMETER_LIST_ARGC_ARGV_GLOBAL_MOCK_HOOK();
    REGISTER_FABRIC_CONFIGURATION_SECTION_ARGC_ARGV_GLOBAL_MOCK_HOOK();
    REGISTER_FABRIC_CONFIGURATION_SECTION_LIST_ARGC_ARGV_GLOBAL_MOCK_HOOK();
    REGISTER_COMMON_ARGC_ARGV_GLOBAL_MOCK_HOOK();

    // assert
    // no explicit assert, if it builds it works
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
