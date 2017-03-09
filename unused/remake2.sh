cd /LLVM_ROOT/build/lib/Transforms/CSE231_Project/DFA/
make
cd /LLVM_ROOT/llvm/lib/Transforms/CSE231_Project/unused/
clang -O0 -S -emit-llvm phi.cpp
clang++ -c -O0 -emit-llvm phi.cpp -o phi.bc
opt -load CSE231-reaching.so -cse231-reaching < phi.bc -o phi-ins.bc
clang++ phi-ins.bc -o phi
