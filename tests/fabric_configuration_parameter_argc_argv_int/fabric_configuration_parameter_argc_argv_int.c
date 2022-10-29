// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fabric_configuration_parameter_argc_argv.h"

TEST_DEFINE_ENUM_TYPE(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

static TEST_MUTEX_HANDLE test_serialize_mutex;

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    TEST_MUTEX_DESTROY(test_serialize_mutex);
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    if (TEST_MUTEX_ACQUIRE(test_serialize_mutex))
    {
        ASSERT_FAIL("Could not acquire test serialization mutex.");
    }
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    TEST_MUTEX_RELEASE(test_serialize_mutex);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV_succeeds)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER p =
    {
        .IsEncrypted = false,
        .MustOverride = false,
        .Reserved = NULL,
        .Name = L"NN",
        .Value = L"VVV"
    };
    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_to_ARGC_ARGV(&p, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 2, argc);
    ASSERT_ARE_EQUAL(char_ptr, "NN", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "VVV", argv[1]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}


TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV_succeeds_1)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER p;
    char* argv[] =
    {
        "AA",
        "BBB"
    };

    int argc_consumed;

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV(sizeof(argv)/sizeof(argv[0]), argv, &p, &argc_consumed);

    ///assert
    ASSERT_IS_TRUE(result== ARGC_ARGV_DATA_OK);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    ASSERT_ARE_EQUAL(wchar_ptr, L"AA", p.Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"BBB", p.Value);
    ASSERT_IS_FALSE(p.IsEncrypted);
    ASSERT_IS_FALSE(p.MustOverride);
    ASSERT_IS_NULL(p.Reserved);

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_free(&p);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV_succeeds_2) /*in this test, 3 argv are passed, however, only 2 are expected to be consumed*/
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER p;
    char* argv[] =
    {
        "AA",
        "BBB",
        "CCCC"
    };

    int argc_consumed;

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV(sizeof(argv) / sizeof(argv[0]), argv, &p, &argc_consumed);

    ///assert
    ASSERT_IS_TRUE(result == ARGC_ARGV_DATA_OK);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    ASSERT_ARE_EQUAL(wchar_ptr, L"AA", p.Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"BBB", p.Value);
    ASSERT_IS_FALSE(p.IsEncrypted);
    ASSERT_IS_FALSE(p.MustOverride);
    ASSERT_IS_NULL(p.Reserved);

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_free(&p);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV_cannot_be_keywords) /*in this test, we're testing that parameters cannot be keywords (as listed in ARGC_ARGV_KEYWORDS_LIST)*/
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER p;
    char* argv[] =
    {
        "AA",
        "BBB",
        "CCCC"
    };

    int argc_consumed;

    ARGC_ARGV_DATA_RESULT result;
    
    for (uint32_t i = 0; i < sizeof(ARGC_ARGV_KEYWORDS_LIST) / sizeof(ARGC_ARGV_KEYWORDS_LIST[0]); i++)
    {
        ///arrange
        argv[0] = (char*)ARGC_ARGV_KEYWORDS_LIST[i];

        ///act
        result = FABRIC_CONFIGURATION_PARAMETER_from_ARGC_ARGV(sizeof(argv) / sizeof(argv[0]), argv, &p, &argc_consumed);

        ///assert
        ASSERT_IS_TRUE(result == ARGC_ARGV_DATA_INVALID);
    }
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
