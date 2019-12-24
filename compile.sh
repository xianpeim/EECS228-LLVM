#! /bin/bash

cd build
ninja
cd ..
clang -Xclang -pthread -emit-llvm test1.c -c -o test1.bc
llvm-dis test1.bc
clang -pthread test1.c -o test1