#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "OverlayUI.h"

// Detour for IDXGISwapChain::Present
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain,
    UINT SyncInterval, UINT Flags) {
    static bool initialized = false;
    static ID3D12DescriptorHeap* descHeap = nullptr;
    static ID3D12Device* device = nullptr;

    if (!initialized) {
        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        HWND hwnd = desc.OutputWindow;

        pSwapChain->GetDevice(__uuidof(device), reinterpret_cast<void**>(&device));

        // Setup ImGui
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hwnd);

        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.NumDescriptors = 1;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descHeap));

        ImGui_ImplDX12_Init(device, 1,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            descHeap,
            descHeap->GetCPUDescriptorHandleForHeapStart(),
            descHeap->GetGPUDescriptorHandleForHeapStart());

        initialized = true;
    }

    // Start frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Render overlay UI
    RenderOverlayUI();

    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),
        nullptr);

    // Call original Present
    return oPresent(pSwapChain, SyncInterval, Flags);
}
