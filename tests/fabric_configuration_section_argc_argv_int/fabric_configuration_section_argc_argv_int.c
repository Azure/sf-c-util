// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fabric_configuration_section_argc_argv.h"

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

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV_with_0_parameters)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER_LIST param_list =
    {
        .Count = 0,
        .Items = NULL
    };
    FABRIC_CONFIGURATION_SECTION input =
    {
        .Name = L"A",
        .Reserved = NULL,
        .Parameters = &param_list
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV(&input, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 2, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv[1]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV_with_1_parameters)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER params[] =
    {
        [0] = 
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"param1",
            .Value = L"value1"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST param_list =
    {
        .Count = sizeof(params)/sizeof(params[0]),
        .Items = params
    };
    FABRIC_CONFIGURATION_SECTION input =
    {
        .Name = L"A",
        .Reserved = NULL,
        .Parameters = &param_list
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV(&input, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 4, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "param1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "value1", argv[3]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV_with_2_parameters)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER params[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"param1",
            .Value = L"value1"
        },
        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"param2",
            .Value = L"value2"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST param_list =
    {
        .Count = sizeof(params) / sizeof(params[0]),
        .Items = params
    };
    FABRIC_CONFIGURATION_SECTION input =
    {
        .Name = L"A",
        .Reserved = NULL,
        .Parameters = &param_list
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_to_ARGC_ARGV(&input, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 6, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, SECTION_NAME_DEFINE, argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "param1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "value1", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "param2", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "value2", argv[5]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
