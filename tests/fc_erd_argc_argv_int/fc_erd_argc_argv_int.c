// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_erd_argc_argv.h"

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

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_to_ARGC_ARGV_succeeds)
{
    ///arrange
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d =
    {
        .CertificateName = L"cert",
        .Name = L"name",
        .Port = 4242, /*what else...?!*/
        .Protocol = L"protocol",
        .Reserved = NULL,
        .Type = L"type",
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_to_ARGC_ARGV(&d, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 6, argc);
    ASSERT_ARE_EQUAL(char_ptr, SERVICE_ENDPOINT_RESOURCE, argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "name", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "protocol", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "type", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "4242", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "cert", argv[5]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_succeeds)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "4242",
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    ASSERT_ARE_EQUAL(wchar_ptr, L"zuzu", d.Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"snowflake", d.Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"vincent", d.Type);
    ASSERT_ARE_EQUAL(uint16_t, 4242, d.Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate", d.CertificateName);
    ASSERT_IS_NULL(d.Reserved);

    ///clean
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free(&d);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_fails_with_5_arguments)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "4242",
        //"certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);

    ///clean
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_only_consumes_6_succeeds)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "4242",
        "certificate", 
        "extraneousStringThatIsNotConsumed"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    ASSERT_ARE_EQUAL(wchar_ptr, L"zuzu", d.Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"snowflake", d.Protocol);
    ASSERT_ARE_EQUAL(wchar_ptr, L"vincent", d.Type);
    ASSERT_ARE_EQUAL(uint16_t, 4242, d.Port);
    ASSERT_ARE_EQUAL(wchar_ptr, L"certificate", d.CertificateName);
    ASSERT_IS_NULL(d.Reserved);

    ///clean
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free(&d);
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_is_ARGC_ARGV_DATA_INVALID_when_not_starting_with_SERVICE_ENDPOINT_RESOURCE)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE "ANDSOMEMORE",
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "4242",
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);
 
    ///clean
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_is_ARGC_ARGV_DATA_INVALID_when_port_exceeds_uint16_t)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "65536", /*note: 65536 exceeds UINT16_MAX*/
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);

    ///clean
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_is_ARGC_ARGV_DATA_INVALID_when_port_is_not_a_number)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "NOT_A_NUMBER", /*expected number*/
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);

    ///clean
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_is_ARGC_ARGV_DATA_INVALID_when_port_number_followed_by_garbage)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "42ANDGARBAGE", /*expected number*/
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);

    ///clean
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_is_ARGC_ARGV_DATA_INVALID_when_overflow)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "18446744073709551616", /*overflownumber, UINT64MAX+1*/
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);

    ///clean
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV_is_ARGC_ARGV_DATA_INVALID_when_negative_number)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "-42", /*negative number*/
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    ///act
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);

    ///clean
}

TEST_FUNCTION(FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free_suucceeds)
{
    ///arrange
    char* argv[] =
    {
        SERVICE_ENDPOINT_RESOURCE,
        "zuzu", /*note: zuzu, snowflake and vincent are cat names*/
        "snowflake",
        "vincent",
        "42", 
        "certificate"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION d;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;
    result = FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_from_ARGC_ARGV(argc, argv, &d, &argc_consumed);
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);

    ///act
    FABRIC_ENDPOINT_RESOURCE_DESCRIPTION_free(&d);

    ///assert

    ///clean
    /*nothing left to do*/
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
