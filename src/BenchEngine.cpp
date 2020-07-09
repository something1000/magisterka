#include <iostream>
#include "BenchEngine.hpp"
#include "common.hpp"

uint BenchEngine::warmupRounds;
uint BenchEngine::executionRounds;

void BenchEngine::Start(BenchmarkMap benchmarks){
    Logger::INFO << "Starting benchmark engine!";

    Logger::INFO << "Benchmarks settings:\n"
                 << " \t- warmup rounds: "    << warmupRounds << "\n"
                 << " \t- execution rounds: " << executionRounds;


    Logger::INFO << "Executing following benchmarks:";
    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << " \t- " << name;
    }


    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << "Initializing benchmark '" << name
                     << "' [warmup: " << warmupRounds << ", rounds: " << executionRounds << "]";
        
        bench->Init();

    }
    
}