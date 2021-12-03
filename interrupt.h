#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <iostream>

#include "mmu.h"
#include "cpu.h"

class Cpu;
class Mmu;

class Interrupter
{
    private:
        bool MasterInterrupt;
        Mmu *mmu;
        Cpu *cpu;
    public:
        Interrupter(Mmu *pmmu, Cpu *pcpu);
        ~Interrupter();
        void handleInterrupts();
        void requestInterrupt(int id);
        void serviceInterrupt(int id);
};

#endif // INTERRUPT_H
