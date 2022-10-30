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

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_without_sectionName_consumes_0_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE "A", /*does not start with a section...*/
        "S1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 0, argc_consumed);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Count);
    ASSERT_IS_NULL(fabric_configuration_section_list.Items);

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_0_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE, 
        "S1" 
        /*note: no arguments follow*/
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
    ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
    
    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_0_arguments_followed_by_half_argument)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "half_argument" /*note: half_argument is not parsed*/
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
    ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_0_arguments_followed_by_SECTION_NAME_DEFINE)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
    ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_0_arguments_followed_by_SERVICE_ENDPOINT_RESOURCE)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        SERVICE_ENDPOINT_RESOURCE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
    ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_0_arguments_followed_by_CONFIGURATION_PACKAGE_NAME)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        CONFIGURATION_PACKAGE_NAME
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 2, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
    ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_1_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Items[0].Parameters->Count);
    ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
    ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
    ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
    ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
    {
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
    }
    {
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_arguments_and_a_half)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        "half_argument"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
    {
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
    }
    {
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_arguments_and_SECTION_NAME)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);
    ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
    {
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
    }
    {
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
        ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[1].Reserved);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
        ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_0_and_0_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        SECTION_NAME_DEFINE,
        "S2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 4, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_0_and_1_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }

        
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_0_and_2_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
        {/*S2 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[1].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Value);
        }


    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_1_and_0_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        SECTION_NAME_DEFINE,
        "S2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 6, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_1_and_1_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_1_and_2_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 10, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
        {/*S2 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[1].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_2_and_0_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 8, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
        {/*S1 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 0, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_2_and_1_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 10, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
        {/*S1 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 1, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_2_and_2_arguments)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
        {/*S1 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
        {/*S2 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[1].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_2_and_2_arguments_followed_by_half_argument)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2",
        "half_argument"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
        {/*S1 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
        {/*S2 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[1].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_2_and_2_arguments_followed_by_SECTION_NAME)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2",
        SECTION_NAME_DEFINE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
        {/*S1 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
        {/*S2 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[1].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_2_and_2_arguments_followed_by_CONFIGURATION_PACKAGE_NAME)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2",
        CONFIGURATION_PACKAGE_NAME
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
        {/*S1 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
        {/*S2 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[1].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}

TEST_FUNCTION(FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV_with_sectionName_with_2_sections_with_2_and_2_arguments_followed_by_SERVICE_ENDPOINT_RESOURCE)
{
    ///arrange
    FABRIC_CONFIGURATION_SECTION_LIST fabric_configuration_section_list;
    int argc_consumed;

    char* argv[] =
    {
        SECTION_NAME_DEFINE,
        "S1",
        "s1p1",
        "s1v1",
        "s1p2",
        "s1v2",
        SECTION_NAME_DEFINE,
        "S2",
        "s2p1",
        "s2v1",
        "s2p2",
        "s2v2",
        SERVICE_ENDPOINT_RESOURCE
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ARGC_ARGV_DATA_RESULT result;

    ///act
    result = FABRIC_CONFIGURATION_SECTION_LIST_from_ARGC_ARGV(argc, argv, &fabric_configuration_section_list, &argc_consumed);

    ///assert
    ASSERT_ARE_EQUAL(ARGC_ARGV_DATA_RESULT, ARGC_ARGV_DATA_OK, result);
    ASSERT_ARE_EQUAL(int, 12, argc_consumed);
    ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Count);
    ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items);

    { /*"S1"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S1", fabric_configuration_section_list.Items[0].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[0].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[0].Parameters->Items);
        {/*S1 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v1", fabric_configuration_section_list.Items[0].Parameters->Items[0].Value);
        }
        {/*S1 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[0].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[0].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1p2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s1v2", fabric_configuration_section_list.Items[0].Parameters->Items[1].Value);
        }
    }
    { /*"S2"*/
        ASSERT_ARE_EQUAL(wchar_ptr, L"S2", fabric_configuration_section_list.Items[1].Name);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters);
        ASSERT_ARE_EQUAL(int, 2, fabric_configuration_section_list.Items[1].Parameters->Count);
        ASSERT_IS_NOT_NULL(fabric_configuration_section_list.Items[1].Parameters->Items);
        {/*S2 p1*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[0].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[0].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v1", fabric_configuration_section_list.Items[1].Parameters->Items[0].Value);
        }
        {/*S2 p2*/
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].IsEncrypted);
            ASSERT_IS_FALSE(fabric_configuration_section_list.Items[1].Parameters->Items[1].MustOverride);
            ASSERT_IS_NULL(fabric_configuration_section_list.Items[1].Parameters->Items[1].Reserved);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2p2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Name);
            ASSERT_ARE_EQUAL(wchar_ptr, L"s2v2", fabric_configuration_section_list.Items[1].Parameters->Items[1].Value);
        }
    }

    ///clean
    FABRIC_CONFIGURATION_SECTION_LIST_free(&fabric_configuration_section_list);
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
