#include "register_viewer.h"

Register_viewer::Register_viewer(Cpu *pcpu)
{
    cpu = pcpu;
}

Register_viewer::~Register_viewer()
{

}

void Register_viewer::Render()
{
    if (ImGui::Begin("Registers", nullptr, ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SetWindowPos(ImVec2(350,0));
        ImGui::SetWindowSize(ImVec2(150,230));
        ImGui::Text("Registers: ");
        ImGui::Text("A: ");
        ImGui::SameLine(25);
        ImGui::Text("%02X",cpu->get_register('A'));
        ImGui::Text("B: ");
        ImGui::SameLine(25);
        ImGui::Text("%02X",cpu->get_register('B'));
        ImGui::SameLine(45);
        ImGui::Text(" |  C: ");
        ImGui::SameLine(90);
        ImGui::Text("%02X",cpu->get_register('C'));
        ImGui::Text("D: ");
        ImGui::SameLine(25);
        ImGui::Text("%02X",cpu->get_register('D'));
        ImGui::SameLine(45);
        ImGui::Text(" |  E: ");
        ImGui::SameLine(90);
        ImGui::Text("%02X",cpu->get_register('E'));
        ImGui::Text("H: ");
        ImGui::SameLine(25);
        ImGui::Text("%02X",cpu->get_register('H'));
        ImGui::SameLine(45);
        ImGui::Text(" |  L: ");
        ImGui::SameLine(90);
        ImGui::Text("%02X",cpu->get_register('L'));
        ImGui::Text("Flags:");
        if (cpu->get_specialRegister().get_zflag())
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
        else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f,0.2f,0.2f,1));
        ImGui::Text("Z");
        ImGui::PopStyleColor();
        ImGui::SameLine(25);
        if (cpu->get_specialRegister().get_subflag())
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
        else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f,0.2f,0.2f,1));
        ImGui::Text("N");
        ImGui::PopStyleColor();
        ImGui::SameLine(50);
        if (cpu->get_specialRegister().get_halfcarryflag())
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
        else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f,0.2f,0.2f,1));
        ImGui::Text("H");
        ImGui::PopStyleColor();
        ImGui::SameLine(75);
        if (cpu->get_specialRegister().get_carryflag())
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
        else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f,0.2f,0.2f,1));
        ImGui::Text("C");
        ImGui::PopStyleColor();

        ImGui::Text("PC: %04X", cpu->get_PC());
        ImGui::Text("SP: %04X", cpu->get_SP());

        ImGui::Separator();

        if (ImGui::Button("Step"))
        {
            if (!cpu->get_cpuState())
                cpu->cpuStep();
        }

        if (ImGui::Button("Continue"))
            cpu->set_cpuState(true);

        ImGui::SameLine(75);

        if (ImGui::Button("Stop"))
            cpu->set_cpuState(false);

//        if (cpu->get_cpuState())
//            cpu->cpuStep();
    }
    ImGui::End();
}
