#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <assert.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include "Benchmark.hpp"
#include "Logger.hpp"

#define PUT_BENCHMARK(NAME) {#NAME, std::make_shared<NAME>(#NAME)}

typedef std::shared_ptr<Benchmark> BenchmarkPtr;
typedef std::unordered_map<std::string, BenchmarkPtr> BenchmarkMap;
typedef unsigned int uint;


#define TIME(name) auto name = omp_get_wtime(); 

#define LOOP_UNOPTIMIZER(var) __asm__ volatile("" : "+g" (var) : :);

#define mpragma(...)  _Pragma(#__VA_ARGS__)

#define BENCHMARK_STRUCTURE(_Excel, _Mode, _Warmup, _Rounds, _Elapsed, ...)                       \
        int unoptimizer = 0;                                                                      \
        for(int warmup_i=0; warmup_i < _Warmup; warmup_i++){                                      \
            {__VA_ARGS__}                                                                         \
            LOOP_UNOPTIMIZER(unoptimizer)                                                         \
        }                                                                                         \
        auto start = omp_get_wtime();                                                             \
        for(int round_i=0; round_i < _Rounds; round_i++){                                         \
            {__VA_ARGS__}                                                                         \
            LOOP_UNOPTIMIZER(unoptimizer)                                                         \
        }                                                                                         \
        auto end = omp_get_wtime();                                                               \
        auto _Elapsed = end - start;                                                              \
        _Excel << this->name << _Warmup << _Rounds << _Elapsed;                                   \
        Logger::INFO << _Mode << " Warmup:" << _Warmup \
                     << " Rounds: " << _Rounds << " Time: " << _Elapsed;
#endif