# Vertex-wise triangle counting
This project is implemented as a part of a homework exercise for [050] - Parallel & 
Distributed Systems course of ECE Department, AUTh. 

This has implementations written in C and C++ for detecting and counting triangles in 
K3 graphs. 

## Getting Started

### Prerequisites
1. CMake
2. Make
3. g++
4. OpenCilk
5. Python (3.6 or higher)


### Instructions
1.  Clone the repository
    ```shell script
    $ git clone git@github.com:pkarakal/vertexwise-triangle-counting.git
    ```
2.  Go to that directory
    ```shell script
    $ cd vertexwise-triangle-counting/
    ```
3.  a. Generate Makefiles from the CMakefiles.txt
    ```shell script
    $ cmake -S .
    ```
    b. There are additional cmake files that import OpenCilk and change the 
    compiler to use the OpenCilk clang compiler and there is also the option
    to include ClangTidy support. By default, all the options are disabled. 
    To enable the build of the executable that leverages OpenCilk run 
    ```shell script
    $ cmake -S . -DENABLE_OPENCILK=ON
    ``` 
    To enable the build of the executable that leverages OpenMP run 
    ```
    $ cmake -S . -DENABLE_OPENMP=ON
    ```
    To enable the build of the executable that leverages PThreads run
    ```shell script
    $ cmake -S . -DENABLE_PTHREADS=ON
    ```
    To enable Clang Tidy support use the following flag
    ```shell script
    $ cmake -S . -DENABLE_CLANGTIDY=ON
    ```
4.  Build and run the application
    ```shell script
    $ cmake --build . && ./triangle_v{version}_{variant}
    ```
    where valid variants are `cilk`, `openmp`, `serial`, `pthreads`,
    and valid versions are 3 and 4.
    By default, only the serial is built
5.  To run the formula validation first install numpy using
    ```shell script
    $ pip3 install numpy
    ```
    or install it from the requirements.txt file running
    ```shell script
    $ pip3 install -r requirements.txt
    ```
    and run the python script by executing the following command
    ```shell script
    $ python3 ./formula-validation/validate.py
    ```
