#include "BenchTimer.hpp"

void BenchTimer::Tic() {
    this->start_time = std::chrono::high_resolution_clock::now();
}

void BenchTimer::Toc() {
    this->end_time = std::chrono::high_resolution_clock::now();
}

float BenchTimer::GetDuration(){
    if(start_time.time_since_epoch().count() == 0 ||
       end_time.time_since_epoch().count() == 0   ||
       start_time > end_time) {
           Logger::ERROR << "Duration can't be negative and timer must be started!";
           assert(false);
       }
    std::chrono::duration<double> duration_ = this->end_time - this->start_time;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_);
    return ms.count();
}

