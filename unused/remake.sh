cd /LLVM_ROOT/build/lib/Transforms/CSE231_Project/part3/
make
cd /LLVM_ROOT/llvm/lib/Transforms/CSE231_Project/unused/
opt -load CSE231-3.so -cse231-bb < input.bc -o input-instrumented.bc
clang++ /lib231/lib231.cpp input-instrumented.bc -o instrumented
