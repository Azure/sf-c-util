// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "windows.h"

#include "macro_utils/macro_utils.h"
#include "testrunnerswitcher.h"

#include "sf_c_util/hresult_to_string.h"

static TEST_MUTEX_HANDLE test_serialize_mutex;

/*this is E_INVALIDARG*/
#define TEST_HRESULT_E_INVALIDARG_DEFINE 0x80070057 
static const HRESULT TEST_HRESULT_E_INVALIDARG = TEST_HRESULT_E_INVALIDARG_DEFINE;

/*this is FABRIC_E_COMMUNICATION_ERROR*/
#define TEST_HRESULT_SF_DEFINE 0x80071bbc
static const HRESULT TEST_HRESULT_SF = TEST_HRESULT_SF_DEFINE;

/*this is E_POINTER*/
#define TEST_HRESULT_E_POINTER_DEFINE 0x80004003
static const HRESULT TEST_HRESULT_E_POINTER = TEST_HRESULT_E_POINTER_DEFINE;

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, gballoc_hl_init(NULL, NULL));
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{

    TEST_MUTEX_DESTROY(test_serialize_mutex);

    gballoc_hl_deinit();
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

/*these test are checks that LogHRESULTError(TEST_HRESULT); compiles (note: missing FORMAT, and the following*/
TEST_FUNCTION(LogHRESULTError_with_E_INVALIDARG_no_format_compiles)
{
    ///arrange

    ///act
    LogHRESULTError(TEST_HRESULT_E_INVALIDARG);

    ///assert

    ///cleanup
}

TEST_FUNCTION(LogHRESULTError_with_E_INVALIDARG_with_format_no_args)
{
    ///arrange

    ///act
    LogHRESULTError(TEST_HRESULT_E_INVALIDARG, "some text");

    ///assert

    ///cleanup
}


/*these tests are just checking that there are no leaks*/
TEST_FUNCTION(LogHRESULTError_with_E_INVALIDARG_with_format_and_1_arg)
{
    ///arrange

    ///act
    LogHRESULTError(TEST_HRESULT_E_INVALIDARG, "some int was=%d", 4);

    ///assert

    ///cleanup
}


/*these tests are just checking that there are no leaks*/
TEST_FUNCTION(LogHRESULTError_with_E_INVALIDARG_with_format_and_2_args)
{
    ///arrange

    ///act
    LogHRESULTError(TEST_HRESULT_E_INVALIDARG, "some int was=%d some other int was=%d", 4, 5);

    ///assert

    ///cleanup
}


TEST_FUNCTION(hresult_to_wstring_succeeds)
{
    ///arrange

    ///act
    wchar_t* t = hresult_to_wstring(TEST_HRESULT_E_POINTER);
    
    ///assert
    ASSERT_IS_NOT_NULL(t); 

    ///cleanup
    free(t);
}

TEST_FUNCTION(hresult_to_string_succeeds)
{
    ///arrange

    ///act
    char* t = hresult_to_string(malloc, free, TEST_HRESULT_E_POINTER);

    ///assert
    ASSERT_IS_NOT_NULL(t);

    ///cleanup
    free(t);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
