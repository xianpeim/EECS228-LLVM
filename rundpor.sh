#! /bin/bash


clang++ dpor.cpp -o dpor
./dpor
#llvm-dis test1new.bc
#lli test1new.bc