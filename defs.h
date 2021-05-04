#pragma once

#include "cpu.h"
#include "register.h"
#include "mmu.h"
#include "debugger.h"
#include "gameboy.h"
#include "memory_viewer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define MAXCYCLE 70224
#define WIDTH 160
#define HEIGHT 144
#define VRAM_WIDTH 128
#define VRAM_HEIGHT 192
#define BG_MAP_WIDTH 256
#define BG_MAP_HEIGTH 256
#define PX 2
#define SPEED 3.6

/*****TEMPORARY DIMENSION*****/
/* TODO: CHANGE TO REAL GAMMEBOY RESOLUTION */
#define HEIGHT_TEMP 480
#define WIDTH_TEMP 640
