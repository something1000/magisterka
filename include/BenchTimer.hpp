#ifndef _BENCHTIMER_HPP
#define _BENCHTIMER_HPP
#include <chrono>
#include "common.hpp"

class BenchTimer {
    public:
        void Tic();
        void Toc();
        float GetDuration();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
};

#endif