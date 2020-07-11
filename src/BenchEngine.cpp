#include <iostream>
#include "BenchEngine.hpp"
#include "common.hpp"

uint BenchEngine::warmupRounds;
uint BenchEngine::executionRounds;

void BenchEngine::Start(BenchmarkMap benchmarks){
    std::fstream excelStream("excelraport.csv", std::ios::out | std::ios::app );
    auto excel = Logger::EXCEL(excelStream);

    Logger::INFO << "Starting benchmark engine!";
    Logger::INFO << "Benchmarks settings:\n"
                 << " \t- warmup rounds: "    << warmupRounds << "\n"
                 << " \t- execution rounds: " << executionRounds;

    Logger::INFO << "Executing following benchmarks:";
    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << " \t- " << name;
    }

    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << "[" << name << "] " << "Initializing benchmark";
        bench->Init();
        Logger::INFO << "[" << name << "] " << "Starting benchmark";
        //START TIMER
        //START FUNCTION IN LOOP
        //STOP TIMER
        Logger::INFO << "[" << name << "] " << "Finished benchmark" << "\n"
                     << "\t - Execution time: " << 832.32 << "ms" << "\n"
                     << "\t - Single iteration time: " << 8.32 << "ms" << "\n";
       excel << "Column" << 1 << 32.2 << "end";
    }
    
}