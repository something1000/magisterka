#include "BenchTimer.hpp"

void BenchTimer::Tic() {
    this->start_time = std::chrono::high_resolution_clock::now();
}

void BenchTimer::Tock() {
    this->end_time = std::chrono::high_resolution_clock::now();
}

void BenchTimer::Print(){
    std::chrono::duration<double> duration_ = this->end_time - this->start_time;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_);
    std::cout << "---" << ms.count() << "ms ---\n";
}

