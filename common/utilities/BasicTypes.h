#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <random>
#include "Params.h"

namespace sim
{
    namespace types
    {
        class rand // only temporary
        {
        public:
            rand(){};
            ~rand(){};

            int get_rand(size_t, size_t);
        };

        struct ConsoleLayout
        {
            int _nxpos = 0;
            int _nypos = 0;
            int _nScreenWidth = 60;
            int _nScreenHeight = 60;
            int _fontw = 8;
            int _fonth = 16;
        };
    }
}