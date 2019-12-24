#! /bin/bash


opt -load build/lib/LLVMDPOR.dylib -dpor < test1.bc -o test1new.bc
clang++ rtlib.cpp test1new.bc -o instrumented
llvm-dis test1new.bc
./instrumented
#lli test1new.bc