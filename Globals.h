#pragma once
#include <dxgi1_4.h>  // Required for IDXGISwapChain
#include <d3d12.h>    // Required for ID3D12DescriptorHeap

// Original Present function pointer
extern HRESULT(__stdcall* oPresent)(IDXGISwapChain*, UINT, UINT);

// Shared D3D12 descriptor heap for ImGui
extern ID3D12DescriptorHeap* g_DescHeap;
