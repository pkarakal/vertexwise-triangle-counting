#.rst:
# FindOpenCilk
# --------
# Finds the OpenCilk library
#
# This will define the following variables::
#
# OPEN_CILK_INCLUDE_DIRS - the OPEN_CILK include directories
# OPEN_CILK_LIBRARIES - the OPEN_CILK libraries

#find_library(OPEN_CILK_LIBRARY NAMES libopencilk cilks
#        PATHS /usr/local/OpenCilk-9.0.1-Linux/lib/clang/9.0.1/lib/x86_64-unknown-linux-gnu)

find_path(OPEN_CILK_INCLUDE_DIR NAMES cilk.h cilk_api.h
        PATHS /usr/local/OpenCilk-9.0.1-Linux/lib/clang/9.0.1/include/cilk)

set(OPEN_CILK_VERSION 9.0.1})


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenCilk
        REQUIRED_VARS OPEN_CILK_LIBRARY OPEN_CILK_INCLUDE_DIR
        VERSION_VAR OPEN_CILK_VERSION)

set(OPEN_CILK_INCLUDE_DIRS /usr/local/OpenCilk-9.0.1-Linux/lib/clang/9.0.1/include/cilk)
set(OPEN_CILK_LIBRARIES /usr/local/OpenCilk-9.0.1-Linux/lib)


mark_as_advanced(OPEN_CILK_INCLUDE_DIR OPEN_CILK_LIBRARY)

add_custom_target(triangle_v3_cilk ALL
        COMMAND gcc -c mmio.c mmio.h -O3
        COMMAND gcc -c coo2csc.c coo2csc.h -O3
        COMMAND /usr/local/OpenCilk-9.0.1-Linux/bin/clang++ triangle_v3_cilk.cpp mmio.o coo2csc.o
        -o triangle_v3_cilk -fopencilk -O3)

add_custom_target(triangle_v4_cilk ALL
        COMMAND gcc -c mmio.c mmio.h -O3
        COMMAND gcc -c coo2csc.c coo2csc.h -O3
        COMMAND /usr/local/OpenCilk-9.0.1-Linux/bin/clang++ triangle_v4_cilk.cpp mmio.o coo2csc.o
        -o triangle_v4_cilk -fopencilk -O3)
