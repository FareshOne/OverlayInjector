#include <Windows.h>
#include "OverlayUI.h"
#include "imgui.h"

bool g_ShowMenu = true;

void HandleOverlayUI() {
    if (GetAsyncKeyState(VK_INSERT) & 1)  // Toggle menu with INSERT
        g_ShowMenu = !g_ShowMenu;

    if (!g_ShowMenu) return;

    ImGui::Begin("Token Overlay");
    ImGui::Text("Token Detected: sk-...****");
    ImGui::End();
}