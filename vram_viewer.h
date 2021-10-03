#pragma once

#include "defs.h"
#include <SDL2/SDL_opengl.h>

class Mmu;


class Vram_viewer
{
    private:
        Mmu *mmu;
        uint32_t textureArray[256 * 256];
        GLuint textureID;
    public:
        Vram_viewer(Mmu *pmmu);
        ~Vram_viewer();
        void Render();
        void BuildTileData(uint16_t value);
        void RenderTile(uint16_t tileAddr, uint8_t tileNum, uint8_t x, uint8_t y);
        void Init();
};