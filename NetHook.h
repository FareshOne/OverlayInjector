#pragma once
#include <winhttp.h>

// Define the function pointer type
using WinHttpWriteDataFn = BOOL(WINAPI*)(
    HINTERNET, LPCVOID, DWORD, LPDWORD
    );

// Extern declarations
extern WinHttpWriteDataFn oWinHttpWriteData;
extern BOOL WINAPI hkWinHttpWriteData(
    HINTERNET hRequest,
    LPCVOID lpBuffer,
    DWORD dwBytesToWrite,
    LPDWORD lpdwBytesWritten
);

// Initializes HTTP hooks
void SetupNetHooks();
