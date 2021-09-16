#pragma once

#ifndef GPU_LISTENER
#define GPU_LISTENER

#include "defs.h"

class Gpu_listener
{
    public:
        virtual void onMemoryWritten() = 0;
};

#endif // GPU_LISTENER
