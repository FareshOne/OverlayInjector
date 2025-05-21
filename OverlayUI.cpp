#include <Windows.h>
#include "imgui.h"
#include "OverlayUI.h"

// Global state declared in one .cpp
bool g_ShowMenu = true;

void RenderOverlayUI() {
    // Toggle overlay with Insert key
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        g_ShowMenu = !g_ShowMenu;
    }
    if (!g_ShowMenu) {
        return;
    }

    ImGui::Begin("Token Alerts", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings);
    // Placeholder: actual token list will be populated here
    ImGui::Text("No tokens detected yet.");
    ImGui::End();
}
