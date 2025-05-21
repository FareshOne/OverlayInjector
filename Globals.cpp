#include "Globals.h"

// Define the globals
HRESULT(__stdcall* oPresent)(IDXGISwapChain*, UINT, UINT) = nullptr;
ID3D12DescriptorHeap* g_DescHeap = nullptr;
