#.rst:
# FindPthreads
# --------
# Finds the Pthreads library
#
# This will define the following variables::
#
# OPEN_CILK_FOUND - system has OPEN_CILK
# OPEN_CILK_INCLUDE_DIRS - the OPEN_CILK include directories
# OPEN_CILK_LIBRARIES - the OPEN_CILK libraries

#find_library(OPEN_CILK_LIBRARY NAMES libopencilk cilks
#        PATHS /usr/local/OpenCilk-9.0.1-Linux/lib/clang/9.0.1/lib/x86_64-unknown-linux-gnu)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_library(Graph OBJECT Graph.cpp Graph.hpp)
add_executable(triangle_v4_pthreads triangle_v4_pthreads.cpp)
target_link_libraries(triangle_v4_pthreads PUBLIC Threads::Threads mmio coo2csc Graph)
add_compile_options(-O3)