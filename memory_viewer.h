#pragma once
#include "defs.h"

class Mmu;


class Memory_viewer
{
    private:
        enum {BIOS, ROM, VRAM, ERAM, WRAM};
        const int bios_size = 0xFF;
        const int vram_size = 0x2000;
        const int extram_size = 0x2000;
        const int wram_size = 0x4000;
        int rom_size;
        int rows;
        int lineTotalCount;
        int memory_type;
        Mmu *mmu;
    public:
        Memory_viewer(){};
        Memory_viewer(Mmu *pmmu);
        ~Memory_viewer();
        void Render();
};
