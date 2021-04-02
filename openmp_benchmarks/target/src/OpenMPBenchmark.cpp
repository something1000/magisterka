#include<iostream>
#include "BenchEngine.hpp"
#include <memory>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;

int main(int argc, char *argv[]) {
   if (argc < 2) { //argument passed
      std::cerr << "Provide descriptors for benchmarks!";
      return -1;
   }
   auto file = argv[1];
   std::ifstream ifs {file};
   if (!ifs.is_open()) {
      std::cerr << "Could not open file for reading!\n";
      return -1;
   }
   
   IStreamWrapper isw {ifs};
   std::shared_ptr<Document> doc = std::make_shared<Document>();
   doc->ParseStream( isw );

   BenchEngine::Start(doc);

   return 0;
}