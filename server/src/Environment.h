#pragma once
#include <cstdint>
#include "../common/utilities/Params.h"
#include "../common/utilities/BasicTypes.h"

namespace sim
{
    class Environment : public sim::Map<sim::params::MapType>
    {   
    public:
        Environment();
        ~Environment();

    private:  
    
    };
}