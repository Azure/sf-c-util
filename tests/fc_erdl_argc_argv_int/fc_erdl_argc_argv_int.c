// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_erdl_argc_argv.h"

TEST_DEFINE_ENUM_TYPE(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
}

TEST_FUNCTION_INITIALIZE(method_init)
{
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
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

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV_succeeds_1) /*an empty list produces an empty list*/
{
    int argc = 0;
    char** argv = NULL;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST list;
    int argc_consumed;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(argc, argv, &list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 0, argc_consumed);

    ASSERT_ARE_EQUAL(ULONG, 0, list.Count);
    ASSERT_IS_NULL(list.Items);

    ///clean
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free(&list);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV_succeeds_2) /*a list with 1 endpoint*/
{
    
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "4242", 
        "certificate"
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST list;
    int argc_consumed;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(argc, argv, &list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ASSERT_ARE_EQUAL(ULONG, 1, list.Count);
    ASSERT_IS_NOT_NULL(list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"zuzu", list.Items[0].Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"snowflake", list.Items[0].Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"vincent", list.Items[0].Type);
    ASSERT_ARE_EQUAL(uint16_t, 4242, list.Items[0].Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate", list.Items[0].CertificateName);
    ASSERT_IS_NULL(list.Items[0].Reserved);

    ///clean
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free(&list);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV_succeeds_3) /*a list with 2 endpoints*/
{

    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "4242",
        "certificate",
        SERVICE_ENDPOINT_RESOURCE,
        "2zuzu", /*zuzu, snowflake and vincent are cat names*/
        "2snowflake",
        "2vincent",
        "24242",
        "2certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST list;
    int argc_consumed;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(argc, argv, &list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);

    ASSERT_ARE_EQUAL(ULONG, 2, list.Count);
    ASSERT_IS_NOT_NULL(list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"zuzu", list.Items[0].Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"snowflake", list.Items[0].Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"vincent", list.Items[0].Type);
    ASSERT_ARE_EQUAL(uint16_t, 4242, list.Items[0].Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate", list.Items[0].CertificateName);
    ASSERT_IS_NULL(list.Items[0].Reserved);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2zuzu", list.Items[1].Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2snowflake", list.Items[1].Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2vincent", list.Items[1].Type);
    ASSERT_ARE_EQUAL(uint16_t, 24242, list.Items[1].Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2certificate", list.Items[1].CertificateName);
    ASSERT_IS_NULL(list.Items[1].Reserved);

    ///clean
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free(&list);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV_succeeds_4) /*a list with 2 endpoints and some extraneous parameter still produces 2 lists*/
{

    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "4242",
        "certificate",
        SERVICE_ENDPOINT_RESOURCE,
        "2zuzu", /*zuzu, snowflake and vincent are cat names*/
        "2snowflake",
        "2vincent",
        "24242",
        "2certificate",
        "extra"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST list;
    int argc_consumed;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_from_ARGC_ARGV(argc, argv, &list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);

    ASSERT_ARE_EQUAL(ULONG, 2, list.Count);
    ASSERT_IS_NOT_NULL(list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"zuzu", list.Items[0].Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"snowflake", list.Items[0].Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"vincent", list.Items[0].Type);
    ASSERT_ARE_EQUAL(uint16_t, 4242, list.Items[0].Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate", list.Items[0].CertificateName);
    ASSERT_IS_NULL(list.Items[0].Reserved);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2zuzu", list.Items[1].Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2snowflake", list.Items[1].Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2vincent", list.Items[1].Type);
    ASSERT_ARE_EQUAL(uint16_t, 24242, list.Items[1].Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"2certificate", list.Items[1].CertificateName);
    ASSERT_IS_NULL(list.Items[1].Reserved);

    ///clean
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_LIST_free(&list);
}



END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
