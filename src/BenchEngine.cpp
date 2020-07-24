#include <iostream>
#include "BenchEngine.hpp"
#include "common.hpp"

void BenchEngine::Start(BenchmarkMap benchmarks){
    std::fstream excelStream("excelraport.csv", std::ios::out);
    auto excel = Logger::EXCEL(excelStream);

    Logger::INFO << "Starting benchmark engine!";
    Logger::INFO << "Executing following benchmarks:";
    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << " \t- " << name;
    }

    // Create Excel Columns Titles
    excel << "Name" << "Warmup" << "Iterations" << "Mode" << "Duration" << "Avg. duration" << "Mode" << "Duration" << "Avg. duration";
    excel.newLine();

    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << "[" << name << "] " << "Initializing benchmark";
        bench->Init(&excel);
        Logger::INFO << "[" << name << "] " << "Starting benchmark";

        /* Benchmark Parallel Function With OpenMP */
        bench->RunParallel();
        Logger::INFO << "[" << name << "] " << "Finished parallel benchmark";

        /* Benchmark Serial Function */
        bench->RunSerial();
        Logger::INFO << "[" << name << "] " << "Finished serial benchmark";
        excel.newLine();
        Logger::INFO << "==============================================================";
    }
}