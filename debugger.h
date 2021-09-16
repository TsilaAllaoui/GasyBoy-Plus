#pragma once

#include "defs.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Memory_viewer;
class Mmu;
class Register_viewer;
class Cpu;
class Instruction_viewer;
class Stack_viewer;

class Debugger
{
    private:
        SDL_GLContext glcontext;
        SDL_Window *window = nullptr;
        Memory_viewer *memview;
        Cpu *cpu;
        Mmu *mmu;
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
