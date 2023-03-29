// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "c_pal/string_utils.h"

#include "sf_c_util/common_argc_argv.h"

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

TEST_FUNCTION(ARGC_ARGV_concat_from_0_plus_0_yields_0)
{
    ///arrange
    int argc_1 = 0;
    char** argv_1 = NULL;

    int argc_2 = 0;
    char** argv_2 = NULL;

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 0, argc_1);

    ///clean
    ARGC_ARGV_free(argc_1, argv_1);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_0_plus_1_yields_1)
{
    ///arrange
    int argc_1 = 0;
    char** argv_1 = NULL;

    
    char* argv_2[] = {sprintf_char("A")};
    int argc_2 = sizeof(argv_2) / sizeof(argv_2[0]);
    ASSERT_ARE_EQUAL(int, 1, argc_2);
    ASSERT_IS_NOT_NULL(argv_2[0]);

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 1, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);

    ///clean
    free(argv_2[0]);
    ARGC_ARGV_free(argc_1, argv_1);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_0_plus_2_yields_2)
{
    ///arrange
    int argc_1 = 0;
    char** argv_1 = NULL;

    char* argv_2[] = { sprintf_char("A"),  sprintf_char("BB") };
    int argc_2 = sizeof(argv_2) / sizeof(argv_2[0]);
    ASSERT_ARE_EQUAL(int, 2, argc_2);
    ASSERT_IS_NOT_NULL(argv_2[0]);
    ASSERT_IS_NOT_NULL(argv_2[1]);

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 2, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);
    ASSERT_ARE_EQUAL(char_ptr, "BB", argv_1[1]);

    ///clean
    free(argv_2[0]);
    free(argv_2[1]);
    ARGC_ARGV_free(argc_1, argv_1);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_1_plus_0_yields_1)
{
    ///arrange
    
    int argc_1 = 1;
    char** argv_1 = malloc(argc_1 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_1);
    argv_1[0] = sprintf_char("A");
    ASSERT_IS_NOT_NULL(argv_1[0]);

    char** argv_2 = NULL;
    int argc_2 = 0;

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 1, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);

    ///clean
    ARGC_ARGV_free(argc_1, argv_1);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_1_plus_1_yields_2)
{
    ///arrange

    int argc_1 = 1;
    char** argv_1 = malloc(argc_1 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_1);
    argv_1[0] = sprintf_char("A");
    ASSERT_IS_NOT_NULL(argv_1[0]);

    int argc_2 = 1;
    char** argv_2 = malloc(argc_1 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_2);
    argv_2[0] = sprintf_char("B");
    ASSERT_IS_NOT_NULL(argv_2[0]);

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 2, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);
    ASSERT_ARE_EQUAL(char_ptr, "B", argv_1[1]);

    ///clean
    ARGC_ARGV_free(argc_1, argv_1);
    ARGC_ARGV_free(argc_2, argv_2);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_1_plus_2_yields_3)
{
    ///arrange

    int argc_1 = 1;
    char** argv_1 = malloc(argc_1 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_1);
    argv_1[0] = sprintf_char("A");
    ASSERT_IS_NOT_NULL(argv_1[0]);

    int argc_2 = 2;
    char** argv_2 = malloc(argc_2 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_2);
    argv_2[0] = sprintf_char("B");
    ASSERT_IS_NOT_NULL(argv_2[0]);
    argv_2[1] = sprintf_char("CC");
    ASSERT_IS_NOT_NULL(argv_2[1]);

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);
    
    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 3, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);
    ASSERT_ARE_EQUAL(char_ptr, "B", argv_1[1]);
    ASSERT_ARE_EQUAL(char_ptr, "CC", argv_1[2]);

    ///clean
    ARGC_ARGV_free(argc_1, argv_1);
    ARGC_ARGV_free(argc_2, argv_2);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_2_plus_0_yields_3)
{
    ///arrange

    int argc_1 = 2;
    char** argv_1 = malloc(argc_1 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_1);
    argv_1[0] = sprintf_char("A");
    ASSERT_IS_NOT_NULL(argv_1[0]);
    argv_1[1] = sprintf_char("BB");
    ASSERT_IS_NOT_NULL(argv_1[1]);

    int argc_2 = 0;
    char** argv_2 = NULL;

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 2, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);
    ASSERT_ARE_EQUAL(char_ptr, "BB", argv_1[1]);

    ///clean
    ARGC_ARGV_free(argc_1, argv_1);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_2_plus_1_yields_3)
{
    ///arrange

    int argc_1 = 2;
    char** argv_1 = malloc(argc_1 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_1);
    argv_1[0] = sprintf_char("A");
    ASSERT_IS_NOT_NULL(argv_1[0]);
    argv_1[1] = sprintf_char("BB");
    ASSERT_IS_NOT_NULL(argv_1[1]);

    int argc_2 = 1;
    char** argv_2 = malloc(argc_2 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_2);
    argv_2[0] = sprintf_char("CCC");
    ASSERT_IS_NOT_NULL(argv_2[0]);

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 3, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);
    ASSERT_ARE_EQUAL(char_ptr, "BB", argv_1[1]);
    ASSERT_ARE_EQUAL(char_ptr, "CCC", argv_1[2]);

    ///clean
    ARGC_ARGV_free(argc_1, argv_1);
    ARGC_ARGV_free(argc_2, argv_2);
}

TEST_FUNCTION(ARGC_ARGV_concat_from_2_plus_2_yields_4)
{
    ///arrange

    int argc_1 = 2;
    char** argv_1 = malloc(argc_1 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_1);
    argv_1[0] = sprintf_char("A");
    ASSERT_IS_NOT_NULL(argv_1[0]);
    argv_1[1] = sprintf_char("BB");
    ASSERT_IS_NOT_NULL(argv_1[1]);

    int argc_2 = 2;
    char** argv_2 = malloc(argc_2 * sizeof(char*));
    ASSERT_IS_NOT_NULL(argv_2);
    argv_2[0] = sprintf_char("CCC");
    ASSERT_IS_NOT_NULL(argv_2[0]);
    argv_2[1] = sprintf_char("DDDD");
    ASSERT_IS_NOT_NULL(argv_2[1]);

    int result;

    ///act
    result = ARGC_ARGV_concat(&argc_1, &argv_1, argc_2, argv_2);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 4, argc_1);
    ASSERT_ARE_EQUAL(char_ptr, "A", argv_1[0]);
    ASSERT_ARE_EQUAL(char_ptr, "BB", argv_1[1]);
    ASSERT_ARE_EQUAL(char_ptr, "CCC", argv_1[2]);
    ASSERT_ARE_EQUAL(char_ptr, "DDDD", argv_1[3]);

    ///clean
    ARGC_ARGV_free(argc_1, argv_1);
    ARGC_ARGV_free(argc_2, argv_2);
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
