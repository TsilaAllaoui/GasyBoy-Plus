#include "instruction_viewer.h"

Instruction_viewer::Instruction_viewer(Cpu* pcpu, Mmu *pmmu)
{
    cpu = pcpu;
    mmu = pmmu;
}

Instruction_viewer::~Instruction_viewer()
{

}

void Instruction_viewer::Render()
{
    if (ImGui::Begin("Instructions", nullptr, ImVec2(0,0), ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(225,300));
        ImGui::Text("Instrucitons : ");

        int index = 0;
        for (int i=0; i<(BIOS_SIZE + (int)mmu->get_romSize()) ; i++)
        {
            opcodeInfo(&index);
        }
    }
    ImGui::End();
}

void Instruction_viewer::opcodeInfo(int *index)
{
    opcode currOpcode;
    currOpcode.mnemonic = "Unknow opcode";
    currOpcode.numberOfBytes = 0;
    switch (mmu->read_ram(*index))
    {
        case 0x31:
        {
            currOpcode.mnemonic = "LD SP, nn";
            currOpcode.numberOfBytes = 2;
            break;
        }
        default:
        {
            currOpcode.mnemonic = "Unknow opcode";
            currOpcode.numberOfBytes = 0;
        }
        break;
    }
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0,1,0,1));
    for (int i=0; i <= currOpcode.numberOfBytes; i++)
    {
        ImGui::Text("%02X", mmu->read_ram(*index + i));
        ImGui::SameLine();
    }
    ImGui::PopStyleColor();
    ImGui::SameLine(85);
    ImGui::Text("%s", (currOpcode.mnemonic).c_str());
    *index += currOpcode.numberOfBytes + 1;

}
