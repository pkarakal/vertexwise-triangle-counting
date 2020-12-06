#.rst:
# FindPthreads
# --------
# Finds the Pthreads library and adds new target
#

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_library(Graph OBJECT Graph.cpp Graph.hpp)
add_executable(triangle_v4_pthreads triangle_v4_pthreads.cpp)
target_link_libraries(triangle_v4_pthreads PUBLIC Threads::Threads mmio coo2csc Graph)
add_compile_options(-O3)