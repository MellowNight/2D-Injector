#include "menu.h"

void ShowMenu()
{
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Always);

    ImGui::Begin(" ",
        (bool*)NULL,
        (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings)
    );

    ImGui::Spacing();	
    ImGui::End();
}