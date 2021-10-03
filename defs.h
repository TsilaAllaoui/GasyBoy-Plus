#pragma once

#include "cpu.h"
#include "register.h"
#include "mmu.h"
#include "debugger.h"
#include "gameboy.h"
#include "memory_viewer.h"
#include "register_viewer.h"
#include "instruction_viewer.h"
#include "vram_viewer.h"
#include "utilities.h"
#include "machine.h"
#include "stack_viewer.h"
#include "timer.h"
#include "gpu.h"
#include "gpu_listener.h"
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
#define BIOS_SIZE 0xFF
#define VARM_SIZE 0x2000
#define ERAM_SIZE 0x2000
#define WRAM_SIZE 0x4000

/*****TEMPORARY DIMENSION*****/
/* TODO: CHANGE TO REAL GAMMEBOY RESOLUTION */
#define HEIGHT_TEMP 768
#define WIDTH_TEMP 1024

enum{RUNNING, STOPPED, RESETED};

