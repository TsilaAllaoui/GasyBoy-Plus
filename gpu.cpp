#include "gpu.h"

static uint32_t palette[4] {0x98BC0FFF, 0x8BAC0FFF, 0x306230FF, 0x0F380FFF};

Gpu::Gpu(Mmu *pmmu)
{
    mmu = pmmu;
    dirty = true;
    screen = new uint32_t[160*144];
}

Gpu::~Gpu()
{
    //dtor
}

void Gpu::addListener(Gpu_listener* listener)
{
    //gpuListeners.push_back(listener); //TODO Fix push back error here
}
void Gpu::onMemoryWritten16(uint16_t addr, uint16_t value)
{

}

void Gpu::onMemoryWritten8(uint16_t addr, uint8_t value)
{
    switch (addr)
    {
        case 0xFF47: // BG & Window Palette Data
            palette[0] = (value & 0b11);
            palette[1] = (value >> 2) & 0b11;
            palette[2] = (value >> 4) & 0b11;
            palette[3] = (value >> 6) & 0b11;
            dirty = true;
        break;
    }

    if (addr >= 0x8000 && addr <= 0x9FFF)
        dirty = true;
}

uint32_t *Gpu::get_screen()
{
    if (dirty)
        dirty = false;
    return screen;
}
