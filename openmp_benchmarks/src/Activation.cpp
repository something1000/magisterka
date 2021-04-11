#include "Activation.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

void Activation::RunParallel() {
    RunParallel_1();
    RunParallel_2();
    RunParallel_3();
}



void Activation::RunParallel_1() {
    auto excel = *this->file;
    std::stringstream os;
    os << VAR_(N) << VAR_(C) << VAR_(H) << VAR_(W) << VAR_(static_size) << "PARALLEL_COLLAPSE";
    std::string name = os.str();

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        name,       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for collapse(4) schedule(static, static_size))
            for(int j=0; j < N; ++j) {
                for(int z=0; z < C; ++z) {
                    for(int k=0; k < H; ++k) {
                        for(int p=0; p < W; ++p) {
                            output[j][z][k][p] = input[j][z][k][p] > 0 ? input[j][z][k][p] : 0;
                        }
                    }
                }
            }
        }
   )
}

void Activation::RunParallel_2() {
    auto excel = *this->file;
    std::stringstream os;
    os << VAR_(N) << VAR_(C) << VAR_(H) << VAR_(W) << VAR_(static_size) << "PARALLEL";
    std::string name = os.str();

    float* raw_input = input[0][0][0];
    float* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        name,       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            int size = N*C*H*W;
            mpragma(omp parallel for schedule(static, static_size))
            for(int i=0; i < size; i++) {
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
        }
   )
}


void Activation::RunParallel_3() {
    auto excel = *this->file;
    std::stringstream os;
    os << VAR_(N) << VAR_(C) << VAR_(H) << VAR_(W) << VAR_(static_size) << "PARALLEL_SIMD";
    std::string name = os.str();

    float* raw_input = input[0][0][0];
    float* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        name,       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            int size = N*C*H*W;
            mpragma(omp parallel for simd)
            for(int i=0; i < size; i++) {
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
        }
   )
}

void Activation::RunSerial() {
    auto excel = *this->file;
    std::stringstream os;
    os << VAR_(N) << VAR_(C) << VAR_(H) << VAR_(W) << VAR_(static_size) << "SERIAL";
    std::string name = os.str();

    float* raw_input = input[0][0][0];
    float* raw_output = output[0][0][0];
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        name,       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i=0; i < N*C*H*W; i++) {
                raw_output[i] = raw_input[i] > 0 ? raw_input[i] : 0;
            }
        }
   )
}

bool Activation::Validate() {
    Tensor4D<float> out_serial = Create4DArray<float>(N, C, H, W);
    Tensor4D<float> out_parallel_1 = Create4DArray<float>(N, C, H, W);
    Tensor4D<float> out_parallel_2 = Create4DArray<float>(N, C, H, W);
    Tensor4D<float> out_parallel_3 = Create4DArray<float>(N, C, H, W);
    rounds = 1;
    warmup = 0;

    Tensor4D<float> tmp = output;

    output = out_serial;
    RunSerial();

    output = out_parallel_1;
    RunParallel_1();

    output = out_parallel_2;
    RunParallel_2();

    output = out_parallel_3;
    RunParallel_3();

    bool is_valid = Compare4DArray(out_serial, out_parallel_1, N, C, H, W);
    is_valid = is_valid && Compare4DArray(out_serial, out_parallel_2, N, C, H, W);
    is_valid = is_valid && Compare4DArray(out_serial, out_parallel_3, N, C, H, W);

    output = tmp;
    Free4DArray<float>(out_serial);
    Free4DArray<float>(out_parallel_1);
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