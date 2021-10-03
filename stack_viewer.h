#pragma once

#include "defs.h"

class Mmu;

class Stack_viewer
{
    private:
        Mmu *mmu;
        Cpu *cpu;
        uint16_t *SP;
        int index;
    public:
        Stack_viewer(Cpu *pcpu, Mmu *pmmu);
        ~Stack_viewer();
        void Render(Mmu *pmmu);
};
