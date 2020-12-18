#include <iostream>
#include "BenchEngine.hpp"

#include <stdexcept>

using JsonBenchmark = rapidjson::Value::ConstValueIterator;

void BenchEngine::Start(JsonPtr json) {
    std::fstream excelStream("excelraport.csv", std::ios::out);
    auto excel = Logger::EXCEL(excelStream);

    Logger::INFO << "Starting benchmark engine!";
    Logger::INFO << "Executing following benchmarks:";
    
    for (JsonBenchmark itr = json->Begin(); itr != json->End(); ++itr) {
         Logger::INFO << " \t- " << itr->GetObject()["name"].GetString();
    }

    // Create Excel Columns Titles
    excel << "Name" << "Warmup" << "Iterations" << "Duration";
    excel.newLine();

    for (JsonBenchmark itr = json->Begin(); itr != json->End(); ++itr) {
        auto benchDescriptor = itr->GetObject();
        auto name = benchDescriptor["name"].GetString();
        auto bench = GetBenchmark(name);
        const rapidjson::Value& properties = benchDescriptor["properties"];

        Logger::INFO << "[" << name << "] " << "Initializing benchmark";
        bench->Init(&excel, properties);
        Logger::INFO << "[" << name << "] " << "Starting benchmark";

        /* Benchmark Parallel Function With OpenMP */
        bench->RunParallel();
        Logger::INFO << "[" << name << "] " << "Finished parallel benchmark";

        /* Benchmark Serial Function */
        bench->RunSerial();
        Logger::INFO << "[" << name << "] " << "Finished serial benchmark";
        excel.newLine();
        Logger::INFO << "==============================================================";
     }
}


std::shared_ptr<Benchmark> BenchEngine::GetBenchmark(std::string name) {
    std::shared_ptr<Benchmark> ptr;
    if (name == "EmptyForLoopBenchmark")
        ptr = std::make_shared<EmptyForLoopBenchmark>(name);
    else if (name == "MatrixMultiplication")
        ptr = std::make_shared<MatrixMultiplication>(name);
    else if (name == "Convolution2D")
        ptr = std::make_shared<Convolution2D>(name);
    else if (name == "QuickSort")
        ptr = std::make_shared<QuickSort>(name);
    else if (name == "BatchNorm")
        ptr = std::make_shared<BatchNorm>(name);
    else if (name == "QuantizeTensor")
        ptr = std::make_shared<QuantizeTensor>(name);
    else if (name == "CooleyTukeyFFT")
        ptr = std::make_shared<CooleyTukeyFFT>(name);
    else if (name == "WaveEquation")
        ptr = std::make_shared<WaveEquation>(name);
    else
        throw std::invalid_argument("No such benchmark as " + name);
    
    return ptr;
}
