#ifndef _BENCHENGINE_HPP
#define _BENCHENGINE_HPP
#include "common.hpp"
#include "Benchmark.hpp"
#include "EmptyForLoopBenchmark.hpp"
#include "MatrixMultiplication.hpp"
#include "Convolution2D.hpp"
#include "QuickSort.hpp"
#include "BatchNorm.hpp"
#include "QuantizeTensor.hpp"
#include "CooleyTukeyFFT.hpp"
#include "WaveEquation.hpp"
 
class BenchEngine {
    public:
        static void Start(JsonPtr json);
        static BenchmarkPtr GetBenchmark(std::string name); 
    private:

};
#endif