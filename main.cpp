#include <Windows.h>
#include <MinHook.h>
#include "DXHookHelper.h"
#include "NetHook.h"
#include "OverlayUI.h"

// Forward-declare the Present hook installer
extern void CreateHookForPresent();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        MH_Initialize();

        // Install DX12 Present hook
        CreateHookForPresent();

        // Install WinHttpWriteData hook
        SetupNetHooks();
    }
    return TRUE;
}
