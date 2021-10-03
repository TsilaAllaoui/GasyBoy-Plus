#include "stack_viewer.h"


Stack_viewer::Stack_viewer(Cpu *pcpu ,Mmu *pmmu)
{
    mmu = pmmu;
    cpu = pcpu;
}

Stack_viewer::~Stack_viewer()
{

}

void Stack_viewer::Render(Mmu *pmmu)
{
   if (ImGui::Begin("Stack",nullptr,ImVec2(0,0), ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SetWindowPos(ImVec2(350,230));
        ImGui::SetWindowSize(ImVec2(150,220));
            for (uint16_t i = 0xFFFE; i>=cpu->get_curr_SP(); i-=2)//i>=0xFFF0; i-=2)
            {
                ///if (mmu->read_ram(i) != 0 || mmu->read_ram(i+1) != 0)
                {
                    /*int a = rand()%255, b = rand()%255;
                    mmu->write_ram(i, a);
                    mmu->write_ram(i+1, b);
                    cout << "a = " << mmu->read_ram(i) << " ,b = " << mmu->read_ram(i+1) << endl;*/
                    ImGui::Text("%04X: ", i);
                    ImGui::SameLine();
                    ImGui::Text("%02X", pmmu->read_ram(i+1));
                    ImGui::SameLine();
                    ImGui::Text("%02X", pmmu->read_ram(i));
                }
            }
    }
    ImGui::End();
}
