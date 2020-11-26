cmake_minimum_required(VERSION 3.17)
project(vertexwise_triangle_counting LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
if (NOT VERBOSE)
    set(CMAKE_REQUIRED_QUIET ON)
endif ()


option(ENABLE_CLANGTIDY "Enable clang-tidy support?" OFF)
option(ENABLE_OPENCILK "Enable OpenCilk support?" OFF)
option(ENABLE_OPENMP "Enable OpenMP support?" OFF)

add_library(coo2csc OBJECT coo2csc.c coo2csc.h)
add_library(mmio OBJECT mmio.h mmio.c)

find_package(PkgConfig)

if (ENABLE_CLANGTIDY)
    include(FindClangTidy.cmake)
endif ()

if (ENABLE_OPENCILK)
    include(FindOpenCilk.cmake)
endif ()

if (ENABLE_OPENMP)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    add_executable(triangle_v3_openmp triangle_v3_openmp.cpp)
    find_package(OpenMP)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(triangle_v3_openmp PUBLIC OpenMP::OpenMP_CXX mmio coo2csc)
    endif ()
endif ()

add_compile_options(-O3)
add_executable(triangle_v3_serial triangle_v3_serial.cpp)
target_link_libraries(triangle_v3_serial PUBLIC mmio coo2csc)
