# README: Building librefBLAS.so for Grading

## 1. Compile Each .cpp Source File into Object Files

g++ -std=c++17 -fPIC -c ref_daxpy.cpp   -o ref_daxpy.o
g++ -std=c++17 -fPIC -c ref_dgemv.cpp   -o ref_dgemv.o
g++ -std=c++17 -fPIC -c ref_dgemm.cpp   -o ref_dgemm.o
g++ -std=c++17 -fPIC -c ref_axpyt.cpp   -o ref_axpyt.o
g++ -std=c++17 -fPIC -c ref_gemvt.cpp   -o ref_gemvt.o
g++ -std=c++17 -fPIC -c ref_gemmt.cpp   -o ref_gemmt.o

The file `refBLAS.hpp` serves as the master header file and contains the 
declarations for all functions implemented across the `.cpp` source files. It includes 
or references the specifications from each of the individual `.hpp` headers 
(e.g., `ref_dgemv.hpp`, `ref_dgemm.hpp`, etc.).


## 2. Create the Shared Object Library librefBLAS.so

g++ -shared -o librefBLAS.so ref_daxpy.o ref_dgemv.o ref_dgemm.o ref_axpyt.o ref_gemvt.o ref_gemmt.o

## 3. Compilation Commands for Grading

To compile the test code used for grading, use the following commands:

g++ -std=c++17 -o xtestrefdblas -DDBLS -I. k8r_refBLAS.cpp -L. -lrefBLAS
g++ -std=c++17 -o xtestreftblas -DTBLS -I. k8r_refBLAS.cpp -L. -lrefBLAS
g++ -std=c++17 -o xtesterrors  -DTST  -I. k8r_refBLAS.cpp -L. -lrefBLAS

This assumes:
- The shared library `librefBLAS.so` is in the current directory or library path.
- The header file `refBLAS.hpp` is in the current directory.
- All source and header files are in the same folder.

## 4. Run (Ensure the Loader Finds librefBLAS.so)

Before running the tests, set the `LD_LIBRARY_PATH` to the current directory:

export LD_LIBRARY_PATH=.
./xtestrefdblas
./xtestreftblas
./xtesterrors




## Note

This README includes the compilation instructions only.
All files mentioned above — `.cpp` source files, `.hpp` header, `.o` object files, `librefBLAS.so`, 
and this README — are included in the submission folder.
