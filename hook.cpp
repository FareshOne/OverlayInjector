#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <dwrite.h>
#include <winhttp.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "MinHook.h"
#include "TokenScanner.h"
#include "OverlayUI.h"

typedef HRESULT(__stdcall* PresentFn)(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags);
PresentFn oPresent = nullptr;

ID3D12Device* g_device = nullptr;
ID3D12DescriptorHeap* g_descHeap = nullptr;
HWND g_hwnd = nullptr;


bool showOverlay = true;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    static bool initialized = false;

    if (!initialized) {
        // Standard ImGui DX12 setup...
        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        HWND hwnd = desc.OutputWindow;

        // Get device + command queue
        ID3D12Device* pDevice = nullptr;
        if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&pDevice))) {
            return ((PresentFn)oPresent)(pSwapChain, SyncInterval, Flags);
        }

        // ImGui init
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX12_Init(pDevice, 3,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            g_pd3dSrvDescHeap,
            g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
            g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

            initialized = true;
        }
    }

    // Start new ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
     // 🟢 Render the token overlay here
    RenderOverlayUI();

    if (showOverlay) {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::Begin("Token Alert", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("⚠️ Token Detected: sk-...****");
        ImGui::End();
    }(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::Begin("Token Alert", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("⚠️ Token Detected: sk-...****");
    ImGui::End();

    ImGui::Render();
ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), /*cmd list*/ nullptr);
ID3D12CommandQueue* pCommandQueue = GetCommandQueueFromSwapChain(pSwapChain);
ID3D12GraphicsCommandList* pCommandList = GetCommandList();

// Hook present once (on first frame)
if (!oPresent) {
    void** vtable = *reinterpret_cast<void***>(pSwapChain);
    MH_CreateHook(vtable[8], hkPresent, reinterpret_cast<void**>(&oPresent));
    MH_EnableHook(vtable[8]);
}

    return oPresent(pSwapChain, SyncInterval, Flags);
}

typedef BOOL(WINAPI* WinHttpWriteDataFn)(HINTERNET, LPCVOID, DWORD, LPDWORD);
WinHttpWriteDataFn oWinHttpWriteData = nullptr;

TokenScanner g_scanner;

BOOL WINAPI hkWinHttpWriteData(HINTERNET hRequest, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten) {
    std::string body((const char*)lpBuffer, dwNumberOfBytesToWrite);
    auto matches = g_scanner.Scan(body);

    if (!matches.empty()) {
        for (const auto& m : matches)
            OutputDebugStringA(("Token Detected: " + m + "\n").c_str());
        // Optionally send to overlay buffer here
    }

    return oWinHttpWriteData(hRequest, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
}