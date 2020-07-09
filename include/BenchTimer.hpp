#ifndef _BENCHTIMER_HPP
#define _BENCHTIMER_HPP
#include <chrono>
#include <iostream>

class BenchTimer {
    public:
        void Tic();
        void Tock();
        void Print();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    
};

#endif