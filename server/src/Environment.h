#pragma once
#include <cstdint>
#include "../common/utilities/Params.h"
#include "../common/utilities/BasicTypes.h"

namespace sim
{
    template <typename T>
    class Environment : public sim::Map<sim::params::MapType>
    {   
    public:
        Environment(T = sim::params::MapType::Environment);
        ~Environment();

    private:  

    };
}