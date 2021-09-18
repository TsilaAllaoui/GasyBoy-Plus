#include "vram_viewer.h"

Vram_viewer::Vram_viewer(Mmu *pmmu)
{
    mmu = pmmu;
}

Vram_viewer::~Vram_viewer()
{

}

void Vram_viewer::Render()
{
    if (ImGui::Begin("Vram",nullptr,ImVec2(0,0), ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SetWindowPos(ImVec2(500,0));
        ImGui::SetWindowSize(ImVec2(525,450));
        ImGui::Text("VRAM");
    }
    ImGui::End();
}