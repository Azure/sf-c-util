// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/logger.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/common_argc_argv.h"

#include "sf_c_util/fc_parameter_list_argc_argv.h"

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

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV_with_0_parameters_succeeds)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list =
    {
        .Items = NULL,
        .Count = 0
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV(&fabric_configuration_parameter_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 0, argc);

    ///clean
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV_with_1_parameters_succeeds)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER parameters[] =
    {
        [0] = {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"NN",
            .Value = L"VVV"
        }
    };
    
    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list =
    {
        .Items = parameters,
        .Count = sizeof(parameters) / sizeof(parameters[0])
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV(&fabric_configuration_parameter_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 2, argc);
    ASSERT_ARE_EQUAL(char_ptr, "NN", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "VVV", argv[1]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV_with_2_parameters_succeeds)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER parameters[] =
    {
        [0] = {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"NN",
            .Value = L"VVV"
        },
        [1] = {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"D",
            .Value = L"22"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list =
    {
        .Items = parameters,
        .Count = sizeof(parameters) / sizeof(parameters[0])
    };

    int argc;
    char** argv;
    int result;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_to_ARGC_ARGV(&fabric_configuration_parameter_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 4, argc);
    ASSERT_ARE_EQUAL(char_ptr, "NN", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "VVV", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "D", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "22", argv[3]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_0_parameters_succeeeds)
{
    ///arrange
    char** argv = NULL;
    int argc = 0;

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 0, c_argc);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_parameter_list.Count);
    ASSERT_IS_NULL(fabric_configuration_parameter_list.Items);

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_SECTION_BEGIN_parses_0_parameters_and_succeeeds)
{
    ///arrange
    char* argv[] = {
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 0, c_argc);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_parameter_list.Count);
    ASSERT_IS_NULL(fabric_configuration_parameter_list.Items);

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_parameter_without_value_parses_0_parameters_and_succeeeds)
{
    ///arrange
    char* argv[] = {
        "A" /*not a full parameter, doesn't have the value*/
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 0, c_argc);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_parameter_list.Count);
    ASSERT_IS_NULL(fabric_configuration_parameter_list.Items);

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_1_parameters_succeeeds)
{
    ///arrange
    char* argv[] = {
        "P1", "V1"
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 2, c_argc);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_parameter_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_parameter_list.Items);
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P1", fabric_configuration_parameter_list.Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V1", fabric_configuration_parameter_list.Items[0].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_1_parameters_and_SECTION_parses_1_parameter_succeeeds)
{
    ///arrange
    char* argv[] = {
        "P1", "V1",
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_parameter_list.Count);
    ASSERT_ARE_EQUAL(int, 2, c_argc);
    ASSERT_IS_NOT_NULL(fabric_configuration_parameter_list.Items);
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P1", fabric_configuration_parameter_list.Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V1", fabric_configuration_parameter_list.Items[0].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_1_parameters_and_incoplete_parameter_parses_1_parameter_succeeeds)
{
    ///arrange
    char* argv[] = {
        "P1", "V1",
        "incomplete_parameter"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_parameter_list.Count);
    ASSERT_ARE_EQUAL(int, 2, c_argc);
    ASSERT_IS_NOT_NULL(fabric_configuration_parameter_list.Items);
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P1", fabric_configuration_parameter_list.Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V1", fabric_configuration_parameter_list.Items[0].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_2_parameters_succeeeds)
{
    ///arrange
    char* argv[] = {
        "P1", "V1",
        "P2", "V2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 4, c_argc);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_parameter_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_parameter_list.Items);
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P1", fabric_configuration_parameter_list.Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V1", fabric_configuration_parameter_list.Items[0].Value);
    }
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P2", fabric_configuration_parameter_list.Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V2", fabric_configuration_parameter_list.Items[1].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_2_parameters_and_SECTION_succeeeds)
{
    ///arrange
    char* argv[] = {
        "P1", "V1",
        "P2", "V2", 
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 4, c_argc);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_parameter_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_parameter_list.Items);
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P1", fabric_configuration_parameter_list.Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V1", fabric_configuration_parameter_list.Items[0].Value);
    }
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P2", fabric_configuration_parameter_list.Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V2", fabric_configuration_parameter_list.Items[1].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV_with_2_parameters_and_incomplete_parmeter_succeeeds)
{
    ///arrange
    char* argv[] = {
        "P1", "V1",
        "P2", "V2",
        "totally_incomplete_parameter"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int c_argc;
    ARGC_ARGV_DATA_RESULT result;

    FABRIC_CONFIGURATION_PARAMETER_LIST fabric_configuration_parameter_list;

    ///act
    result = FABRIC_CONFIGURATION_PARAMETER_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_parameter_list, &c_argc);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 4, c_argc);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_parameter_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_parameter_list.Items);
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P1", fabric_configuration_parameter_list.Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V1", fabric_configuration_parameter_list.Items[0].Value);
    }
    {
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_parameter_list.Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_parameter_list.Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"P2", fabric_configuration_parameter_list.Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"V2", fabric_configuration_parameter_list.Items[1].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_PARAMETER_LIST_free(&fabric_configuration_parameter_list);
}



END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
