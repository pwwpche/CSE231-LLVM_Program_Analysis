#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "/lib231/lib231.cpp"

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/GlobalVariable.h"
#include <cstdint>
using namespace llvm;
using namespace std;
namespace {

    struct BranchBiasPass : public ModulePass {

        static char ID;

        BranchBiasPass() : ModulePass(ID) {
        }


        Function* printBranch, *updateBranch;

        virtual bool runOnModule(Module &M)
        {

            Constant* printBranchFunc = M.getOrInsertFunction("printOutBranchInfo",
            /* Return type */                     Type::getVoidTy(M.getContext()),
            /*varargs terminated with null*/      NULL);
            Constant* updateBranchFunc = M.getOrInsertFunction("updateBranchInfo",
                                                  Type::getVoidTy(M.getContext()),
                                                  IntegerType::get(M.getContext(),1),
                                                  NULL);

            printBranch = cast<Function>(printBranchFunc);
            updateBranch = cast<Function>(updateBranchFunc);

            for(Module::iterator F = M.begin(), E = M.end(); F!= E; ++F)
            {

                for(Function::iterator BB = F->begin(), BE = F->end(); BB != BE; ++BB)
                {
                    bool hasRet = runOnBasicBlock(BB);
                    if(hasRet){
                      BasicBlock::iterator retIt = BB->end();
                      retIt--;
                      IRBuilder<> builder(&*retIt);
                      builder.CreateCall(printBranch);
                    }
                }

            }

            return true;

        }

        virtual bool runOnBasicBlock(Function::iterator &BB)
        {

            bool hasRet = false;
            for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
            {
                if((*BI).getOpcode() == 2 && (*BI).getNumOperands() > 1){     // Op(BR) = 2
                    Value* taken = (*BI).getOperand(0);
                    BasicBlock::iterator lastInstr = BI;
                    callUpdateBranch(lastInstr, taken);
                }

                if((*BI).getOpcode() == 1){
                  hasRet = true;
                }
            }
            return hasRet;
        }

        void callUpdateBranch(BasicBlock::iterator &BB, Value* taken){
          IRBuilder<> builder(&*BB);
          vector<Value*> args;
          args.push_back(taken);
          builder.CreateCall(updateBranch, args);
        }




        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
        }




    }; // end of struct Hello

}  // end of anonymous namespace

char BranchBiasPass::ID = 0;
static RegisterPass<BranchBiasPass> X("cse231-bb", "BranchBiasPass",
                                             false /* Only looks at CFG */,
                                             false /* Analysis Pass */);
