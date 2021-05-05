#include "gameboy.h"

Gameboy::Gameboy()
{
    mmu = new Mmu();
    cpu = new Cpu(mmu);
    debugger = new Debugger(mmu, cpu);
}

Gameboy::~Gameboy()
{

}

void Gameboy::step()
{
    cpu->cpuStep();
}

Mmu *Gameboy::get_mmu()
{
    return mmu;
}

Debugger *Gameboy::get_debugger()
{
    return debugger;
}
