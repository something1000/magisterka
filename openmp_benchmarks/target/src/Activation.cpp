#include "Activation.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

void Activation::RunParallel() {
    RunParallel_2();
    RunParallel_3();
}

void Activation::RunParallel_1() {
    // RunParallel_1 is not designed to run on gpu
}

void Activation::RunParallel_2() {
    auto excel = *this->file;

    auto fn = [&]() {
        int size = N*C*H*W;
        int _static_size = static_size; // fix for gcc bug - cant use class fields inside pragma region
        float* raw_input = input[0][0][0];
        float* raw_output = output[0][0][0];
        #pragma omp target teams distribute parallel for schedule(static, _static_size) \
                map(tofrom:raw_input[0:size]) map(tofrom:raw_output[0:size])
            for(int i=0; i < size; i++) {
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
    };

    BenchmarkIt(excel, "PARALLEL_FOR", warmup, rounds, fn);
}


void Activation::RunParallel_3() {
    auto excel = *this->file;

    auto fn = [&]() {
        int size = N*C*H*W;
        float* raw_input = input[0][0][0];
        float* raw_output = output[0][0][0];
        #pragma omp target teams distribute parallel for simd \
                map(tofrom:raw_input[0:size]) map(tofrom:raw_output[0:size])
            for(int i=0; i < size; i++) {
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
    };

    BenchmarkIt(excel, "PARALLEL_FOR_SIMD", warmup, rounds, fn);
}

void Activation::RunSerial() {
    auto excel = *this->file;

    auto fn = [&]() {
        int size = N*C*H*W;
        float* raw_input = input[0][0][0];
        float* raw_output = output[0][0][0];
        for(int i=0; i < size; i++) {
            raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
        }
    };
    BenchmarkIt(excel, "SERIAL", warmup, rounds, fn);
}

bool Activation::Validate() {
    Tensor4D<float> out_serial = Create4DArray<float>(N, C, H, W);
   // Tensor4D<float> out_parallel_1 = Create4DArray<float>(N, C, H, W);
    Tensor4D<float> out_parallel_2 = Create4DArray<float>(N, C, H, W);
    Tensor4D<float> out_parallel_3 = Create4DArray<float>(N, C, H, W);
    rounds = 1;
    warmup = 0;

    Tensor4D<float> tmp = output;

    output = out_serial;
    RunSerial();

    // output = out_parallel_1;
    // RunParallel_1();

    output = out_parallel_2;
    RunParallel_2();

    output = out_parallel_3;
    RunParallel_3();

    bool is_valid = Compare4DArray(out_serial, out_parallel_2, N, C, H, W);
    is_valid = is_valid && Compare4DArray(out_serial, out_parallel_3, N, C, H, W);
    // is_valid = is_valid && Compare4DArray(out_serial, out_parallel_3, N, C, H, W);

    output = tmp;
    Free4DArray<float>(out_serial);
    // Free4DArray<float>(out_parallel_1);
    Free4DArray<float>(out_parallel_2);
    Free4DArray<float>(out_parallel_3);
    return is_valid;
}

void Activation::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();

    N =  properties["N"].GetInt();
    C =  properties["C"].GetInt();
    H =  properties["H"].GetInt();
    W =  properties["W"].GetInt();
    static_size = properties["static_size"].GetInt();
    Logger::INFO << VAR(N) << VAR(C) << VAR(H) << VAR(W) << VAR(static_size);

    Reinitialize();
}

void Activation::Reinitialize() {
    if(initialized) {
        Free4DArray<float>(input);
        Free4DArray<float>(output);
    }

    input = Create4DArray<float>(N, C, H, W);
    output = Create4DArray<float>(N, C, H, W);

    FillRandomArray(input[0][0][0], N*C*H*W);
    this->initialized = true;
}

static std::shared_ptr<Benchmark> CreateBench() {
    return std::make_shared<Activation>("Activation");
}

REGISTER_BENCHMARK(Activation, CreateBench);