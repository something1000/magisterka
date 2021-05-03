#include "PrimeTest.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <functional>

bool CheckPrime(int value) {
    if (value <= 3) {
        return value > 1;
    }

    if (value % 2 == 0 || value % 3 == 0) {
        return false;
    }

    int i = 5;
    while (i * i <= value) {
        if(value % i == 0 || value % (i + 2) == 0) {
            return false;
        }
        i += 6;
    };
    return true;
}

void PrimeTest::RunParallel() {
    RunParallel_1();
    RunParallel_2();
    RunParallel_3();
}


void PrimeTest::RunParallel_1() {
    auto excel = *this->file;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_GUIDED",       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for schedule(guided))
            for(int i=0; i < size; i++) {
                output[i] = CheckPrime(input[i]);
            }
        }
   )
}

void PrimeTest::RunParallel_2() {
    auto excel = *this->file;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_DYNAMIC",       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for schedule(dynamic))
            for(int i=0; i < size; i++) {
                output[i] = CheckPrime(input[i]);
            }
        }
   )
}


void PrimeTest::RunParallel_3() {
    auto excel = *this->file;

    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "PARALLEL_STATIC",       // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            mpragma(omp parallel for schedule(static))
            for(int i=0; i < size; i++) {
                output[i] = CheckPrime(input[i]);
            }
        }
   )
}

void PrimeTest::RunSerial() {
    auto excel = *this->file;
    
    BENCHMARK_STRUCTURE(
        excel,      // name of csv logger
        "SERIAL",   // name of benchmark
        warmup,     // name of warmup rounds variable
        rounds,     // name of benchmark rounds variable
        ELAPSED,    // variable name to store execution time
        {
            for(int i=0; i < size; i++) {
                output[i] = CheckPrime(input[i]);
            }
        }
   )
}

bool PrimeTest::Validate() {

    auto CompareBoolArray = [](bool* arr1, bool* arr2, int N) {
        bool result = true;
        int err_cnt = 0;
        for(int i=0; i < N; i++) {
            if(arr1[i] != arr2[i]) {
                Logger::ERROR << "Boolean value not equal: arr1" << IND(i) << " = " << arr1[i]
                            << " and arr2" << IND(i) << " = " << arr2[i];
                result = false;
                if (err_cnt++ >= 10)
                    return result;
            }
        }
        return result;
    };

    bool* out_serial = new bool[size];
    bool* out_parallel_1 = new bool[size];
    bool* out_parallel_2 = new bool[size];
    bool* out_parallel_3 = new bool[size];
    rounds = 1;
    warmup = 0;

    bool* tmp = output;

    output = out_serial;
    RunSerial();

    output = out_parallel_1;
    RunParallel_1();

    output = out_parallel_2;
    RunParallel_2();

    output = out_parallel_3;
    RunParallel_3();


    bool is_valid = CompareBoolArray(out_serial, out_parallel_1, size);
    is_valid &= CompareBoolArray(out_serial, out_parallel_2, size);
    is_valid &= CompareBoolArray(out_serial, out_parallel_3, size);

    output = tmp;
    delete[] out_serial;
    delete[] out_parallel_1;
    delete[] out_parallel_2;
    delete[] out_parallel_3;

    return is_valid;
}

void PrimeTest::Init(Logger::LoggerClass* file, const rapidjson::Value& properties) {
    this->file = file;
    rounds = properties["rounds"].GetInt();
    warmup = properties["warmup"].GetInt();

    size =  properties["size"].GetInt();
    Logger::INFO << VAR(size);

    std::stringstream os;
    os << VAR_(size);
    descriptor = os.str();

    Reinitialize();
}

void PrimeTest::Reinitialize() {
    if(initialized) {
        delete[] input;
        delete[] output;
    }

    input = new int[size];
    output = new bool[size];

    FillRandomArray(input, size, 0, 100000);
    this->initialized = true;
}

static std::shared_ptr<Benchmark> CreateBench() {
    return std::make_shared<PrimeTest>("PrimeTest");
}

REGISTER_BENCHMARK(PrimeTest, CreateBench);