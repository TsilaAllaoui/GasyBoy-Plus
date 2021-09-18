#pragma once

#include "defs.h"

class Mmu;

class Vram_viewer
{
    private:
        Mmu *mmu;
    public:
        Vram_viewer(Mmu *pmmu);
        ~Vram_viewer();
        void Render();
};