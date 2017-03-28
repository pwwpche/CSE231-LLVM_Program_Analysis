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

    struct DynamicInstrCountPass : public ModulePass {

        static char ID;

        DynamicInstrCountPass() : ModulePass(ID) {
        }


        Function* printInstr, *updateInstr;

        virtual bool runOnModule(Module &M)
        {

            Constant* printInstrFunc = M.getOrInsertFunction("printOutInstrInfo",
            /* Return type */                     Type::getVoidTy(M.getContext()),
            /*varargs terminated with null*/      NULL);
            Constant* updateInstrFunc = M.getOrInsertFunction("updateInstrInfo1",
                                                  Type::getVoidTy(M.getContext()),
                                                  IntegerType::get(M.getContext(),32),
                                                  IntegerType::get(M.getContext(),32),
                                                  //PointerType::get(IntegerType::get(M.getContext(), 32), 0),
                                                  //PointerType::get(IntegerType::get(M.getContext(), 32), 0),
                                                  NULL);

            printInstr = cast<Function>(printInstrFunc);
            updateInstr = cast<Function>(updateInstrFunc);

            for(Module::iterator F = M.begin(), E = M.end(); F!= E; ++F)
            {

                for(Function::iterator BB = F->begin(), BE = F->end(); BB != BE; ++BB)
                {
                    bool hasRet = runOnBasicBlock(BB);
                    if(hasRet){
                      BasicBlock::iterator retIt = BB->end();
                      retIt--;
                      IRBuilder<> builder(&*retIt);
                      builder.CreateCall(printInstr);
                    }
                }

            }

            return true;

        }

        virtual bool runOnBasicBlock(Function::iterator &BB)
        {
            map<int, int> instrCnt;
            bool hasRet = false;
            for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
            {
                instrCnt[(*BI).getOpcode()]++;
                if((*BI).getOpcode() == 1){     // Op(RET) = 1
                    hasRet = true;
                }
            }
            //Update before each basic block ends.

            BasicBlock::iterator lastInstr = BB->end();
            --lastInstr;
            callUpdateInstr(lastInstr, instrCnt);
            return hasRet;
        }


        void callUpdateInstr(BasicBlock::iterator &BB, const map<int, int>& instrCnt){

          //Assert there are less than 80 different instructions.
          assert(instrCnt.size() < 128);
          IRBuilder<> builder(&*BB);

          for(auto it = instrCnt.begin(), endIt = instrCnt.end(); it != endIt ; it++){
              vector<Value*> args;
              args.push_back(builder.getInt32(it->first));
              args.push_back(builder.getInt32(it->second));
              builder.CreateCall(updateInstr, args);
          }

        }

        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
        }




    }; // end of struct Hello

}  // end of anonymous namespace

char DynamicInstrCountPass::ID = 0;
static RegisterPass<DynamicInstrCountPass> X("cse231-cdi", "DynamicInstrCountPass",
                                             false /* Only looks at CFG */,
                                             false /* Analysis Pass */);
