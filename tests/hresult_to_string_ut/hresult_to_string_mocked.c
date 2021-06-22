// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "windows.h"

#define GetCurrentProcess mocked_GetCurrentProcess
extern HANDLE mocked_GetCurrentProcess(void);

/*EnumProcessModules is a macro that is expanded to K32EnumProcessModules*/
#define K32EnumProcessModules mocked_K32EnumProcessModules
extern BOOL WINAPI mocked_K32EnumProcessModules(
    HANDLE  hProcess,
    HMODULE *lphModule,
    DWORD   cb,
    LPDWORD lpcbNeeded
);

#define GetModuleFileNameA mocked_GetModuleFileNameA
extern DWORD WINAPI mocked_GetModuleFileNameA(
    HMODULE hModule,
    LPSTR  lpFilename,
    DWORD   nSize
);

#define FormatMessageA mocked_FormatMessageA
extern DWORD WINAPI mocked_FormatMessageA(
    DWORD   dwFlags,
    LPCVOID lpSource,
    DWORD   dwMessageId,
    DWORD   dwLanguageId,
    LPTSTR  lpBuffer,
    DWORD   nSize,
    va_list *Arguments
);

#define FormatMessageW mocked_FormatMessageW
extern DWORD WINAPI mocked_FormatMessageW(
    DWORD   dwFlags,
    LPCVOID lpSource,
    DWORD   dwMessageId,
    DWORD   dwLanguageId,
    LPTSTR  lpBuffer,
    DWORD   nSize,
    va_list *Arguments
);

#include "../../src/hresult_to_string.c"
