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

typedef HRESULT(__stdcall* PresentFn)(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags);
PresentFn oPresent = nullptr;

ID3D12Device* g_device = nullptr;
ID3D12DescriptorHeap* g_descHeap = nullptr;
HWND g_hwnd = nullptr;


bool showOverlay = true;

HRESULT __stdcall hkPresent
(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    static bool initialized = false;

    // Handle toggle
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        showOverlay = !showOverlay;
    }


    if (!initialized) {
        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        g_hwnd = desc.OutputWindow;

        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&g_device))) {
            ImGui::CreateContext();
            ImGui_ImplWin32_Init(g_hwnd);

            D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            heapDesc.NumDescriptors = 1;
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            g_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&g_descHeap));

            ImGui_ImplDX12_Init(g_device, 3,
                DXGI_FORMAT_R8G8B8A8_UNORM,
                g_descHeap,
                g_descHeap->GetCPUDescriptorHandleForHeapStart(),
                g_descHeap->GetGPUDescriptorHandleForHeapStart());

            initialized = true;
        }
    }

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

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