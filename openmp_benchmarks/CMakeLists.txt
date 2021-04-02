cmake_minimum_required(VERSION 2.8.9)
project(OpenMPBench)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE OpenMPBench)
set(CMAKE_BUILD_TYPE Debug)

option(BUILD_OFFLOADING "Build with offloading support" OFF)


#Bring basic benchmarks
include_directories(include)
include_directories(3rdparty/rapidjson/include)
file(GLOB SOURCES "src/*.cpp")
add_executable(${EXECUTABLE} ${SOURCES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${EXECUTABLE} PUBLIC OpenMP::OpenMP_CXX)
endif()

#build offloading benchmarks
if(BUILD_OFFLOADING)
    set(OFFLOAD_EXEC OpenMPOffloadBench)
    file(GLOB OFFLOAD_SOURCE "target/src/*.cpp")
    add_executable(${OFFLOAD_EXEC} ${OFFLOAD_SOURCE})

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${OFFLOAD_EXEC} PUBLIC -fopenmp-targets=nvptx64-nvidia-cuda)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${OFFLOAD_EXEC} PUBLIC -foffload=nvptx-none)
    endif()

    if(OpenMP_CXX_FOUND)
        target_link_libraries(${OFFLOAD_EXEC} PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif()