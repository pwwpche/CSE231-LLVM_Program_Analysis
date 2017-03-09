#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "231DFA.h"
#include <set>
#include <algorithm>
using namespace llvm;
using namespace std;

namespace llvm{

class ReachingInfo : public Info{
public:

  ~ReachingInfo() {}

  /*
   * Print out the information
   *
   * Direction:
   *   In your subclass you should implement this function according to the project specifications.
   */
  void print(){
    for(auto def : definedInstructions){
      errs() << def << "|";
    }
    errs() << "\n";
  }

  /*
   * Compare two pieces of information
   *
   * Direction:
   *   In your subclass you need to implement this function.
   */
  static bool equals(ReachingInfo * info1, ReachingInfo * info2){
    return info1->definedInstructions == info2->definedInstructions;
  }
  /*
   * Join two pieces of information.
   * The third parameter points to the result.
   *
   * Direction:
   *   In your subclass you need to implement this function.
   */
  static ReachingInfo* join(ReachingInfo * info1, ReachingInfo * info2, ReachingInfo * result){
    result->definedInstructions.insert(info1->definedInstructions.begin(), info1->definedInstructions.end());
    result->definedInstructions.insert(info2->definedInstructions.begin(), info2->definedInstructions.end());
    return result;
  }

  static void join(ReachingInfo * info1, ReachingInfo * result){
    result->definedInstructions.insert(info1->definedInstructions.begin(), info1->definedInstructions.end());
  }

  set<unsigned> definedInstructions;
};


class ReachingDefinitionAnalysis : public DataFlowAnalysis<ReachingInfo, true>{

public:
  ReachingDefinitionAnalysis(ReachingInfo info):
    DataFlowAnalysis<ReachingInfo, true>(info, info){}

    void flowfunction(Instruction * I,
    													std::vector<unsigned> & IncomingEdges,
															std::vector<unsigned> & OutgoingEdges,
															std::vector<ReachingInfo *> & Infos) {

    unsigned instrIdx = InstrToIndex[I];

      ReachingInfo* tempInfo = new ReachingInfo();
      for (auto incoming : IncomingEdges){
        Edge edge = Edge(incoming, instrIdx);
        ReachingInfo* incomingInfo = EdgeToInfo[edge];
        ReachingInfo::join(incomingInfo, tempInfo);
      }

      unsigned opcode = I->getOpcode();

      if(opcode == 29 || opcode == 30 || opcode == 32 ||
        opcode == 51 || opcode == 52 || opcode == 55 ||
        I->isBinaryOp()  ){
        tempInfo->definedInstructions.insert(instrIdx);
      }

      if(opcode == 53){     //Phi
          // tempInfo->definedInstructions.erase(InstrToIndex[llvm::cast<llvm::Instruction>(I->getOperand(0))]);
          // tempInfo->definedInstructions.erase(InstrToIndex[llvm::cast<llvm::Instruction>(I->getOperand(1))]);
          // tempInfo->definedInstructions.insert(instrIdx);
          BasicBlock* block = I->getParent();
          for (auto ii = block->begin(), ie = block->end(); ii != ie; ++ii) {
            Instruction * instr = &*ii;
            if (isa<PHINode>(instr)){
              tempInfo->definedInstructions.insert(InstrToIndex[instr]);
            }
          }
      }


      for(int i = 0 , size = OutgoingEdges.size() ; i < size ; i++){
        ReachingInfo* newInfo = new ReachingInfo();
        newInfo->definedInstructions = tempInfo->definedInstructions;
        Infos.push_back(newInfo);
      }

      delete tempInfo;


  }
};




struct ReachingDefinitionAnalysisPass : public FunctionPass {

  static char ID;

  ReachingDefinitionAnalysisPass() : FunctionPass(ID) {
  }

  bool runOnFunction(Function &F) override {
    ReachingInfo baseInfo;
    ReachingDefinitionAnalysis analysis(baseInfo);
    analysis.runWorklistAlgorithm(&F);
    analysis.print();
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

}; // end of struct Hello

}  // end of anonymous namespace

char ReachingDefinitionAnalysisPass::ID = 0;
static RegisterPass<ReachingDefinitionAnalysisPass> X("cse231-reaching", "StaticInstrCountPass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
