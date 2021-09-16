#include "stack_viewer.h"


Stack_viewer::Stack_viewer(Cpu *pcpu ,Mmu *pmmu)
{
    mmu = pmmu;
    cpu = pcpu;
}

Stack_viewer::~Stack_viewer()
{

}

void Stack_viewer::Render()
{
   if (ImGui::Begin("Stack",nullptr,ImVec2(0,0), ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SetWindowPos(ImVec2(350,230));
        ImGui::SetWindowSize(ImVec2(150,220));

        for (int i=0; i<100; i++)
        {
            // ImGui::Text("%X04", cpu->get_SP_Stack()[i]);
            ImGui::Text("%d", i);
        }
        //ImGui::Text("SP: %02X", cpu->get_register('A'));
            
    }
    ImGui::End();
}
