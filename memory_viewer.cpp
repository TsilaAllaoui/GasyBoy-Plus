#include "memory_viewer.h"

Memory_viewer::Memory_viewer(Mmu *pmmu)
{
    rows = 28;
    mmu = pmmu;
    rom_size = mmu->get_romSize();
}

Memory_viewer::~Memory_viewer()
{
    //dtor
}

void Memory_viewer::Render()
{
    memory_type = BIOS;
    //Create a new window with name "Memory Viewer", with size of 640,150 with some windowsflags
    if (ImGui::Begin("Memory Viewer", nullptr, ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SetWindowPos(ImVec2(0,450)); //Set window position to x,y
        ImGui::SetWindowSize(ImVec2(1024,574));
        float CharWidth = ImGui::CalcTextSize("F").x;                                //get "F" character font size
        float cellWidth = CharWidth * 3;                                             //3 * "F" font size
        float lineHeight = ImGui::GetTextLineHeight();                               //height of one line
        lineTotalCount = (int)((rom_size + bios_size + vram_size + extram_size + wram_size + 1) / rows);                           //count number of line
        ImGuiListClipper clipper(lineTotalCount, lineHeight);                        //create scrolling clipper

        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)              //iterating through clipper length
        {
            int addr = i * rows;
            if (addr <= bios_size)                                               //current address to show on leftmost part ex: "00FE: "
            {
                memory_type = BIOS;
                ImGui::Text("BOOT");
                ImGui::SameLine(37);
                ImGui::Text("%04X: ", addr);
            }
            else if (addr > bios_size && addr <= rom_size + bios_size)
            {
                memory_type = ROM;
                ImGui::Text("ROM");
                ImGui::SameLine(37);
                ImGui::Text("%04X: ", addr - bios_size - 1);                //show addr in 4 digits hex
            }

            else if (addr > rom_size + bios_size && addr <= rom_size + bios_size + vram_size)
            {
                memory_type = VRAM;
                ImGui::Text("VRAM");
                ImGui::SameLine(37);
                ImGui::Text("%04X: ", addr - bios_size - rom_size - 1);                //show addr in 4 digits hex
            }
            else if (addr > bios_size + rom_size + vram_size && addr <= rom_size + bios_size + vram_size + extram_size)
            {
                memory_type = ERAM;
                ImGui::Text("ERAM");
                ImGui::SameLine(37);
                ImGui::Text("%04X: ", addr - bios_size - rom_size - vram_size - 1);                //show addr in 4 digits hex
            }
            else if (addr > bios_size + rom_size + vram_size + extram_size && addr <= rom_size + bios_size + vram_size + extram_size + wram_size)
            {
                memory_type = WRAM;
                ImGui::Text("WRAM");
                ImGui::SameLine(37);
                ImGui::Text("%04X: ", addr - bios_size - rom_size - vram_size - extram_size - 1);                //show addr in 4 digits hex
            }
            ImGui::SameLine();                                                       //staying on the same line

            float lineStartX = ImGui::GetCursorPosX();                               //get position of the start of the line

            for (int j = 0; j < rows ; j++)                                          //iterating through number of rows
            {
                ImGui::SameLine(lineStartX + cellWidth * j);                         //staying on the same line but start next text at the parameter
                if (memory_type == BIOS)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
                    ImGui::Text("%02X", mmu->get_bios()[addr]);
                }
                else if(memory_type == ROM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1));
                    ImGui::Text("%02X", mmu->get_rom()[addr - bios_size - 1]);
                }
                 else if(memory_type == VRAM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
                    ImGui::Text("%02X", mmu->get_vram()[addr - bios_size - rom_size - 1]);
                }
                 else if(memory_type == ERAM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1));
                    ImGui::Text("%02X", mmu->get_vram()[addr - bios_size - rom_size - vram_size - 1]);
                }
                else if(memory_type == WRAM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
                    ImGui::Text("%02X", mmu->get_vram()[addr - bios_size - rom_size - vram_size - extram_size - 1]);
                }
                ImGui::PopStyleColor();
                addr++;                                                          //incrementing addr if not at EOF
            }
            ImGui::SameLine();
            ImGui::Text("|");
            ImGui::SameLine(lineStartX + cellWidth * rows + CharWidth * 2 + 10);          //adding space on same line

			addr = i * rows;                                                              //the same as above but showing character instead of hex
			for (int j = 0; j < rows; j++)
			{
				if (j > 0) ImGui::SameLine();
				int c;
				if (memory_type == BIOS)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
                    c = mmu->get_bios()[addr];
                }
                else if (memory_type == ROM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1));
                    c = mmu->get_rom()[addr - bios_size - 1];
                }
                else if (memory_type == VRAM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
                    c = mmu->get_vram()[addr - bios_size - rom_size - 1];
                }
                else if (memory_type == ERAM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1));
                    c = mmu->get_extram()[addr - bios_size - rom_size - vram_size - 1];
                }
                else if (memory_type == WRAM)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
                    c = mmu->get_extram()[addr - bios_size - rom_size - vram_size - extram_size - 1];
                }
				ImGui::Text("%c", (c >= 32 && c < 128) ? c : '.');                          //showing a dot if is not within (32, 128) and printable ascii character if within
                addr++;
                ImGui::PopStyleColor();
			}
        }
        clipper.End();
    }
    ImGui::End();
}
