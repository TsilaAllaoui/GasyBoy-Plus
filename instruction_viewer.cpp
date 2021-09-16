#include "instruction_viewer.h"

Instruction_viewer::Instruction_viewer(Cpu* pcpu, Mmu *pmmu)
{
    cpu = pcpu;
    mmu = pmmu;
    currOpcode.numberOfBytes = -1;
    currLine = 0;
    prevLine = -1;
    cpu->addCpuListener(this);
    this->fillMap();
}

void Instruction_viewer::OnStep()
{
	uint16_t pc = cpu->get_PC();
	currLine = mapInstr2line[pc];


	if (breakPointList.size() > 0)
    {
        for (int i=0; i<breakPointList.size(); i++)
        {
            if (mapInstr2line[pc] == breakPointList[i])
                cpu->set_cpuState(false);
        }
    }

}

void Instruction_viewer::showLine(int *index)
{
    if (*index <= BIOS_SIZE)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
        ImGui::Text("    BOOT%04X: ", *index);
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
        ImGui::Text("    ROM%04X: ", *index - BIOS_SIZE);
        ImGui::PopStyleColor();
    }
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0,1,0,1));
    for (int i=0; i <= GetopcodeInfo(*index).numberOfBytes - 1; i++)
    {
        ImGui::Text("%02X", mmu->read_ram(*index + i));
        ImGui::SameLine();
    }
    ImGui::PopStyleColor();
    ImGui::SameLine(160);
    ImGui::Text("   %s",  GetopcodeInfo(*index).mnemonic.c_str());
    *index += GetopcodeInfo(*index).numberOfBytes;

}

void Instruction_viewer::Render()
{

    if (ImGui::Begin("Instructions", nullptr, ImVec2(0,0), ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(350,450));
        ImGui::Text("Instructions : ");

        int index = 0;
        float lineHeight = ImGui::GetTextLineHeight();
        for (int i=0; i<(BIOS_SIZE + (int)mmu->get_romSize()) ; i++)
        {

            /*****Highligt clicked line*****/
            ImVec2 screenPos = ImGui::GetCursorScreenPos();
            if (i == selectedLine)
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0,screenPos.y), ImVec2(275,screenPos.y+lineHeight), ImColor(255,0,0));
                for (int j=0; j<breakPointList.size(); j++)
                    if (i == breakPointList[j])
                        ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(12,screenPos.y + (lineHeight/2.0f)), 5.0f, ImColor(0,0,255));

            if (i == currLine)
                ImGui::GetWindowDrawList()->AddTriangleFilled(ImVec2(23,(screenPos.y + 10)), ImVec2(30,(screenPos.y +6)), ImVec2(23,(screenPos.y + 2)), ImColor(0,255,0));

                this->mouseEvent();
            if (ImGui::IsMouseClicked(0))
            {
                ImVec2 mousepos = ImGui::GetMousePos();
                if ((mousepos.x <= (ImGui::GetWindowWidth() - 18)) &&(mousepos.y >= screenPos.y) && (mousepos.y <= (screenPos.y + lineHeight)))
                    selectedLine = i;
            }

            /*****add breakpoint to clicked line*****/
            if (ImGui::IsMouseDoubleClicked(0))
            {
                ImVec2 mousepos = ImGui::GetMousePos();
                if ((mousepos.x <= (ImGui::GetWindowWidth() - 18)) && (mousepos.y >= screenPos.y) && (mousepos.y <= (screenPos.y + lineHeight)))
                {
                    if (breakPointList.size() > 0)
                    {
                        for (int j=0; j<breakPointList.size(); j++)
                        {
                            if (breakPointList[j] == i)
                            {
                               breakPointList.erase(breakPointList.begin() + j);
                               showLine(&index);
                               ImGui::End();
                               return;
                            }
                        }
                         breakPointList.push_back(i);
                    }
                    else
                    {
                        breakPointList.push_back(i);
                    }
                }
            }

             showLine(&index);

        }
    }
    ImGui::End();
}

void Instruction_viewer::mouseEvent()
{

}

void Instruction_viewer::fillMap()
{
    instrList.clear();
    for (int i=0; i<(BIOS_SIZE + (int)mmu->get_romSize()) ; i++)
    {
        int offset = i;
        instrList.push_back(GetopcodeInfo(i));
        i += GetopcodeInfo(i).numberOfBytes - 1;
        mapInstr2line[offset] = instrList.size() - 1;
    }

}

opcode Instruction_viewer::GetopcodeInfo(int index)
{
    currOpcode.mnemonic = "Unknow opcode";
    currOpcode.numberOfBytes = 0;
    switch (mmu->read_ram(index))
    {
    case 0x00:
    {
        currOpcode.mnemonic = "NOP";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x01:
    {
        currOpcode.mnemonic = "LD BC, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0x02:
    {
        currOpcode.mnemonic = "LD (BC), A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x03:
    {
        currOpcode.mnemonic = "INC BC";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x04:
    {
        currOpcode.mnemonic = "INC B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x05:
    {
        currOpcode.mnemonic = "DEC B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x06:
    {
        currOpcode.mnemonic = "LD B, "+ int2hex(mmu->read_ram(index+1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x07:
    {
        currOpcode.mnemonic = "RLCA";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x08:
    {
        currOpcode.mnemonic = "LD (" + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1)) + "), SP";
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0x09:
    {
        currOpcode.mnemonic = "ADD HL, BC";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x0A:
    {
        currOpcode.mnemonic = "LD A,(BC)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x0B:
    {
        currOpcode.mnemonic = "DEC BC";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x0C:
    {
        currOpcode.mnemonic = "INC C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x0D:
    {
        currOpcode.mnemonic = "DEC C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x0E:
    {
        currOpcode.mnemonic = "LD C, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x0F:
    {
        currOpcode.mnemonic = "RRCA";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x10:
    {
        currOpcode.mnemonic = "STOP";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x11:
    {
        currOpcode.mnemonic = "LD DE, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0x12:
    {
        currOpcode.mnemonic = "LD (DE), A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x13:
    {
        currOpcode.mnemonic = "INC DE";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x14:
    {
        currOpcode.mnemonic = "INC D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x15:
    {
        currOpcode.mnemonic = "DEC D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x16:
    {
        currOpcode.mnemonic = "LD D, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x17:
    {
        currOpcode.mnemonic = "RLA";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x18:
    {
        currOpcode.mnemonic = "JR r";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x19:
    {
        currOpcode.mnemonic = "ADD HL, DE";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x1A:
    {
        currOpcode.mnemonic = "LD A,(DE)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x1B:
    {
        currOpcode.mnemonic = "DEC DE";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x1C:
    {
        currOpcode.mnemonic = "INC E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x1D:
    {
        currOpcode.mnemonic = "DEC E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x1E:
    {
        currOpcode.mnemonic = "LD E, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x1F:
    {
        currOpcode.mnemonic = "RRA";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x20:
    {
        currOpcode.mnemonic = "JR NZ, r";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x21:
    {
        currOpcode.mnemonic = "LD HL, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0x22:
    {
        currOpcode.mnemonic = "LD (HL+), A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x23:
    {
        currOpcode.mnemonic = "INC HL";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x24:
    {
        currOpcode.mnemonic = "INC H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x25:
    {
        currOpcode.mnemonic = "DEC H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x26:
    {
        currOpcode.mnemonic = "LD H, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x27:
    {
        currOpcode.mnemonic = "DAA";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x28:
    {
        currOpcode.mnemonic = "JR Z,r";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x29:
    {
        currOpcode.mnemonic = "ADD HL, HL";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x2A:
    {
        currOpcode.mnemonic = "LD A,(HL+)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x2B:
    {
        currOpcode.mnemonic = "DEC HL";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x2C:
    {
        currOpcode.mnemonic = "INC L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x2D:
    {
        currOpcode.mnemonic = "DEC L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x2E:
    {
        currOpcode.mnemonic = "LD L, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x2F:
    {
        currOpcode.mnemonic = "CPL";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x30:
    {
        currOpcode.mnemonic = "JR NC, r";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x31:
    {
        currOpcode.mnemonic = "LD SP, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0x32:
    {
        currOpcode.mnemonic = "LD (HL-), A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x33:
    {
        currOpcode.mnemonic = "INC SP";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x34:
    {
        currOpcode.mnemonic = "INC (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x35:
    {
        currOpcode.mnemonic = "DEC (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x36:
    {
        currOpcode.mnemonic = "LD (HL), " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x37:
    {
        currOpcode.mnemonic = "SCF";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x38:
    {
        currOpcode.mnemonic = "JR C,r";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x39:
    {
        currOpcode.mnemonic = "ADD HL, SP";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x3A:
    {
        currOpcode.mnemonic = "LD A,(HL-)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x3B:
    {
        currOpcode.mnemonic = "DEC SP";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x3C:
    {
        currOpcode.mnemonic = "INC A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x3D:
    {
        currOpcode.mnemonic = "DEC A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x3E:
    {
        currOpcode.mnemonic = "LD A, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x3F:
    {
        currOpcode.mnemonic = "CCF";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x40:
    {
        currOpcode.mnemonic = "LD B, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x41:
    {
        currOpcode.mnemonic = "LD B, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x42:
    {
        currOpcode.mnemonic = "LD B, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x43:
    {
        currOpcode.mnemonic = "LD B, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x44:
    {
        currOpcode.mnemonic = "LD B, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x45:
    {
        currOpcode.mnemonic = "LD B, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x46:
    {
        currOpcode.mnemonic = "LD B, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x47:
    {
        currOpcode.mnemonic = "LD B, A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x48:
    {
        currOpcode.mnemonic = "LD C, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x49:
    {
        currOpcode.mnemonic = "LD C, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x4A:
    {
        currOpcode.mnemonic = "LD C, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x4B:
    {
        currOpcode.mnemonic = "LD C, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x4C:
    {
        currOpcode.mnemonic = "LD C, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x4D:
    {
        currOpcode.mnemonic = "LD C, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x4E:
    {
        currOpcode.mnemonic = "LD C, (HL)";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0x4F:
    {
        currOpcode.mnemonic = "LD C, A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x50:
    {
        currOpcode.mnemonic = "LD D, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x51:
    {
        currOpcode.mnemonic = "LD D, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x52:
    {
        currOpcode.mnemonic = "LD D, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x53:
    {
        currOpcode.mnemonic = "LD D, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x54:
    {
        currOpcode.mnemonic = "LD D, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x55:
    {
        currOpcode.mnemonic = "LD D, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x56:
    {
        currOpcode.mnemonic = "LD D, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x57:
    {
        currOpcode.mnemonic = "LD D, A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x58:
    {
        currOpcode.mnemonic = "LD E, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x59:
    {
        currOpcode.mnemonic = "LD E, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x5A:
    {
        currOpcode.mnemonic = "LD E, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x5B:
    {
        currOpcode.mnemonic = "LD E, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x5C:
    {
        currOpcode.mnemonic = "LD E, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x5D:
    {
        currOpcode.mnemonic = "LD E, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x5E:
    {
        currOpcode.mnemonic = "LD E, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x5F:
    {
        currOpcode.mnemonic = "LD E, A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x60:
    {
        currOpcode.mnemonic = "LD H, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x61:
    {
        currOpcode.mnemonic = "LD H, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x62:
    {
        currOpcode.mnemonic = "LD H, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x63:
    {
        currOpcode.mnemonic = "LD H, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x64:
    {
        currOpcode.mnemonic = "LD H, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x65:
    {
        currOpcode.mnemonic = "LD H, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x66:
    {
        currOpcode.mnemonic = "LD H, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x67:
    {
        currOpcode.mnemonic = "LD H, A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x68:
    {
        currOpcode.mnemonic = "LD L, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x69:
    {
        currOpcode.mnemonic = "LD L, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x6A:
    {
        currOpcode.mnemonic = "LD L, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x6B:
    {
        currOpcode.mnemonic = "LD L, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x6C:
    {
        currOpcode.mnemonic = "LD L, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x6D:
    {
        currOpcode.mnemonic = "LD L, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x6E:
    {
        currOpcode.mnemonic = "LD L, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x6F:
    {
        currOpcode.mnemonic = "LD L, A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x70:
    {
        currOpcode.mnemonic = "LD (HL), B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x71:
    {
        currOpcode.mnemonic = "LD (HL), C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x72:
    {
        currOpcode.mnemonic = "LD (HL), D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x73:
    {
        currOpcode.mnemonic = "LD (HL), E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x74:
    {
        currOpcode.mnemonic = "LD (HL), H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x75:
    {
        currOpcode.mnemonic = "LD (HL), L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x76:
    {
        currOpcode.mnemonic = "HALT";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x77:
    {
        currOpcode.mnemonic = "LD (HL), A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x78:
    {
        currOpcode.mnemonic = "LD A, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x79:
    {
        currOpcode.mnemonic = "LD A, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x7A:
    {
        currOpcode.mnemonic = "LD A, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x7B:
    {
        currOpcode.mnemonic = "LD A, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x7C:
    {
        currOpcode.mnemonic = "LD A, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x7D:
    {
        currOpcode.mnemonic = "LD A, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x7E:
    {
        currOpcode.mnemonic = "LD A, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x7F:
    {
        currOpcode.mnemonic = "LD A, A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x80:
    {
        currOpcode.mnemonic = "ADD A, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x81:
    {
        currOpcode.mnemonic = "ADD A,C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x82:
    {
        currOpcode.mnemonic = "ADD A,D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x83:
    {
        currOpcode.mnemonic = "ADD A,E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x84:
    {
        currOpcode.mnemonic = "ADD A,H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x85:
    {
        currOpcode.mnemonic = "ADD A,L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x86:
    {
        currOpcode.mnemonic = "ADD A,(HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x87:
    {
        currOpcode.mnemonic = "ADD A,A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x88:
    {
        currOpcode.mnemonic = "ADC A, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x89:
    {
        currOpcode.mnemonic = "ADC A, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x8A:
    {
        currOpcode.mnemonic = "ADC A, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x8B:
    {
        currOpcode.mnemonic = "ADC A, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x8C:
    {
        currOpcode.mnemonic = "ADC A, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x8D:
    {
        currOpcode.mnemonic = "ADC A, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x8E:
    {
        currOpcode.mnemonic = "ADC A, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x8F:
    {
        currOpcode.mnemonic = "ADC A, A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0x90:
    {
        currOpcode.mnemonic = "SUB B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x91:
    {
        currOpcode.mnemonic = "SUB C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x92:
    {
        currOpcode.mnemonic = "SUB D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x93:
    {
        currOpcode.mnemonic = "SUB E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x94:
    {
        currOpcode.mnemonic = "SUB H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x95:
    {
        currOpcode.mnemonic = "SUB L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x96:
    {
        currOpcode.mnemonic = "SUB (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x97:
    {
        currOpcode.mnemonic = "SUB A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x98:
    {
        currOpcode.mnemonic = "SBC A, B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x99:
    {
        currOpcode.mnemonic = "SBC A, C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x9A:
    {
        currOpcode.mnemonic = "SBC A, D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x9B:
    {
        currOpcode.mnemonic = "SBC A, E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x9C:
    {
        currOpcode.mnemonic = "SBC A, H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x9D:
    {
        currOpcode.mnemonic = "SBC A, L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x9E:
    {
        currOpcode.mnemonic = "SBC A, (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0x9F:
    {
        currOpcode.mnemonic = "SBC A, A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0xA0:
    {
        currOpcode.mnemonic = "AND B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA1:
    {
        currOpcode.mnemonic = "AND C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA2:
    {
        currOpcode.mnemonic = "AND D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA3:
    {
        currOpcode.mnemonic = "AND E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA4:
    {
        currOpcode.mnemonic = "AND H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA5:
    {
        currOpcode.mnemonic = "AND L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA6:
    {
        currOpcode.mnemonic = "AND (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA7:
    {
        currOpcode.mnemonic = "AND A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA8:
    {
        currOpcode.mnemonic = "XOR B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xA9:
    {
        currOpcode.mnemonic = "XOR C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xAA:
    {
        currOpcode.mnemonic = "XOR D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xAB:
    {
        currOpcode.mnemonic = "XOR E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xAC:
    {
        currOpcode.mnemonic = "XOR H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xAD:
    {
        currOpcode.mnemonic = "XOR L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xAE:
    {
        currOpcode.mnemonic = "XOR (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xAF:
    {
        currOpcode.mnemonic = "XOR A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0xB0:
    {
        currOpcode.mnemonic = "OR B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB1:
    {
        currOpcode.mnemonic = "OR C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB2:
    {
        currOpcode.mnemonic = "OR D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB3:
    {
        currOpcode.mnemonic = "OR E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB4:
    {
        currOpcode.mnemonic = "OR H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB5:
    {
        currOpcode.mnemonic = "OR L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB6:
    {
        currOpcode.mnemonic = "OR (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB7:
    {
        currOpcode.mnemonic = "OR A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB8:
    {
        currOpcode.mnemonic = "CP B";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xB9:
    {
        currOpcode.mnemonic = "CP C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xBA:
    {
        currOpcode.mnemonic = "CP D";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xBB:
    {
        currOpcode.mnemonic = "CP E";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xBC:
    {
        currOpcode.mnemonic = "CP H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xBD:
    {
        currOpcode.mnemonic = "CP L";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xBE:
    {
        currOpcode.mnemonic = "CP (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xBF:
    {
        currOpcode.mnemonic = "CP A";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0xC0:
    {
        currOpcode.mnemonic = "RET NZ";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xC1:
    {
        currOpcode.mnemonic = "POP BC";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xC2:
    {
        currOpcode.mnemonic = "JP NZ, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xC3:
    {
        currOpcode.mnemonic = "JP " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xC4:
    {
        currOpcode.mnemonic = "CALL NZ, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xC5:
    {
        currOpcode.mnemonic = "PUSH BC";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xC6:
    {
        currOpcode.mnemonic = "ADD A, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xC7:
    {
        currOpcode.mnemonic = "RST 00H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xC8:
    {
        currOpcode.mnemonic = "RET Z";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xC9:
    {
        currOpcode.mnemonic = "RET";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xCA:
    {
        currOpcode.mnemonic = "JP Z, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xCB:
    {
        switch (mmu->read_ram(index + 1))
        {
        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x00:
        {
            currOpcode.mnemonic = "RLC B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x01:
        {
            currOpcode.mnemonic = "RLC C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x02:
        {
            currOpcode.mnemonic = "RLC D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x03:
        {
            currOpcode.mnemonic = "RLC E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x04:
        {
            currOpcode.mnemonic = "RLC H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x05:
        {
            currOpcode.mnemonic = "RLC L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x06:
        {
            currOpcode.mnemonic = "RLC (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x07:
        {
            currOpcode.mnemonic = "RLC A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x08:
        {
            currOpcode.mnemonic = "RRC B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x09:
        {
            currOpcode.mnemonic = "RRC C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x0A:
        {
            currOpcode.mnemonic = "RRC D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x0B:
        {
            currOpcode.mnemonic = "RRC E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x0C:
        {
            currOpcode.mnemonic = "RRC H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x0D:
        {
            currOpcode.mnemonic = "RRC L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x0E:
        {
            currOpcode.mnemonic = "RRC (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x0F:
        {
            currOpcode.mnemonic = "RRC A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x10:
        {
            currOpcode.mnemonic = "RL B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x11:
        {
            currOpcode.mnemonic = "RL C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x12:
        {
            currOpcode.mnemonic = "RL D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x13:
        {
            currOpcode.mnemonic = "RL E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x14:
        {
            currOpcode.mnemonic = "RL H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x15:
        {
            currOpcode.mnemonic = "RL L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x16:
        {
            currOpcode.mnemonic = "RL (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x17:
        {
            currOpcode.mnemonic = "RL A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x18:
        {
            currOpcode.mnemonic = "RR B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x19:
        {
            currOpcode.mnemonic = "RR C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x1A:
        {
            currOpcode.mnemonic = "RR D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x1B:
        {
            currOpcode.mnemonic = "RR E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x1C:
        {
            currOpcode.mnemonic = "RR H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x1D:
        {
            currOpcode.mnemonic = "RR L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x1E:
        {
            currOpcode.mnemonic = "RR (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x1F:
        {
            currOpcode.mnemonic = "RR A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x20:
        {
            currOpcode.mnemonic = "SLA B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x21:
        {
            currOpcode.mnemonic = "SLA C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x22:
        {
            currOpcode.mnemonic = "SLA D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x23:
        {
            currOpcode.mnemonic = "SLA E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x24:
        {
            currOpcode.mnemonic = "SLA H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x25:
        {
            currOpcode.mnemonic = "SLA L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x26:
        {
            currOpcode.mnemonic = "SLA (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x27:
        {
            currOpcode.mnemonic = "SLA A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x28:
        {
            currOpcode.mnemonic = "SRA B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x29:
        {
            currOpcode.mnemonic = "SRA C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x2A:
        {
            currOpcode.mnemonic = "SRA D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x2B:
        {
            currOpcode.mnemonic = "SRA E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x2C:
        {
            currOpcode.mnemonic = "SRA H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x2D:
        {
            currOpcode.mnemonic = "SRA L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x2E:
        {
            currOpcode.mnemonic = "SRA (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x2F:
        {
            currOpcode.mnemonic = "SRA A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x30:
        {
            currOpcode.mnemonic = "SWAP B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x31:
        {
            currOpcode.mnemonic = "SWAP C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x32:
        {
            currOpcode.mnemonic = "SWAP D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x33:
        {
            currOpcode.mnemonic = "SWAP E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x34:
        {
            currOpcode.mnemonic = "SWAP H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x35:
        {
            currOpcode.mnemonic = "SWAP L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x36:
        {
            currOpcode.mnemonic = "SWAP (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x37:
        {
            currOpcode.mnemonic = "SWAP A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x38:
        {
            currOpcode.mnemonic = "SRLB";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x39:
        {
            currOpcode.mnemonic = "SRLC";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x3A:
        {
            currOpcode.mnemonic = "SRLD";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x3B:
        {
            currOpcode.mnemonic = "SRLE";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x3C:
        {
            currOpcode.mnemonic = "SRLH";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x3D:
        {
            currOpcode.mnemonic = "SRLL";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x3E:
        {
            currOpcode.mnemonic = "SRL(HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x3F:
        {
            currOpcode.mnemonic = "SRLA";
            currOpcode.numberOfBytes= 2;
            break;
        }
        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x40:
        {
            currOpcode.mnemonic = "BIT 0, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x41:
        {
            currOpcode.mnemonic = "BIT 0, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x42:
        {
            currOpcode.mnemonic = "BIT 0, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x43:
        {
            currOpcode.mnemonic = "BIT 0, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x44:
        {
            currOpcode.mnemonic = "BIT 0, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x45:
        {
            currOpcode.mnemonic = "BIT 0, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x46:
        {
            currOpcode.mnemonic = "BIT 0, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x47:
        {
            currOpcode.mnemonic = "BIT 0, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x48:
        {
            currOpcode.mnemonic = "BIT 1, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x49:
        {
            currOpcode.mnemonic = "BIT 1, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x4A:
        {
            currOpcode.mnemonic = "BIT 1, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x4B:
        {
            currOpcode.mnemonic = "BIT 1, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x4C:
        {
            currOpcode.mnemonic = "BIT 1, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x4D:
        {
            currOpcode.mnemonic = "BIT 1, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x4E:
        {
            currOpcode.mnemonic = "BIT 1, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x4F:
        {
            currOpcode.mnemonic = "BIT 1, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x50:
        {
            currOpcode.mnemonic = "BIT 2, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x51:
        {
            currOpcode.mnemonic = "BIT 2, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x52:
        {
            currOpcode.mnemonic = "BIT 2, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x53:
        {
            currOpcode.mnemonic = "BIT 2, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x54:
        {
            currOpcode.mnemonic = "BIT 2, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x55:
        {
            currOpcode.mnemonic = "BIT 2, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x56:
        {
            currOpcode.mnemonic = "BIT 2, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x57:
        {
            currOpcode.mnemonic = "BIT 2, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x58:
        {
            currOpcode.mnemonic = "BIT 3, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x59:
        {
            currOpcode.mnemonic = "BIT 3, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x5A:
        {
            currOpcode.mnemonic = "BIT 3, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x5B:
        {
            currOpcode.mnemonic = "BIT 3, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x5C:
        {
            currOpcode.mnemonic = "BIT 3, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x5D:
        {
            currOpcode.mnemonic = "BIT 3, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x5E:
        {
            currOpcode.mnemonic = "BIT 3, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x5F:
        {
            currOpcode.mnemonic = "BIT 3, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x60:
        {
            currOpcode.mnemonic = "BIT 4, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x61:
        {
            currOpcode.mnemonic = "BIT 4, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x62:
        {
            currOpcode.mnemonic = "BIT 4, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x63:
        {
            currOpcode.mnemonic = "BIT 4, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x64:
        {
            currOpcode.mnemonic = "BIT 4, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x65:
        {
            currOpcode.mnemonic = "BIT 4, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x66:
        {
            currOpcode.mnemonic = "BIT 4, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x67:
        {
            currOpcode.mnemonic = "BIT 4, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x68:
        {
            currOpcode.mnemonic = "BIT 5, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x69:
        {
            currOpcode.mnemonic = "BIT 5, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x6A:
        {
            currOpcode.mnemonic = "BIT 5, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x6B:
        {
            currOpcode.mnemonic = "BIT 5, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x6C:
        {
            currOpcode.mnemonic = "BIT 5, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x6D:
        {
            currOpcode.mnemonic = "BIT 5, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x6E:
        {
            currOpcode.mnemonic = "BIT 5, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x6F:
        {
            currOpcode.mnemonic = "BIT 5, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x70:
        {
            currOpcode.mnemonic = "BIT 6, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x71:
        {
            currOpcode.mnemonic = "BIT 6, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x72:
        {
            currOpcode.mnemonic = "BIT 6, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x73:
        {
            currOpcode.mnemonic = "BIT 6, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x74:
        {
            currOpcode.mnemonic = "BIT 6, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x75:
        {
            currOpcode.mnemonic = "BIT 6, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x76:
        {
            currOpcode.mnemonic = "BIT 6, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x77:
        {
            currOpcode.mnemonic = "BIT 6, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x78:
        {
            currOpcode.mnemonic = "BIT 7, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x79:
        {
            currOpcode.mnemonic = "BIT 7, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x7A:
        {
            currOpcode.mnemonic = "BIT 7, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x7B:
        {
            currOpcode.mnemonic = "BIT 7, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x7C:
        {
            currOpcode.mnemonic = "BIT 7, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x7D:
        {
            currOpcode.mnemonic = "BIT 7, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x7E:
        {
            currOpcode.mnemonic = "BIT 7, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x7F:
        {
            currOpcode.mnemonic = "BIT 7, A";
            currOpcode.numberOfBytes= 2;
            break;
        }


        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x80:
        {
            currOpcode.mnemonic = "RES 0, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x81:
        {
            currOpcode.mnemonic = "RES 0, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x82:
        {
            currOpcode.mnemonic = "RES 0, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x83:
        {
            currOpcode.mnemonic = "RES 0, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x84:
        {
            currOpcode.mnemonic = "RES 0, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x85:
        {
            currOpcode.mnemonic = "RES 0, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x86:
        {
            currOpcode.mnemonic = "RES 0, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x87:
        {
            currOpcode.mnemonic = "RES 0, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x88:
        {
            currOpcode.mnemonic = "RES 1, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x89:
        {
            currOpcode.mnemonic = "RES 1, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x8A:
        {
            currOpcode.mnemonic = "RES 1, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x8B:
        {
            currOpcode.mnemonic = "RES 1, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x8C:
        {
            currOpcode.mnemonic = "RES 1, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x8D:
        {
            currOpcode.mnemonic = "RES 1, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x8E:
        {
            currOpcode.mnemonic = "RES 1, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x8F:
        {
            currOpcode.mnemonic = "RES 1, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0x90:
        {
            currOpcode.mnemonic = "RES 2, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x91:
        {
            currOpcode.mnemonic = "RES 2, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x92:
        {
            currOpcode.mnemonic = "RES 2, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x93:
        {
            currOpcode.mnemonic = "RES 2, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x94:
        {
            currOpcode.mnemonic = "RES 2, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x95:
        {
            currOpcode.mnemonic = "RES 2, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x96:
        {
            currOpcode.mnemonic = "RES 2, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x97:
        {
            currOpcode.mnemonic = "RES 2, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x98:
        {
            currOpcode.mnemonic = "RES 3, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x99:
        {
            currOpcode.mnemonic = "RES 3, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x9A:
        {
            currOpcode.mnemonic = "RES 3, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x9B:
        {
            currOpcode.mnemonic = "RES 3, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x9C:
        {
            currOpcode.mnemonic = "RES 3, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x9D:
        {
            currOpcode.mnemonic = "RES 3, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x9E:
        {
            currOpcode.mnemonic = "RES 3, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0x9F:
        {
            currOpcode.mnemonic = "RES 3, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0xA0:
        {
            currOpcode.mnemonic = "RES 4, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA1:
        {
            currOpcode.mnemonic = "RES 4, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA2:
        {
            currOpcode.mnemonic = "RES 4, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA3:
        {
            currOpcode.mnemonic = "RES 4, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA4:
        {
            currOpcode.mnemonic = "RES 4, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA5:
        {
            currOpcode.mnemonic = "RES 4, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA6:
        {
            currOpcode.mnemonic = "RES 4, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA7:
        {
            currOpcode.mnemonic = "RES 4, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA8:
        {
            currOpcode.mnemonic = "RES 5, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xA9:
        {
            currOpcode.mnemonic = "RES 5, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xAA:
        {
            currOpcode.mnemonic = "RES 5, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xAB:
        {
            currOpcode.mnemonic = "RES 5, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xAC:
        {
            currOpcode.mnemonic = "RES 5, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xAD:
        {
            currOpcode.mnemonic = "RES 5, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xAE:
        {
            currOpcode.mnemonic = "RES 5, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xAF:
        {
            currOpcode.mnemonic = "RES 5, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0xB0:
        {
            currOpcode.mnemonic = "RES 6, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB1:
        {
            currOpcode.mnemonic = "RES 6, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB2:
        {
            currOpcode.mnemonic = "RES 6, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB3:
        {
            currOpcode.mnemonic = "RES 6, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB4:
        {
            currOpcode.mnemonic = "RES 6, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB5:
        {
            currOpcode.mnemonic = "RES 6, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB6:
        {
            currOpcode.mnemonic = "RES 6, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB7:
        {
            currOpcode.mnemonic = "RES 6, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB8:
        {
            currOpcode.mnemonic = "RES 7, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xB9:
        {
            currOpcode.mnemonic = "RES 7, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xBA:
        {
            currOpcode.mnemonic = "RES 7, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xBB:
        {
            currOpcode.mnemonic = "RES 7, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xBC:
        {
            currOpcode.mnemonic = "RES 7, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xBD:
        {
            currOpcode.mnemonic = "RES 7, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xBE:
        {
            currOpcode.mnemonic = "RES 7, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xBF:
        {
            currOpcode.mnemonic = "RES 7, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0xC0:
        {
            currOpcode.mnemonic = "SET 0,B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC1:
        {
            currOpcode.mnemonic = "SET 0,C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC2:
        {
            currOpcode.mnemonic = "SET 0,D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC3:
        {
            currOpcode.mnemonic = "SET 0,E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC4:
        {
            currOpcode.mnemonic = "SET 0,H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC5:
        {
            currOpcode.mnemonic = "SET 0,L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC6:
        {
            currOpcode.mnemonic = "SET 0,(HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC7:
        {
            currOpcode.mnemonic = "SET 0,A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC8:
        {
            currOpcode.mnemonic = "SET 1, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xC9:
        {
            currOpcode.mnemonic = "SET 1, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xCA:
        {
            currOpcode.mnemonic = "SET 1, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xCB:
        {
            currOpcode.mnemonic = "SET 1, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xCC:
        {
            currOpcode.mnemonic = "SET 1, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xCD:
        {
            currOpcode.mnemonic = "SET 1, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xCE:
        {
            currOpcode.mnemonic = "SET 1, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xCF:
        {
            currOpcode.mnemonic = "SET 1, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0xD0:
        {
            currOpcode.mnemonic = "SET 2, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD1:
        {
            currOpcode.mnemonic = "SET 2, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD2:
        {
            currOpcode.mnemonic = "SET 2, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD3:
        {
            currOpcode.mnemonic = "SET 2, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD4:
        {
            currOpcode.mnemonic = "SET 2, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD5:
        {
            currOpcode.mnemonic = "SET 2, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD6:
        {
            currOpcode.mnemonic = "SET 2, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD7:
        {
            currOpcode.mnemonic = "SET 2, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD8:
        {
            currOpcode.mnemonic = "SET 3, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xD9:
        {
            currOpcode.mnemonic = "SET 3, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xDA:
        {
            currOpcode.mnemonic = "SET 3, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xDB:
        {
            currOpcode.mnemonic = "SET 3, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xDC:
        {
            currOpcode.mnemonic = "SET 3, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xDD:
        {
            currOpcode.mnemonic = "SET 3, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xDE:
        {
            currOpcode.mnemonic = "SET 3, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xDF:
        {
            currOpcode.mnemonic = "SET 3, A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0xE0:
        {
            currOpcode.mnemonic = "SET 4, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE1:
        {
            currOpcode.mnemonic = "SET 4, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE2:
        {
            currOpcode.mnemonic = "SET 4, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE3:
        {
            currOpcode.mnemonic = "SET 4, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE4:
        {
            currOpcode.mnemonic = "SET 4, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE5:
        {
            currOpcode.mnemonic = "SET 4, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE6:
        {
            currOpcode.mnemonic = "SET 4, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE7:
        {
            currOpcode.mnemonic = "SET 4, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE8:
        {
            currOpcode.mnemonic = "SET 5,B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xE9:
        {
            currOpcode.mnemonic = "SET 5,C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xEA:
        {
            currOpcode.mnemonic = "SET 5,D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xEB:
        {
            currOpcode.mnemonic = "SET 5,E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xEC:
        {
            currOpcode.mnemonic = "SET 5,H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xED:
        {
            currOpcode.mnemonic = "SET 5,L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xEE:
        {
            currOpcode.mnemonic = "SET 5,(HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xEF:
        {
            currOpcode.mnemonic = "SET 5,A";
            currOpcode.numberOfBytes= 2;
            break;
        }

        /*****************************************/
        /*****************************************/
        /*****************************************/
        /*****************************************/

        case 0xF0:
        {
            currOpcode.mnemonic = "SET 6, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF1:
        {
            currOpcode.mnemonic = "SET 6, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF2:
        {
            currOpcode.mnemonic = "SET 6, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF3:
        {
            currOpcode.mnemonic = "SET 6, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF4:
        {
            currOpcode.mnemonic = "SET 6, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF5:
        {
            currOpcode.mnemonic = "SET 6, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF6:
        {
            currOpcode.mnemonic = "SET 6, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF7:
        {
            currOpcode.mnemonic = "SET 6, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF8:
        {
            currOpcode.mnemonic = "SET 7, B";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xF9:
        {
            currOpcode.mnemonic = "SET 7, C";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xFA:
        {
            currOpcode.mnemonic = "SET 7, D";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xFB:
        {
            currOpcode.mnemonic = "SET 7, E";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xFC:
        {
            currOpcode.mnemonic = "SET 7, H";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xFD:
        {
            currOpcode.mnemonic = "SET 7, L";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xFE:
        {
            currOpcode.mnemonic = "SET 7, (HL)";
            currOpcode.numberOfBytes= 2;
            break;
        }
        case 0xFF:
        {
            currOpcode.mnemonic = "SET 7, A";
            currOpcode.numberOfBytes= 2;
            break;
        }
        }
        break;
    }
    case 0xCC:
    {
        currOpcode.mnemonic = "CALL Z, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xCD:
    {
        currOpcode.mnemonic = "CALL " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xCE:
    {
        currOpcode.mnemonic = "ADC A, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xCF:
    {
        currOpcode.mnemonic = "RST 08H";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0xD0:
    {
        currOpcode.mnemonic = "RET NC";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xD1:
    {
        currOpcode.mnemonic = "POP DE";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xD2:
    {
        currOpcode.mnemonic = "JP NC, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xD3:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xD4:
    {
        currOpcode.mnemonic = "CALL NC, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xD5:
    {
        currOpcode.mnemonic = "PUSH DE";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xD6:
    {
        currOpcode.mnemonic = "SUB " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xD7:
    {
        currOpcode.mnemonic = "RST 10H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xD8:
    {
        currOpcode.mnemonic = "RET C";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xD9:
    {
        currOpcode.mnemonic = "RETI";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xDA:
    {
        currOpcode.mnemonic = "JP C, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xDB:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xDC:
    {
        currOpcode.mnemonic = "CALL C, " + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xDD:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xDE:
    {
        currOpcode.mnemonic = "SBC A, " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xDF:
    {
        currOpcode.mnemonic = "RST 18H";
        currOpcode.numberOfBytes = 1;
        break;
    }

    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0xE0:
    {
        currOpcode.mnemonic = "LDH (" + int2hex((int)mmu->read_ram(index + 1)) + "), A";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xE1:
    {
        currOpcode.mnemonic = "POP HL";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xE2:
    {
        currOpcode.mnemonic = "LD (C), A";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xE3:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xE4:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xE5:
    {
        currOpcode.mnemonic = "PUSH HL";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xE6:
    {
        currOpcode.mnemonic = "AND " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xE7:
    {
        currOpcode.mnemonic = "RST 20H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xE8:
    {
        currOpcode.mnemonic = "ADD SP, r";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xE9:
    {
        currOpcode.mnemonic = "JP (HL)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xEA:
    {
        currOpcode.mnemonic = "LD (" + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1)) + "), A";
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xEB:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xEC:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xED:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xEE:
    {
        currOpcode.mnemonic = "XOR " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xEF:
    {
        currOpcode.mnemonic = "RST 28H";
        currOpcode.numberOfBytes = 1;
        break;
    }


    /*****************************************/
    /*****************************************/
    /*****************************************/
    /*****************************************/

    case 0xF0:
    {
        currOpcode.mnemonic = "LDH A, (" + int2hex((int)mmu->read_ram(index + 1)) +")";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xF1:
    {
        currOpcode.mnemonic = "POP AF";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xF2:
    {
        currOpcode.mnemonic = "LD A, (C)";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xF3:
    {
        currOpcode.mnemonic = "DI";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xF4:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xF5:
    {
        currOpcode.mnemonic = "PUSH AF";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xF6:
    {
        currOpcode.mnemonic = "OR " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xF7:
    {
        currOpcode.mnemonic = "RST 30H";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xF8:
    {
        currOpcode.mnemonic = "LD HL, SP+r";
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xF9:
    {
        currOpcode.mnemonic = "LD SP, HL";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xFA:
    {
        currOpcode.mnemonic = "LD A, (" + int2hex((int)mmu->read_ram(index + 2)) + int2hex((int)mmu->read_ram(index + 1)) + ")";
        currOpcode.numberOfBytes = 3;
        break;
    }
    case 0xFB:
    {
        currOpcode.mnemonic = "EI";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xFC:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xFD:
    {
        currOpcode.mnemonic = "ILLEGAL INSTRUCTION";
        currOpcode.numberOfBytes = 1;
        break;
    }
    case 0xFE:
    {
        currOpcode.mnemonic = "CP " + int2hex((int)mmu->read_ram(index + 1));
        currOpcode.numberOfBytes = 2;
        break;
    }
    case 0xFF:
    {
        currOpcode.mnemonic = "RST 38H";
        currOpcode.numberOfBytes = 1;
        break;
    }

    default:
    {
        currOpcode.mnemonic = "Unknow opcode";
        currOpcode.numberOfBytes = 1;
    }
    break;
    }
    return currOpcode;

}
