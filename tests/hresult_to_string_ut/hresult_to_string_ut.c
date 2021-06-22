// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#include <cstdint>
#else
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#endif

#include "real_gballoc_ll.h"
static void* my_gballoc_malloc(size_t size)
{
    return real_gballoc_ll_malloc(size);
}

static void my_gballoc_free(void* ptr)
{
     real_gballoc_ll_free(ptr);
}

#include "macro_utils/macro_utils.h"
#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes_stdint.h"
#include "umock_c/umocktypes_charptr.h"
#include "umock_c/umocktypes_wcharptr.h"
#include "umock_c/umocktypes_bool.h"
#include "umock_c/umock_c_negative_tests.h"

#include "windows.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
MOCKABLE_FUNCTION(, DWORD, mocked_FormatMessageA,
    DWORD   ,dwFlags,
    LPCVOID ,lpSource,
    DWORD   ,dwMessageId,
    DWORD   ,dwLanguageId,
    LPCSTR  ,lpBuffer,
    DWORD   ,nSize,
    va_list *, Arguments);

MOCKABLE_FUNCTION(, DWORD, mocked_FormatMessageW,
    DWORD, dwFlags,
    LPCVOID, lpSource,
    DWORD, dwMessageId,
    DWORD, dwLanguageId,
    LPWSTR, lpBuffer,
    DWORD, nSize,
    va_list *, Arguments);

MOCKABLE_FUNCTION(, DWORD, mocked_GetCurrentProcess
);

MOCKABLE_FUNCTION(, BOOL, mocked_K32EnumProcessModules,
    HANDLE   ,hProcess,
    HMODULE* ,lphModule,
    DWORD    ,cb,
    LPDWORD  ,lpcbNeeded
);

MOCKABLE_FUNCTION(, DWORD, mocked_GetModuleFileNameA,
    HMODULE ,hModule,
    LPSTR   ,lpFilename,
    DWORD   ,nSize
);
#undef ENABLE_MOCKS

#include "real_gballoc_hl.h"


#ifdef __cplusplus
}
#endif

#include "hresult_to_string.h"

static TEST_MUTEX_HANDLE test_serialize_mutex;

MU_DEFINE_ENUM_STRINGS(UMOCK_C_ERROR_CODE, UMOCK_C_ERROR_CODE_VALUES)

static void on_umock_c_error(UMOCK_C_ERROR_CODE error_code)
{
    ASSERT_FAIL("umock_c reported error :%" PRI_MU_ENUM "", MU_ENUM_VALUE(UMOCK_C_ERROR_CODE, error_code));
}

/*this is E_INVALIDARG*/
#define TEST_HRESULT_DEFINE 0x80070057
static const HRESULT TEST_HRESULT = TEST_HRESULT_DEFINE;
#define TEST_HRESULT_STRING_DEFINE "E_INVALIDARG"
static const char* TEST_HRESULT_STRING = TEST_HRESULT_STRING_DEFINE;

/*this is FABRIC_E_COMMUNICATION_ERROR*/
#define TEST_HRESULT_SF_DEFINE 0x80071bbc
static const HRESULT TEST_HRESULT_SF = TEST_HRESULT_SF_DEFINE;
#define TEST_HRESULT_SF_STRING_DEFINE "FABRIC_E_COMMUNICATION_ERROR"
static const char* TEST_HRESULT_SF_STRING = TEST_HRESULT_SF_STRING_DEFINE;

/*this is ERROR_WINHTTP_CANNOT_CONNECT - from WinHttp which is not usually loaded in a process*/
#define TEST_HRESULT_WINHTTP_DEFINE 12029
static const HRESULT TEST_HRESULT_WINHTTP = MAKE_HRESULT(1, FACILITY_WIN32, 12029);
#define TEST_HRESULT_WINHTTP_STRING_DEFINE "let's pretend this is a real description"
#define TEST_HRESULT_WINHTTP_WSTRING_DEFINE MU_C2(L, "let's pretend this is a real description")
static const char* TEST_HRESULT_WINHTTP_STRING = TEST_HRESULT_WINHTTP_STRING_DEFINE;
static const wchar_t* TEST_HRESULT_WINHTTP_WSTRING = TEST_HRESULT_WINHTTP_WSTRING_DEFINE;


BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_init)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));
    test_serialize_mutex = TEST_MUTEX_CREATE();
    ASSERT_IS_NOT_NULL(test_serialize_mutex);

    ASSERT_ARE_EQUAL(int, 0, umock_c_init(on_umock_c_error));

    ASSERT_ARE_EQUAL(int, 0, umocktypes_stdint_register_types());

    ASSERT_ARE_EQUAL(int, 0, umocktypes_charptr_register_types());

    ASSERT_ARE_EQUAL(int, 0, umocktypes_wcharptr_register_types());

    ASSERT_ARE_EQUAL(int, 0, umocktypes_bool_register_types());

    REGISTER_UMOCK_ALIAS_TYPE(DWORD, uint32_t);
    REGISTER_UMOCK_ALIAS_TYPE(LPCVOID, void*);
    REGISTER_UMOCK_ALIAS_TYPE(HANDLE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(LPDWORD, void*);
    REGISTER_UMOCK_ALIAS_TYPE(HMODULE, void*);
    REGISTER_UMOCK_ALIAS_TYPE(LPSTR, char*);
    REGISTER_UMOCK_ALIAS_TYPE(LPCSTR, const char*);
    REGISTER_UMOCK_ALIAS_TYPE(LPWSTR, wchar_t*);
    REGISTER_UMOCK_ALIAS_TYPE(BOOL, int);



    REGISTER_GLOBAL_MOCK_HOOK(malloc, my_gballoc_malloc);
    REGISTER_GLOBAL_MOCK_HOOK(free, my_gballoc_free);
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    umock_c_deinit();

    TEST_MUTEX_DESTROY(test_serialize_mutex);

    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(method_init)
{
    if (TEST_MUTEX_ACQUIRE(test_serialize_mutex))
    {
        ASSERT_FAIL("Could not acquire test serialization mutex.");
    }

    umock_c_reset_all_calls();
}

TEST_FUNCTION_CLEANUP(method_cleanup)
{
    TEST_MUTEX_RELEASE(test_serialize_mutex);
}

/*Tests_SRS_HRESULT_TO_STRING_02_018: [ If the_malloc is NULL then hresult_to_string shall fail and return NULL. ]*/
TEST_FUNCTION(hresult_to_string_with_the_malloc_NULL_fails)
{
    ///act
    char* humanReadable = hresult_to_string(NULL, free, TEST_HRESULT);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_HRESULT_TO_STRING_02_017: [ If the_free is NULL then hresult_to_string shall fail and return NULL. ]*/
TEST_FUNCTION(hresult_to_string_with_the_free_NULL_fails)
{
    ///act
    char* humanReadable = hresult_to_string(malloc, NULL, TEST_HRESULT);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
}

/*Tests_SRS_HRESULT_TO_STRING_02_001: [ hresult_to_string shall allocate memory for a "big enough" string representation. ]*/
/*Tests_SRS_HRESULT_TO_STRING_02_002: [ hresult_to_string shall call FormatMessageA with dwFlags set to FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS. ]*/
/*Tests_SRS_HRESULT_TO_STRING_02_003: [ If FormatMessageA succeeds then hresult_to_string shall return the value as given by FormatMessageA. ]*/
TEST_FUNCTION(hresult_to_string_succeeds_from_system)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
    .SetReturn(1); /*1 is "success"*/

    ///act
    char* humanReadable = hresult_to_string(malloc, free, TEST_HRESULT);

    ///assert
    ASSERT_IS_NOT_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    my_gballoc_free(humanReadable);
}

/*Tests_SRS_HRESULT_TO_STRING_02_008: [ If there are any failures then hresult_to_string shall return NULL. ]*/
TEST_FUNCTION(hresult_to_string_fails_when_malloc_fails)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .SetReturn(NULL);

    ///act
    char* humanReadable = hresult_to_string(malloc, free, TEST_HRESULT);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}

/*Tests_SRS_HRESULT_TO_STRING_02_004: [ Otherwise, hresult_to_string shall look in all Service Fabric code for a match and return that match. ]*/
TEST_FUNCTION(hresult_to_string_succeeds_from_service_fabric)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_SF,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
    .SetReturn(0); /*0 means "system doesn't have it"*/

    ///act
    char* humanReadable = hresult_to_string(malloc, free, TEST_HRESULT_SF);

    ///assert
    ASSERT_ARE_EQUAL(char_ptr, "FABRIC_E_FIRST_RESERVED_HRESULT", humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    my_gballoc_free(humanReadable);
}

/*Tests_SRS_HRESULT_TO_STRING_02_005: [ If no Service Fabric codes match hresult then hresult_to_string shall look in all the loaded modules by the current process. ]*/
/*Tests_SRS_HRESULT_TO_STRING_02_006: [ If a module can decode hresult then that value shall be returned. ]*/
TEST_FUNCTION(hresult_to_string_succeeds_from_module_succeeds)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
    .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(TRUE);

    STRICT_EXPECTED_CALL(mocked_GetModuleFileNameA(hmodule, IGNORED_ARG, MAX_PATH))
        .SetReturn(1);
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
        hmodule, /*"some module"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(1) /*1 means the module has it!*/
        .CopyOutArgumentBuffer_lpBuffer(TEST_HRESULT_WINHTTP_STRING, strlen(TEST_HRESULT_WINHTTP_STRING) + 1);

    ///act
    char* humanReadable = hresult_to_string(malloc, free, TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_ARE_EQUAL(char_ptr, TEST_HRESULT_WINHTTP_STRING_DEFINE, humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    my_gballoc_free(humanReadable);
}

/*Tests_SRS_HRESULT_TO_STRING_02_007: [ Otherwise NULL shall be returned. ]*/
TEST_FUNCTION(hresult_to_string_succeeds_from_module_returns_NULL_no_module_can_decode_it)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(TRUE);

    STRICT_EXPECTED_CALL(mocked_GetModuleFileNameA(hmodule, IGNORED_ARG, MAX_PATH))
        .SetReturn(1);
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
        hmodule, /*"some module"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0) /*0 means the module does not have it!*/
        ;

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    char* humanReadable = hresult_to_string(malloc, free, TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}

/*Tests_SRS_HRESULT_TO_STRING_02_008: [ If there are any failures then hresult_to_string shall return NULL. ]*/
TEST_FUNCTION(hresult_to_string_succeeds_from_module_returns_when_GetModuleFileName_fails)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(TRUE);

    STRICT_EXPECTED_CALL(mocked_GetModuleFileNameA(hmodule, IGNORED_ARG, MAX_PATH))
        .SetReturn(0); /*fails*/

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    char* humanReadable = hresult_to_string(malloc, free, TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}

/*Tests_SRS_HRESULT_TO_STRING_02_008: [ If there are any failures then hresult_to_string shall return NULL. ]*/
TEST_FUNCTION(hresult_to_string_succeeds_from_module_returns_when_EnumProcessModules_fails)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(FALSE);

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    char* humanReadable = hresult_to_string(malloc, free, TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}

/*Tests_SRS_HRESULT_TO_STRING_02_009: [ hresult_to_wstring shall allocate memory for a "big enough" string representation. ]*/
/*Tests_SRS_HRESULT_TO_STRING_02_010: [ hresult_to_wstring shall call FormatMessageW with dwFlags set to FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS. ]*/
/*Tests_SRS_HRESULT_TO_STRING_02_011: [ If FormatMessageW succeeds then hresult_to_wstring shall return the value as given by FormatMessageW. ]*/
TEST_FUNCTION(hresult_to_wstring_succeeds_from_system)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
    .SetReturn(1); /*1 is "success"*/

    ///act
    wchar_t* humanReadable = hresult_to_wstring(TEST_HRESULT);

    ///assert
    ASSERT_IS_NOT_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    my_gballoc_free(humanReadable);
}

/*Tests_SRS_HRESULT_TO_STRING_02_016: [ If there are any failures then hresult_to_wstring shall return NULL. ]*/
TEST_FUNCTION(hresult_to_wstring_fails_when_malloc_fails)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .SetReturn(NULL);

    ///act
    wchar_t* humanReadable = hresult_to_wstring(TEST_HRESULT);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}

/*Tests_SRS_HRESULT_TO_STRING_02_012: [ Otherwise, hresult_to_wstring shall look in all Service Fabric code for a match and return that match. ]*/
TEST_FUNCTION(hresult_to_wstring_succeeds_from_service_fabric)
{
    ///arrange
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_SF,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
    .SetReturn(0); /*0 means "system doesn't have it"*/

    ///act
    wchar_t* humanReadable = hresult_to_wstring(TEST_HRESULT_SF);

    ///assert
    ASSERT_IS_TRUE(wcscmp(L"FABRIC_E_FIRST_RESERVED_HRESULT", humanReadable)==0);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    my_gballoc_free(humanReadable);
}

/*Tests_SRS_HRESULT_TO_STRING_02_013: [ If no Service Fabric codes match hresult then hresult_to_wstring shall look in all the loaded modules by the current process. ]*/
/*Tests_SRS_HRESULT_TO_STRING_02_014: [ If a module can decode hresult then that value shall be returned. ]*/
TEST_FUNCTION(hresult_to_wstring_succeeds_from_module_succeeds)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
    .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(TRUE);

    STRICT_EXPECTED_CALL(mocked_GetModuleFileNameA(hmodule, IGNORED_ARG, MAX_PATH))
        .SetReturn(1);
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
        hmodule, /*"some module"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(1) /*1 means the module has it!*/
        .CopyOutArgumentBuffer_lpBuffer(TEST_HRESULT_WINHTTP_WSTRING, (wcslen(TEST_HRESULT_WINHTTP_WSTRING) + 1)*sizeof(wchar_t));

    ///act
    wchar_t* humanReadable = hresult_to_wstring(TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_IS_TRUE(wcscmp(TEST_HRESULT_WINHTTP_WSTRING_DEFINE, humanReadable)==0);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
    my_gballoc_free(humanReadable);
}

/*Tests_SRS_HRESULT_TO_STRING_02_015: [ Otherwise NULL shall be returned. ]*/
TEST_FUNCTION(hresult_to_wstring_succeeds_from_module_returns_NULL_no_module_can_decode_it)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(TRUE);

    STRICT_EXPECTED_CALL(mocked_GetModuleFileNameA(hmodule, IGNORED_ARG, MAX_PATH))
        .SetReturn(1);
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
        hmodule, /*"some module"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0) /*0 means the module does not have it!*/
        ;

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    wchar_t* humanReadable = hresult_to_wstring(TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}

/*Tests_SRS_HRESULT_TO_STRING_02_016: [ If there are any failures then hresult_to_wstring shall return NULL. ]*/
TEST_FUNCTION(hresult_to_wstring_succeeds_from_module_returns_when_GetModuleFileName_fails)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(TRUE);

    STRICT_EXPECTED_CALL(mocked_GetModuleFileNameA(hmodule, IGNORED_ARG, MAX_PATH))
        .SetReturn(0); /*fails*/

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    wchar_t* humanReadable = hresult_to_wstring(TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}

/*Tests_SRS_HRESULT_TO_STRING_02_016: [ If there are any failures then hresult_to_wstring shall return NULL. ]*/
TEST_FUNCTION(hresult_to_wstring_succeeds_from_module_returns_when_EnumProcessModules_fails)
{
    ///arrange
    HMODULE hmodule = (HMODULE)0x11;
    DWORD size = sizeof(hmodule);

    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG));
    STRICT_EXPECTED_CALL(mocked_FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, /*NULL is "system"*/
        (DWORD)TEST_HRESULT_WINHTTP,
        0,
        IGNORED_ARG,
        IGNORED_ARG,
        NULL))
        .SetReturn(0); /*0 means "system doesn't have it"*/

    /*here a "for" loop over SF codes happens*/

    STRICT_EXPECTED_CALL(mocked_GetCurrentProcess());
    STRICT_EXPECTED_CALL(mocked_K32EnumProcessModules(IGNORED_ARG, IGNORED_ARG, IGNORED_ARG, IGNORED_ARG))
        .CopyOutArgumentBuffer_lphModule(&hmodule, sizeof(hmodule))
        .CopyOutArgumentBuffer_lpcbNeeded(&size, sizeof(size))
        .SetReturn(FALSE);

    STRICT_EXPECTED_CALL(free(IGNORED_ARG));

    ///act
    wchar_t* humanReadable = hresult_to_wstring(TEST_HRESULT_WINHTTP);

    ///assert
    ASSERT_IS_NULL(humanReadable);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());

    ///cleanup
}


END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
