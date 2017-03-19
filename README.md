This code is a *wrapper* for the **Dynamic Creator Mersenne Twister**, which is a *thread safe PRNG* with *very large periods*.

Having *DCMT library* is a *prerequisite*. You can obtain the version 0.6.3. You need to have the header "dc.h" and the library "libdcmt.a" in a location which is in the path. The library is supposed to be linked with -ldcmt.

You can compile "example.c" in src/, you can either use OpenMP flags, or may not use it. The code will compile nevertheless. Without OpenMP you will not have multithreading.

You'd find the required libraries [Here](https://github.com/MersenneTwister-Lab/dcmt).

Compilation:

`$ CC -O2 src/example.c -I./include -ldcmt -fopenmp -lm -o ./example`
