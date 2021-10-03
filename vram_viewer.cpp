#include "vram_viewer.h"

static uint32_t palette[4] = {0x000000FF, 0x404040FF, 0x7F7F7FFF, 0xFFFFFFFF};

Vram_viewer::Vram_viewer(Mmu *pmmu)
{
    mmu = pmmu;
    textureID = 0;
    for (int i=0; i<256*256; i++)
        textureArray[i] = 0xFFFF00FF;//FF00FF;
}

void Vram_viewer::Init()
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
        
        //For first tailemap

        BuildTileData(0x8000);
        ImGui::Image((ImTextureID)textureID, ImVec2(256,256));
    }
    ImGui::End();
}

void Vram_viewer::BuildTileData(uint16_t value)
{
    int r = 0;
    int c = 0;
    for (int i=0; i<256; i++)
    {
        RenderTile(value, i, c *8, r*8);
        c++;
        if (c == 16)
        {
            c  = 0;
            r++;
        }
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGR, GL_UNSIGNED_BYTE, textureArray);
}

void Vram_viewer::RenderTile(uint16_t tileDataAddr, uint8_t tileNum, uint8_t x, uint8_t y)
{

    uint16_t tileAddr = tileDataAddr + (tileNum * 16);
    for (int i = 0; i < 8; i++)
    {
        uint8_t b1 = mmu->read_ram(tileAddr);
        tileAddr++;
        uint8_t b2 = mmu->read_ram(tileAddr);
        tileAddr++;
        for (int j = 7; j>= 0; j--)
        {
           uint8_t cp = ((b1 >> j) << 1) || (b2 >> j);
           uint32_t c = palette[cp]; 
           textureArray[((y + i) * 256) + x + (7 - j)] = c;
        }
    }
}