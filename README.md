# CSE231 LLVM Program Analysis
Project of UCSD CSE231 Advanced Compilers


## Description:

This is the course project of UCSD CSE. This project mainly takes LLVM to finish three parts of works:

### Part 1 : [Link](https://ucsd-pl.github.io/cse231/wi17/part1.html)

1. **Collecting Static Instruction Counts:**

   Your task is to write a function pass that counts the number of each unique instruction in a function statically. 

2. **Collecting Dynamic Instruction Counts**

   The analysis you wrote in Section 1 was a static analysis because it did not actually execute the program to analyze it. In this section, we are going to write a dynamic analysis that executes the program and collects runtime information. In particular, you will write a pass that instruments the original program by inserting code to count the number times each instruction executes. Each instrumented function should output the analysis results before function termination.
   
3. **Profiling Branch Bias**

   Now, write a dynamic analysis that computes the branch bias on a per-function basis: count the number of times conditional branch instructions are executed and the number of times conditional branch instructions are taken. Note that we only consider conditional branches. A conditional branch is considered taken if its condition evaluates to true. Each instrumented function should output these two counts before function termination. 
   
### Part 2 : [Link](https://ucsd-pl.github.io/cse231/wi17/part2.html)

1. **Dataflow Analysis Framework**

   You need to implement a generic intra-procedural dataflow analysis framework. 
   
2. **Reaching Definition Analysis**

   You will also need to implement a reaching definition analysis based on the framework you implemented.
   
### Part 3 : [Link](https://ucsd-pl.github.io/cse231/wi17/part3.html)

1. **Liveness Analysis**

   Your first task for this part is to implement a liveness analysis based on the framework implemented in the previous assignment. Recall that a variable is live at a particular point in the program if its value at that point will be used in the future. It is dead otherwise.
   
2. **May-point-to Analysis**

   In part 3, you will also need to implement a may-point-to analysis based on the framework you implemented


## Configuration
Install Docker, and follow [this](https://ucsd-pl.github.io/cse231/wi17/part0.html) tutorial

## Useful Tips
There are several commands that might be useful:

* Generate .ll file for a C++ program
```
clang -O0 -S -emit-llvm input.cpp
```

* Run passes 
```
opt -load [.so file] -[pass name] < [.ll file] > /dev/null
opt -load LLVMPart1.so -cse231-csi < input.ll > /dev/null
```

* Generate .bc file
```
clang++ -c -O0 -emit-llvm phi.cpp -o phi.bc
opt -load LLVMPart2.so -cse231-cdi < input.bc -o input-instrumented.bc
```

* Generate executives
```
clang++ /lib231/lib231.cpp input-instrumented.bc -o instrumented
```

There are also automaking scripts in `unused` folder named `remake*.sh`
