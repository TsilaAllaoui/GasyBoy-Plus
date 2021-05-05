#pragma once

#include "defs.h"

class Cpu;
class mmu;

class Instruction_viewer
{
    private:
        struct opcode
        {
            int numberOfBytes;
            std::string mnemonic;
        };
        Cpu *cpu;
        Mmu *mmu;
    public:
        Instruction_viewer(Cpu *pcpu, Mmu *pmmu);
        ~Instruction_viewer();
        void Render();
        void opcodeInfo(int *index);
};
