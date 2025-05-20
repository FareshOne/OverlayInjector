#include <Windows.h>
#include "OverlayUI.h"
#include "imgui.h"
#include "TokenScanner.h"   // declares extern TokenScanner g_scanner

// toggle state
static bool g_ShowOverlay = true;

// call once per-frame from hkPresent
void RenderOverlayUI() {
    // toggle visibility
    if (GetAsyncKeyState(VK_INSERT) & 1)
        g_ShowOverlay = !g_ShowOverlay;
    if (!g_ShowOverlay) 
        return;

    // fetch the latest detected tokens
    std::vector<std::string> tokens = g_scanner.GetDetectedTokens();
    if (tokens.empty()) 
        return;

    // draw collapsible window
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Token Alerts", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }
    ImGui::TextColored(ImVec4(1,0,0,1), "⚠️ Detected tokens:");
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Show tokens", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (const auto& t : tokens) {
            ImGui::BulletText("%s", t.c_str());
        }
    }
    ImGui::End();
}
