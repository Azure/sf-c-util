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

TEST_DEFINE_ENUM_TYPE(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_RESULT_VALUES);

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

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV__no_section_does_not_parse)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE "A",  /*so not really SECTION_NAME_DEFINE, something else*/
        "section_name_is_a_rose",
        "p1",
        "v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV__only_SECTION_NAME_DEFINE_parameters_is_invalid)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_INVALID, result);

    ///clear
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_0_parameters_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section.Parameters->Count);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_0_parameters_followed_by_section_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section.Parameters->Count);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_0_parameters_succeeds_2)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        "parameter_without_value"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section.Parameters->Count);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_1_parameters_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        "p1",
        "v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section.Parameters->Count);
    {/*[0]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p1", fabric_configuration_section.Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v1", fabric_configuration_section.Parameters->Items[0].Value);
    }
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_1_parameters_followed_by_incomplete_parameter_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        "p1",
        "v1",
        "incomplete_parameter_without_value"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section.Parameters->Count);
    {/*[0]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p1", fabric_configuration_section.Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v1", fabric_configuration_section.Parameters->Items[0].Value);
    }
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_1_parameters_followed_by_SECTION_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        "p1",
        "v1",
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section.Parameters->Count);
    {/*[0]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p1", fabric_configuration_section.Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v1", fabric_configuration_section.Parameters->Items[0].Value);
    }
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_2_parameters_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        "p1",
        "v1",
        "p2",
        "v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section.Parameters->Count);
    {/*[0]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p1", fabric_configuration_section.Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v1", fabric_configuration_section.Parameters->Items[0].Value);
    }
    {/*[1]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p2", fabric_configuration_section.Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v2", fabric_configuration_section.Parameters->Items[1].Value);
    }
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_2_parameters_and_a_half_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        "p1",
        "v1",
        "p2",
        "v2",
        "p3_but_does_not_have_value"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section.Parameters->Count);
    {/*[0]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p1", fabric_configuration_section.Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v1", fabric_configuration_section.Parameters->Items[0].Value);
    }
    {/*[1]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p2", fabric_configuration_section.Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v2", fabric_configuration_section.Parameters->Items[1].Value);
    }
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV_2_parameters_followed_by_section_succeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE,
        "section_name_is_a_rose",
        "p1",
        "v1",
        "p2",
        "v2",
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    FABRIC_CONFIGURATION_SECTION fabric_configuration_section;
    int argc_consumed;
    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_from_ARGC_ARGV(argc, argv, &fabric_configuration_section, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(wchar_ptr, L"section_name_is_a_rose", fabric_configuration_section.Name);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section.Parameters->Count);
    {/*[0]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p1", fabric_configuration_section.Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v1", fabric_configuration_section.Parameters->Items[0].Value);
    }
    {/*[1]*/
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section.Parameters->Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section.Parameters->Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"p2", fabric_configuration_section.Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"v2", fabric_configuration_section.Parameters->Items[1].Value);
    }
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);

    ///clear
    FABRIC_CONFIGURATION_SECTION_free(&fabric_configuration_section);
}



END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
