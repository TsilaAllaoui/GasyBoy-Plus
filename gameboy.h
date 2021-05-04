#pragma once
#include "defs.h"

class Mmu;
class Cpu;
class Debugger;

class Gameboy
{
    private:
        Mmu *mmu;
        Cpu *cpu;
        Debugger *debugger;
    public:
        Gameboy();
        ~Gameboy();
        void step();
        Mmu *get_mmu();
        Debugger *get_debugger();
};

