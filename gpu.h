#pragma once



#include "defs.h"

class mmu;

class Gpu_listener;

class Gpu
{
    private:
        Mmu *mmu;
        std::vector <Gpu_listener> gpuListeners;
        uint8_t palette[4]{0};
        uint32_t *screen = nullptr;
        bool dirty;
    public:
        Gpu(){};
        Gpu(Mmu *pmmu);
        ~Gpu();
        void addListener(Gpu_listener *listener);
        void onMemoryWritten16(uint16_t addr, uint16_t value);
        void onMemoryWritten8(uint16_t addr, uint8_t value);
        uint32_t *get_screen();
};

