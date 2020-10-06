#include<iostream>
#include "BenchEngine.hpp"
#include <memory>
#include "Benchmark.hpp"
#include "EmptyForLoopBenchmark.hpp"
#include "MatrixMultiplication.hpp"
#include "Convolution2D.hpp"


static BenchmarkMap AllBenchmarks {
    PUT_BENCHMARK(EmptyForLoopBenchmark),
    PUT_BENCHMARK(MatrixMultiplication)
    PUT_BENCHMARK(Convolution2D)
};

int main(int argc, char *argv[]) {
   BenchmarkMap benchmarksToRun;
   if (argc > 1) { //argument passed
      for(int i=1; i < argc; i++) {
         auto name = argv[i];
         if (AllBenchmarks.find(name) != AllBenchmarks.end()) {
            BenchmarkPtr b = std::move(AllBenchmarks.at(name));
            benchmarksToRun.insert_or_assign(name, b);
         }
      }
   } else {
      benchmarksToRun = AllBenchmarks;
   }

   BenchEngine::Start(benchmarksToRun);

   return 0;
}