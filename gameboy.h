#pragma once
#include "defs.h"

class Mmu;
class Cpu;
class Debugger;
class Gpu;

class Gameboy
{
    private:
        Mmu *mmu;
        Cpu *cpu;
        Debugger *debugger;
        Gpu *gpu = nullptr;
    public:
        Gameboy();
        ~Gameboy();
        void step();
        Mmu *get_mmu();
        Cpu *get_cpu();
        Gpu *get_gpu();
        Debugger *get_debugger();
};

