#pragma once

#include "defs.h"

class Mmu;

class Stack_viewer
{
    private:
        Mmu *mmu;
        Cpu *cpu;
    public:
        Stack_viewer(Cpu *pcpu, Mmu *pmmu);
        ~Stack_viewer();
        void Render();
};
