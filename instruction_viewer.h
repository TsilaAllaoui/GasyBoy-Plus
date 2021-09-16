#pragma once

#include "defs.h"
#include <map>


class Cpu;
class mmu;

struct opcode
{
    int numberOfBytes;
    std::string mnemonic;
};

class CpuListener
{
	public:
	     CpuListener(){};
	     void OnStep(){};
};

class Instruction_viewer : public CpuListener
{
    private:

        Cpu *cpu;
        Mmu *mmu;
        int selectedLine = -1;
        int selectedBkLine = -1;
        int currLine = 0;
        int prevLine = 0;
        opcode currOpcode;
        std::map<uint16_t,int> mapInstr2line;
        std::vector <opcode> instrList;
        std::vector <int> breakPointList;

    public:
        Instruction_viewer(Cpu *pcpu, Mmu *pmmu);
        ~Instruction_viewer(){};
        void Render();
        opcode GetopcodeInfo(int index);
        void showLine(int *index);
        void OnStep();
        void fillMap();
        void mouseEvent();

};
