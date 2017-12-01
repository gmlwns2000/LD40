#pragma once
#include "util/types.h"

namespace ldjam
{
    struct WindowConfig
    {
        int x;
        int y;
        uint16 width;
        uint16 height;
    };

    namespace config
    {
        WindowConfig LoadWindow(const char* filename);
        void WriteWindow(WindowConfig cfg, FILE* fp);
    }
}