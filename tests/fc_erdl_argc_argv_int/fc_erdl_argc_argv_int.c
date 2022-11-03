// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_erdl_argc_argv.h"

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

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV_succeeds_1) /*a list with 0 entries*/
{
    ///arrange
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST dl =
    {
        .Count = 0,
        .Items = NULL
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV(&dl, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 0, argc);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV_succeeds_2) /*a list with 1 entries*/
{
    ///arrange
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION ds[] =
    {
        [0]=
        {
            .Name = L"1",
            .Protocol = L"2",
            .Type = L"3",
            .Port = 4,
            .CertificateName= L"5", 
            .Reserved = NULL
        }
    };
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST dl =
    {
        .Count = sizeof(ds)/sizeof(ds[0]),
        .Items = ds
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV(&dl, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 6, argc);
    ASSERT_ARE_EQUAL(char_ptr, SERVICE_ENDPOINT_RESOURCE, argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "2", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "3", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "4", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "5", argv[5]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV_succeeds_3) /*a list with 2 entries*/
{
    ///arrange
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION ds[] =
    {
        [0] =
        {
            .Name = L"1",
            .Protocol = L"2",
            .Type = L"3",
            .Port = 4,
            .CertificateName = L"5",
            .Reserved = NULL
        },
        [1] =
        {
            .Name = L"6",
            .Protocol = L"7",
            .Type = L"8",
            .Port = 9,
            .CertificateName = L"10",
            .Reserved = NULL
        }
    };
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST dl =
    {
        .Count = sizeof(ds) / sizeof(ds[0]),
        .Items = ds
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_to_ARGC_ARGV(&dl, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 12, argc);
    ASSERT_ARE_EQUAL(char_ptr, SERVICE_ENDPOINT_RESOURCE, argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "2", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "3", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "4", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "5", argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, SERVICE_ENDPOINT_RESOURCE, argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "6", argv[7]);
    ASSERT_ARE_EQUAL(char_ptr, "7", argv[8]);
    ASSERT_ARE_EQUAL(char_ptr, "8", argv[9]);
    ASSERT_ARE_EQUAL(char_ptr, "9", argv[10]);
    ASSERT_ARE_EQUAL(char_ptr, "10", argv[11]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}




END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
