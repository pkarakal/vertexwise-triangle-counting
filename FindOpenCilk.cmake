#.rst:
# FindOpenCilk
# --------
# Finds the OpenCilk library
#
# This will define the following variables::
#
# OPEN_CILK_FOUND - system has dav1d
# OPEN_CILK_INCLUDE_DIRS - the dav1d include directories
# OPEN_CILK_LIBRARIES - the dav1d libraries

find_library(OPEN_CILK_LIBRARY NAMES cilk
        PATHS /usr/local/OpenCilk-9.0.1-Linux/lib)

find_path(OPEN_CILK_INCLUDE_DIR NAMES cilk/cilk.h cilk/cilk_api.h
        PATHS /usr/local/OpenCilk-9.0.1-Linux/lib/clang/9.0.1/include)

set(DAV1D_VERSION 9.0.1})


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenCilk
        REQUIRED_VARS OPEN_CILK_LIBRARY OPEN_CILK_INCLUDE_DIR
        VERSION_VAR OPEN_CILK_VERSION)

set(DAV1D_INCLUDE_DIRS /usr/local/OpenCilk-9.0.1-Linux/lib/clang/9.0.1/include)
set(DAV1D_LIBRARIES /usr/local/OpenCilk-9.0.1-Linux/lib)

mark_as_advanced(OPEN_CILK_INCLUDE_DIR OPEN_CILK_LIBRARY)
