#include <Windows.h>           // Must come first for Win32 types
#include <winhttp.h>           // Defines WinHttpWriteData and related
#include "NetHook.h"
#include "TokenScanner.h"
#include <MinHook.h>
#include <string>

// Global scanner and original function pointer
TokenScanner g_scanner;
WinHttpWriteDataFn oWinHttpWriteData = nullptr;

// Hooked WinHttpWriteData implementation
BOOL WINAPI hkWinHttpWriteData(
    HINTERNET hRequest,
    LPCVOID lpBuffer,
    DWORD dwBytesToWrite,
    LPDWORD lpdwBytesWritten
) {
    // Capture payload as std::string
    std::string payload(
        reinterpret_cast<const char*>(lpBuffer),
        static_cast<size_t>(dwBytesToWrite)
    );
    // Scan for tokens
    auto matches = g_scanner.Scan(payload);
    if (!matches.empty()) {
        for (const auto& token : matches) {
            OutputDebugStringA(("Token Detected: " + token + "\n").c_str());
        }
    }
    // Call original function
    return oWinHttpWriteData(hRequest, lpBuffer, dwBytesToWrite, lpdwBytesWritten);
}

// Sets up the hook on WinHttpWriteData
void SetupNetHooks() {
    HMODULE hWinHttp = GetModuleHandleA("winhttp.dll");
    if (!hWinHttp) {
        hWinHttp = LoadLibraryA("winhttp.dll");
    }
    if (hWinHttp) {
        void* target = reinterpret_cast<void*>(
            GetProcAddress(hWinHttp, "WinHttpWriteData")
            );
        if (target) {
            MH_CreateHook(target, &hkWinHttpWriteData,
                reinterpret_cast<void**>(&oWinHttpWriteData));
            MH_EnableHook(target);
        }
    }
}
