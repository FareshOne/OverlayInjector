#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "extern/MinHook/include/MinHook.h"
#include "TokenScanner.h"
#include "NetHook.h"
#include "LogSystem.h"

LogSystem g_log;


// Forward declarations (only declare, don't define here)
extern void CreateHookForPresent();

void SetupHooks() {
    MH_Initialize();

    // DX12 overlay hook
    CreateHookForPresent();

    // Net hooks (WinHttp etc.)
    SetupNetHooks();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        SetupHooks();
    }
    return TRUE;
}
