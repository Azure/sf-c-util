// Copyright (c) Microsoft. All rights reserved.

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

#include "fabricruntime.h"
#include "fabrictypes.h"

#include "macro_utils/macro_utils.h"

#include "real_gballoc_ll.h"
static void* my_gballoc_malloc(size_t size)
{
    return real_gballoc_ll_malloc(size);
}

static void my_gballoc_free(void* ptr)
{
    real_gballoc_ll_free(ptr);
}

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_charptr.h"
#include "umock_c/umocktypes_wcharptr.h"
#include "umock_c/umock_c_negative_tests.h"

#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "c_pal/string_utils.h"
#include "c_util/rc_string.h"
#include "c_util/thandle.h"
#include "sf_c_util/configuration_reader.h"
#undef ENABLE_MOCKS

// Must include umock_c_prod so mocks are not expanded in real_rc_string
#include "umock_c/umock_c_prod.h"

#include "real_gballoc_hl.h"
#include "real_rc_string.h"
#include "real_string_utils.h"
#include "c_util_test_helpers/rc_string_test_decl.h"
#include "c_util_test_helpers/rc_string_test_type.h"

#include "configuration_wrapper_ut_helpers.h"
#include "test_configuration_wrapper.h"

#include "sf_c_util/configuration_wrapper.h"

CTEST_DECLARE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING);
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(TEST_THANDLE_RC_STRING, );

static TEST_MUTEX_HANDLE test_serialize_mutex;


RC_STRING_TEST_DECL(
    empty_string, ""
)

typedef THANDLE(RC_STRING) thandle_rc_string;

static const wchar_t* expected_config_package_name = L"default_config";
static const wchar_t* expected_section_name = L"MyConfigSectionName";

TEST_CONFIGURATION_WRAPPER_DEFINE_CONFIGURATION_READER_HOOKS(my_config, MY_CONFIG_TEST_PARAMS)

TEST_CONFIGURATION_WRAPPER_DEFINE_EXPECTED_CALL_HELPERS(my_config, expected_config_package_name, expected_section_name, MY_CONFIG_TEST_PARAMS)


MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error));

    ASSERT_ARE_EQUAL(int, 0, umocktypes_charptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_wcharptr_register_types());
    ASSERT_ARE_EQUAL(int, 0, umocktypes_THANDLE_RC_STRING_register_types(), "umocktypes_THANDLE_RC_STRING_register_types");

    REGISTER_RC_STRING_GLOBAL_MOCK_HOOKS();
    REGISTER_STRING_UTILS_GLOBAL_MOCK_HOOK();
    REGISTER_GBALLOC_HL_GLOBAL_MOCK_HOOK();

    REGISTER_GLOBAL_MOCK_HOOK(malloc, my_gballoc_malloc);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(malloc_flex, NULL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(realloc, NULL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(realloc_2, NULL);
    REGISTER_GLOBAL_MOCK_FAIL_RETURN(realloc_flex, NULL);
    REGISTER_GLOBAL_MOCK_HOOK(free, my_gballoc_free);

    TEST_CONFIGURATION_WRAPPER_HOOK_CONFIGURATION_READER(my_config)

    rc_string_test_init_statics();
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    rc_string_test_cleanup_statics();

    TEST_MUTEX_DESTROY(test_serialize_mutex);

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    if (TEST_MUTEX_ACQUIRE(test_serialize_mutex))
    {
        ASSERT_FAIL("Could not acquire test serialization mutex.");
    }

    // Initialize so each default value is returned (individual tests can override what gets returned)
    TEST_CONFIGURATION_WRAPPER_RESET(MY_CONFIG_TEST_PARAMS);

    umock_c_reset_all_calls();
    umock_c_negative_tests_init();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    umock_c_negative_tests_deinit();

    TEST_CONFIGURATION_WRAPPER_CLEANUP(MY_CONFIG_TEST_PARAMS);

    TEST_MUTEX_RELEASE(test_serialize_mutex);
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
