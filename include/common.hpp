#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <assert.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include "Benchmark.hpp"
#include "Logger.hpp"

#define PUT_BENCHMARK(NAME) {#NAME, std::make_shared<NAME>()}

typedef std::shared_ptr<Benchmark> BenchmarkPtr;
typedef std::unordered_map<std::string, BenchmarkPtr> BenchmarkMap;
typedef unsigned int uint;


#define TIME(name) auto name = omp_get_wtime(); 

#endif