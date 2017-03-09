#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "/lib231/lib231.cpp"
using namespace llvm;
using namespace std;

namespace {

struct StaticInstrCountPass : public FunctionPass {

  static char ID;

  StaticInstrCountPass() : FunctionPass(ID) {
  }

  bool runOnFunction(Function &F) override {
    map<unsigned, int> useCount;

    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I){
      useCount[(*I).getOpcode()]++;
    }

    for(auto it = useCount.begin() ; it != useCount.end() ; it++){
      errs() << mapCodeToName(it->first) << "\t" << it->second << "\n";
    }


    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

}; // end of struct Hello

}  // end of anonymous namespace

char StaticInstrCountPass::ID = 0;
static RegisterPass<StaticInstrCountPass> X("cse231-csi", "StaticInstrCountPass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
