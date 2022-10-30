// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fc_section_list_argc_argv.h"

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

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_0_sections)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = 0,
        .Items = NULL
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 0, argc);
    ASSERT_IS_NULL(argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_1_section_with_0_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = 0,
        .Items = NULL
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections)/sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 2, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_1_section_with_1_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters)/sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 4, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_1_section_with_2_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        },

        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p2",
            .Value = L"12"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters) / sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 6, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p2", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "12", argv[5]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_0_and_0_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = 0,
        .Items = NULL
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = 0,
        .Items = NULL
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 4, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[3]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_0_and_1_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = 0,
        .Items = NULL
    };

    FABRIC_CONFIGURATION_PARAMETER s2_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p1",
            .Value = L"21"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = sizeof(s2_parameters) / sizeof(s2_parameters[0]),
        .Items = s2_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 6, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p1", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "21", argv[5]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_0_and_2_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = 0,
        .Items = NULL
    };

    FABRIC_CONFIGURATION_PARAMETER s2_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p1",
            .Value = L"21"
        },
        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p2",
            .Value = L"22"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = sizeof(s2_parameters) / sizeof(s2_parameters[0]),
        .Items = s2_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 8, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p1", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "21", argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p2", argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "22", argv[7]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_1_and_0_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters) / sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = 0,
        .Items = NULL
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int,6, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[5]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_1_and_1_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters) / sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };

    FABRIC_CONFIGURATION_PARAMETER s2_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p1",
            .Value = L"21"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = sizeof(s2_parameters) / sizeof(s2_parameters[0]),
        .Items = s2_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 8, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p1", argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "21", argv[7]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_1_and_2_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters) / sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };

    FABRIC_CONFIGURATION_PARAMETER s2_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p1",
            .Value = L"21"
        },
        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p2",
            .Value = L"22"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = sizeof(s2_parameters) / sizeof(s2_parameters[0]),
        .Items = s2_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 10, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p1", argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "21", argv[7]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p2", argv[8]);
    ASSERT_ARE_EQUAL(char_ptr, "22", argv[9]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_2_and_0_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        },
        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p2",
            .Value = L"12"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters) / sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };


    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = 0,
        .Items = NULL
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 8, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p2", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "12", argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[7]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_2_and_1_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        },
        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p2",
            .Value = L"12"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters) / sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };

    FABRIC_CONFIGURATION_PARAMETER s2_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p1",
            .Value = L"21"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = sizeof(s2_parameters) / sizeof(s2_parameters[0]),
        .Items = s2_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 10, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p2", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "12", argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[7]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p1", argv[8]);
    ASSERT_ARE_EQUAL(char_ptr, "21", argv[9]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV_with_2_sections_with_2_and_2_arguments)
{
    ///arrange

    FABRIC_CONFIGURATION_PARAMETER s1_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p1",
            .Value = L"11"
        },
        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s1p2",
            .Value = L"12"
        },
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s1_parameter_list =
    {
        .Count = sizeof(s1_parameters) / sizeof(s1_parameters[0]),
        .Items = s1_parameters
    };

    FABRIC_CONFIGURATION_PARAMETER s2_parameters[] =
    {
        [0] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p1",
            .Value = L"21"
        },
        [1] =
        {
            .IsEncrypted = false,
            .MustOverride = false,
            .Reserved = NULL,
            .Name = L"s2p2",
            .Value = L"22"
        }
    };

    FABRIC_CONFIGURATION_PARAMETER_LIST s2_parameter_list =
    {
        .Count = sizeof(s2_parameters) / sizeof(s2_parameters[0]),
        .Items = s2_parameters
    };

    FABRIC_CONFIGURATION_SECTION fabric_configuration_sections[] =
    {
        [0] =
        {
            .Name = L"S1",
            .Parameters = &s1_parameter_list
        },
        [1] =
        {
            .Name = L"S2",
            .Parameters = &s2_parameter_list
        }
    };

    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list =
    {
        .Count = sizeof(fabric_configuration_sections) / sizeof(fabric_configuration_sections[0]),
        .Items = fabric_configuration_sections
    };

    int result;
    int argc;
    char** argv;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_to_ARGC_ARGV(&fabric_configuration_section_list, &argc, &argv);

    ///assert
    ASSERT_ARE_EQUAL(int, 0, result);
    ASSERT_ARE_EQUAL(int, 12, argc);
    ASSERT_IS_NOT_NULL(argv);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[0]);
    ASSERT_ARE_EQUAL(char_ptr, "S1", argv[1]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p1", argv[2]);
    ASSERT_ARE_EQUAL(char_ptr, "11", argv[3]);
    ASSERT_ARE_EQUAL(char_ptr, "s1p2", argv[4]);
    ASSERT_ARE_EQUAL(char_ptr, "12", argv[5]);
    ASSERT_ARE_EQUAL(char_ptr, "--sectionName", argv[6]);
    ASSERT_ARE_EQUAL(char_ptr, "S2", argv[7]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p1", argv[8]);
    ASSERT_ARE_EQUAL(char_ptr, "21", argv[9]);
    ASSERT_ARE_EQUAL(char_ptr, "s2p2", argv[10]);
    ASSERT_ARE_EQUAL(char_ptr, "22", argv[11]);

    ///clean
    ARGC_ARGV_free(argc, argv);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
