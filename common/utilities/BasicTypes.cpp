#include "BasicTypes.h"

namespace sim
{
    namespace types
    {
        int rand::get_rand(size_t min, size_t max)
        {
            std::uniform_int_distribution<> _distribution(min, max);
            std::random_device _random_dev;
            std::default_random_engine _generator(_random_dev());
            int _rand = _distribution(_generator);
            return _rand;
        }
    }
}