#include "Activation.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

void Activation::RunParallel() {
    RunParallel_1();
    RunParallel_2();
}


void Activation::RunParallel_1() {
    auto excel = *this->file;
    std::stringstream os;
    os << VAR_(size) << VAR_(static_size) << "PARALLEL";
    std::string name = os.str();

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        name,       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for schedule(static, static_size))
            for(int i=0; i < size; i++) {
                input[i] = input[i] > 0 ? input[i] : 0;
            }
        }
   )
}


void Activation::RunParallel_2() {
    auto excel = *this->file;
    std::stringstream os;
    os << VAR_(size) << VAR_(static_size) << "PARALLEL_SIMD";
    std::string name = os.str();

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        name,       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for simd schedule(static, static_size))
            for(int i=0; i < size; i++) {
                input[i] = input[i] > 0 ? input[i] : 0;
            }
        }
   )
}

void Activation::RunSerial() {
    auto excel = *this->file;
    std::stringstream os;
    os << VAR_(size) << VAR_(static_size);
    std::string descriptor = os.str();
    std::string mode = "SERIAL";
    
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        descriptor,
        mode,       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i=0; i < size; i++) {
                input[i] = input[i] > 0 ? input[i] : 0;
            }
        }
   )
}

bool Activation::Validate() {
    float* out_serial = new float[size];
    float* out_parallel_1 = new float[size];
    float* out_parallel_2 = new float[size];
    rounds = 1;
    warmup = 0;

    float* tmp = output;

    output = out_serial;
    RunSerial();

    output = out_parallel_1;
    RunParallel_1();

    output = out_parallel_2;
    RunParallel_2();


    bool is_valid = CompareArray(out_serial, out_parallel_1, size);
    is_valid &= CompareArray(out_serial, out_parallel_2, size);

    output = tmp;
    delete[] out_serial;
    delete[] out_parallel_1;
    delete[] out_parallel_2;

    return is_valid;
}

void Activation::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();

    size =  properties["size"].GetInt();
    static_size = properties["static_size"].GetInt();
    Logger::INFO << VAR(size) << VAR(static_size);

    Reinitialize();
}

void Activation::Reinitialize() {
    if(initialized) {
        delete[] input;
        delete[] output;
    }

    input = new float[size];
    output = new float[size];

    FillRandomArray(input, size);
    this->initialized = true;
}

static std::shared_ptr<Benchmark> CreateBench() {
    return std::make_shared<Activation>("Activation");
}

REGISTER_BENCHMARK(Activation, CreateBench);