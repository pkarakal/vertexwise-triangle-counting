cmake_minimum_required(VERSION 3.17)
project(vertexwise_triangle_counting LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
if (NOT VERBOSE)
    set(CMAKE_REQUIRED_QUIET ON)
endif ()


option(ENABLE_CLANGTIDY "Enable clang-tidy support?" OFF)
option(ENABLE_OPENCILK "Enable OpenCilk support?" OFF)
option(ENABLE_OPENMP "Enable OpenMP support?" OFF)

find_package(PkgConfig)

if (ENABLE_CLANGTIDY)
    include(FindClangTidy.cmake)
endif ()

if (ENABLE_OPENCILK)
    include(FindOpenCilk.cmake)
    set(CMAKE_C_COMPILER /usr/local/OpenCilk-9.0.1-Linux/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/local/OpenCilk-9.0.1-Linux/bin/clang++)
    add_executable(triangle_v3_cilk triangle_v3_cilk.cpp mmio.h mmio.c coo2csc.h coo2csc.c)
endif ()

if (ENABLE_OPENMP)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    add_executable(triangle_v3_openmp triangle_v3_openmp.cpp mmio.h mmio.c coo2csc.h coo2csc.c)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(triangle_v3_openmp PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif ()

add_executable(triangle_v3_serial triangle_v3_serial.cpp mmio.h mmio.c coo2csc.h coo2csc.c)
