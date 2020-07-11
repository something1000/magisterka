#include <iostream>
#include "BenchEngine.hpp"
#include "BenchTimer.hpp"
#include "common.hpp"

uint BenchEngine::warmupRounds;
uint BenchEngine::executionRounds;

void BenchEngine::Start(BenchmarkMap benchmarks){
    std::fstream excelStream("excelraport.csv", std::ios::out);
    auto excel = Logger::EXCEL(excelStream);

    Logger::INFO << "Starting benchmark engine!";
    Logger::INFO << "Benchmarks settings:\n"
                 << " \t- warmup rounds: "    << warmupRounds << "\n"
                 << " \t- execution rounds: " << executionRounds;

    Logger::INFO << "Executing following benchmarks:";
    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << " \t- " << name;
    }

    // Add benchmark info to file log
    excel << "Rounds" << executionRounds << "Warmup" << warmupRounds;
    excel.newLine(); excel.newLine();

    // Create Excel Columns Titles
    excel << "Name" << "Mode" << "Duration" << "Avg. duration" << "Mode" << "Duration" << "Avg. duration";
    excel.newLine();

    auto timer = BenchTimer(); // create timer
    for(auto& [name, bench] : benchmarks) {
        Logger::INFO << "[" << name << "] " << "Initializing benchmark";
        bench->Init();
        Logger::INFO << "[" << name << "] " << "Starting benchmark";


        /* Benchmark Parallel Function With OpenMP */
        timer.Tic();

        for(int i=0; i < warmupRounds; ++i){
            bench->RunParallel();
        }
        for(int i=0; i < executionRounds; ++i){
            bench->RunParallel();
        }

        timer.Toc();

        auto duration_parallel = timer.GetDuration();
        auto parallel_iteration = duration_parallel / executionRounds;
        Logger::INFO << "[" << name << "] " << "Finished parallel benchmark" << "\n"
                     << "\t - Execution time: " << duration_parallel << "ms" << "\n"
                     << "\t - Single iteration time: " << duration_parallel/executionRounds << "ms" << "\n";


        /* Benchmark Serial Function */
        timer.Tic();

        for(int i=0; i < warmupRounds; ++i){
            bench->RunSerial();
        }
        for(int i=0; i < executionRounds; ++i){
            bench->RunSerial();
        }

        timer.Toc();

        auto duration_serial = timer.GetDuration();
        auto serial_iteration = duration_serial / executionRounds;
        Logger::INFO << "[" << name << "] " << "Finished serial benchmark" << "\n"
                     << "\t - Execution time: " << duration_serial << "ms" << "\n"
                     << "\t - Single iteration time: " << serial_iteration << "ms" << "\n";

        /* Save benchmarks result to csv file */
        excel << name << "Serial"   << duration_serial   << serial_iteration
                      << "Parallel" << duration_parallel << parallel_iteration;
        excel.newLine();

        Logger::INFO << "==============================================================";
    }
}