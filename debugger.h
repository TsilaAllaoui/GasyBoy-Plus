#pragma once

#include "defs.h"

#include <SDL2/SDL.h>

class Memory_viewer;
class Mmu;
class Register_viewer;
class Cpu;
class Instruction_viewer;
class Stack_viewer;

class Debugger
{
    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;

        Cpu *cpu;
        Mmu *mmu;

        Memory_viewer *memview;
        Instruction_viewer *instrViewer;
        Register_viewer *reg_viewer;
        Stack_viewer *stackView;

    public:
        Debugger(Mmu *pmmu, Cpu *pcpu);
        ~Debugger();
        void HandleEvent(SDL_Event event);
        void Render();
        void step();
};
