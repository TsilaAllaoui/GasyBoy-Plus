#pragma once

#include "defs.h"

class Cpu;

class Register_viewer
{
    private:
        Cpu *cpu;
    public:
        Register_viewer(Cpu *pcpu);
        ~Register_viewer();
        void Render();
};
