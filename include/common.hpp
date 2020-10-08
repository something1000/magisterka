#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <assert.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include "Benchmark.hpp"
#include "Logger.hpp"

#define PUT_BENCHMARK(NAME) {#NAME, std::make_shared<NAME>(#NAME)}

typedef std::shared_ptr<Benchmark> BenchmarkPtr;
typedef std::unordered_map<std::string, BenchmarkPtr> BenchmarkMap;
typedef unsigned int uint;

template<class T>
T** Create2DArray(int N, int M) {
    //Temporary pointer for storing continous data
    T* rawData = new T[N*M];

    T** array2D = new T*[N];
    for(int i=0; i < N; i++) {
        array2D[i] = rawData;
        rawData += M;
    }

    return array2D;
}

template<class T>
T*** Create3DArray(int N, int M, int K) {
    T*** array3D = new T**[N]; // 5 
    T** array2D = new T*[N*M]; // 25
    T* rawData = new T[N*M*K]; // 75

    for(int i=0; i < N; i++) { //5
        array3D[i] = array2D;
        for(int j=0; j < M; j++) {
            array2D[j] = rawData;   
            rawData += K;
        }
        array2D++;
    }

    return array3D;
} 
inline void FillRandomArray(float* arr, int N) {
    std::srand(std::time(nullptr));
    for(int i=0; i < N; i++) {
        arr[i] = static_cast<float>(std::rand()) / RAND_MAX;
    }
}

inline void FillRandom2DArray(float** arr, int N, int M) {
    std::srand(std::time(nullptr));
    for(int i=0; i < N; i++) {
        for(int j=0; j < M; j++) {
            arr[i][j] = static_cast<float>(std::rand()) / RAND_MAX;
        }
    }
}

inline void FillRandom3DArray(float*** arr, int N, int M, int K) {
    std::srand(std::time(nullptr));
    for(int i=0; i < N; i++) {
        for(int j=0; j < M; j++) {
            for(int l=0; l < K; l++) {
                arr[i][j][l] = static_cast<float>(std::rand()) / RAND_MAX;
            }
        }
    }
}

inline void Print2DArray(float** arr, int N, int M) {
    for(int i=0; i < N; i++) {
        for(int j=0; j < M; j++){
            std::cout << arr[i][j] << " ";
        }
        std::cout << "\n";
    }
}


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
        _Excel << this->name  << _Mode << _Warmup << _Rounds << _Elapsed;                         \
        Logger::INFO << _Mode << " Warmup:" << _Warmup \
                     << " Rounds: " << _Rounds << " Time: " << _Elapsed;
#endif