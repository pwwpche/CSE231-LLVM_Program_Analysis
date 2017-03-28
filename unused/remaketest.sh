cd /LLVM_ROOT/build/lib/Transforms/CSE231_Project/DFA/
make
cd /LLVM_ROOT/llvm/lib/Transforms/CSE231_Project/unused/
clang -O1 -S -emit-llvm test.c
clang -c -O1 -emit-llvm test.c -o test.bc
opt -load CSE231-liveness.so -cse231-maypointto < test.bc -o test-ins.bc
clang++ test-ins.bc -o test
