#pragma once

#include "defs.h"
#include <SDL2/SDL.h>

class Machine
{
    private:
        SDL_Window *window;
        SDL_GLContext glcontext;
        SDL_Renderer *renderer;
    public:
        Machine();
        ~Machine();
        void Render();
        void Update();
};
