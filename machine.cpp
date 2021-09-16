#include "machine.h"

Machine::Machine()
{
    window = SDL_CreateWindow("GasyBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH << 1, HEIGHT << 1, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Machine::~Machine()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Machine::Update()
{
    //TODO cpu steps if is not on debug mode
}

void Machine::Render()
{
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}
