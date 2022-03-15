#pragma once
#include <cstdint>
#include <vector>
#include "Params.h"
#include "BasicTypes.h"

namespace sim
{
    class Environment
    {   
    public:
        Environment();
        ~Environment();

    private:  
        std::vector<sim::types::Map<sim::params::MapType>> m_maps;
    
    };
}