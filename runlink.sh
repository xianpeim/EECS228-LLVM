#! /bin/bash


cc -c rtlib.c
/usr/local/opt/llvm/bin/clang -Xclang -load -Xclang build/lib/LLVMDPOR.dylib test1.c
#clang -Xclang -load -Xclang build/lib/LLVMDPOR.dylib test1.c
cc test1.o rtlib.o
./a.out