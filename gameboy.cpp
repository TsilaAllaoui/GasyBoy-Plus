#include "gameboy.h"

Gameboy::Gameboy()
{
    mmu = new Mmu();
    cpu = new Cpu(mmu);
    debugger = new Debugger(mmu, cpu);
    gpu = new Gpu(mmu);
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

Cpu *Gameboy::get_cpu()
{
    return cpu;
}

Gpu *Gameboy::get_gpu()
{
    return gpu;
}


Debugger *Gameboy::get_debugger()
{
    return debugger;
}
