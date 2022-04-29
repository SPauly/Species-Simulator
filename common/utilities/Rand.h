#pragma once
#include <chrono>
#include <random>

namespace sim
{
    /*temporary solution*/
    template<typename T>
    T rand(const T min, const T max)
    {
        std::uniform_real_distribution<> _dist(min, max);
        std::random_device rand_dev;
        std::default_random_engine rand_eng(rand_dev());
        return _dist(rand_eng);
    };
}