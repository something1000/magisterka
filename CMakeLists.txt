cmake_minimum_required(VERSION 2.8.9)
project(OpenMPBench)
set(CMAKE_CXX_STANDARD 17)

set(TARGET OpenMPBench)

set(CMAKE_BUILD_TYPE Debug)


#Bring the headers, such as Student.h into the project
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(${TARGET} ${SOURCES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${TARGET} PUBLIC OpenMP::OpenMP_CXX)
endif()

#cmake -rmG "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..