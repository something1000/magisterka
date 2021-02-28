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
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/istreamwrapper.h>
#include <stdlib.h>

#define PUT_BENCHMARK(NAME) {#NAME, std::make_shared<NAME>(#NAME)}
#define VAR(X) #X":" << X << " "

typedef std::shared_ptr<Benchmark> BenchmarkPtr;
typedef std::unordered_map<std::string, BenchmarkPtr> BenchmarkMap;
typedef unsigned int uint;
typedef rapidjson::Document Json;
typedef std::shared_ptr<rapidjson::Document> JsonPtr;

template <class T>
using Tensor4D = T****;

template <class T>
using Tensor3D = T***;

template <class T>
using Tensor2D = T**;

template<class T>
T** Create2DArray(int H, int W) {
    T** array2D = new T*[H];
    int size = H*W*sizeof(T);
    T* rawData = (T*) _aligned_malloc(size, 128);//new T[H*W];

    for(int i=0; i < H; i++) {
        array2D[i] = rawData;
        rawData += W;
    }

    return array2D;
}

template<class T>
T*** Create3DArray(int C, int H, int W) {
    T*** array3D = new T**[C]; 
    T** array2D = new T*[C*H];
    int size = C*H*W*sizeof(T);
    T* rawData = (T*) _aligned_malloc(size, 128);//new T[C*H*W];

    for(int i=0; i < C; i++) {
        array3D[i] = array2D;
        for(int j=0; j < H; j++) {
            array2D[j] = rawData;   
            rawData += W;
        }
        array2D += H;
    }

    return array3D;
}

template<class T>
T**** Create4DArray(int N, int C, int H, int W) {
    T**** array4D = new T***[N];
    T*** array3D = new T**[N*C]; 
    T** array2D = new T*[N*C*H];
    int size = N*C*H*W*sizeof(T);
    T* rawData = (T*) _aligned_malloc(size, 128);//new T[N*C*H*W];


    for(int i=0; i < N; ++i) {
        array4D[i] = array3D;
        for(int j=0; j < C; ++j) {
            array3D[j] = array2D;
            for(int z=0; z < H; ++z) {
                array2D[z] = rawData;   
                rawData += W;
            }
            array2D += H;
        }
        array3D += C;
    }

    return array4D;
}

template <class T> 
void Free2DArray(T** ptr) {
    delete[] ptr[0];
    delete[] ptr;
};

template <class T> 
void Free3DArray(T*** ptr) {
    delete[] ptr[0][0]; 
    delete[] ptr[0];
    delete[] ptr;
};

template <class T> 
void Free4DArray(T**** ptr) {
    delete[] ptr[0][0][0]; 
    delete[] ptr[0][0]; 
    delete[] ptr[0];
    delete[] ptr;
};


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

inline void FillRandom4DArray(float**** arr, int N, int M, int K, int O) {
    std::srand(std::time(nullptr));
    for(int i=0; i < N; i++) {
        for(int j=0; j < M; j++) {
            for(int l=0; l < K; l++) {
                 for(int z=0; z < O; z++) {
                    arr[i][j][l][z] = static_cast<float>(std::rand()) / RAND_MAX;
                 }
            }
        }
    }
}

inline void PrintArray(float* arr, int N) {
    for(int i=0; i < N; i++) {
        std::cout << (float)arr[i] << " ";
    }
}

inline void Print2DArray(float** arr, int N, int M) {
    for(int i=0; i < N; i++) {
        for(int j=0; j < M; j++){
            std::cout << (float)arr[i][j] << " ";
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
// TODO: Wyliczenie odchylenia standardowego