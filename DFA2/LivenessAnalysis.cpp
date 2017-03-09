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

class LiveInfo : public Info{
public:

  ~LiveInfo() {}

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
  static bool equals(LiveInfo * info1, LiveInfo * info2){
    return info1->definedInstructions == info2->definedInstructions;
  }
  /*
   * Join two pieces of information.
   * The third parameter points to the result.
   *
   * Direction:
   *   In your subclass you need to implement this function.
   */
  static LiveInfo* join(LiveInfo * info1, LiveInfo * info2, LiveInfo * result){
    result->definedInstructions.insert(info1->definedInstructions.begin(), info1->definedInstructions.end());
    result->definedInstructions.insert(info2->definedInstructions.begin(), info2->definedInstructions.end());
    return result;
  }

  static void join(LiveInfo * info1, LiveInfo * result){
    result->definedInstructions.insert(info1->definedInstructions.begin(), info1->definedInstructions.end());
  }

  set<unsigned> definedInstructions;
};


class LivenessAnalysis : public DataFlowAnalysis<LiveInfo, false>{

public:
  LivenessAnalysis(LiveInfo info):
    DataFlowAnalysis<LiveInfo, false>(info, info){}

    void flowfunction(Instruction * I,
    													std::vector<unsigned> & IncomingEdges,
															std::vector<unsigned> & OutgoingEdges,
															std::vector<LiveInfo *> & Infos) {

    unsigned instrIdx = InstrToIndex[I];
    LiveInfo* tempInfo = new LiveInfo();
    Infos.resize(OutgoingEdges.size());

    for (auto incoming : IncomingEdges){
      Edge edge = Edge(incoming, instrIdx);
      LiveInfo* incomingInfo = EdgeToInfo[edge];
      LiveInfo::join(incomingInfo, tempInfo);
    }

    unsigned opcode = I->getOpcode();

    // alloca, load, getelementptr
    // icmp, fcmp, select
    if(opcode == 29 || opcode == 30 || opcode == 32 ||
      opcode == 51 || opcode == 52 || opcode == 55 ||
      I->isBinaryOp()  ){
      tempInfo->definedInstructions.erase(instrIdx);
      for (Use &U : I->operands()) {
        Value *v = U.get();
        tempInfo->definedInstructions.insert(InstrToIndex[llvm::cast<llvm::Instruction>(v)]);
      }

      for(int i = 0 , size = OutgoingEdges.size() ; i < size ; i++){
        LiveInfo* newInfo = new LiveInfo();
        newInfo->definedInstructions = tempInfo->definedInstructions;
        Infos.push_back(newInfo);
      }

      delete tempInfo;
    }


    // br, switch, store
    if(opcode == 2 || opcode == 3 || opcode == 31){
      for (Use &U : I->operands()) {
        Value *v = U.get();
        tempInfo->definedInstructions.insert(InstrToIndex[llvm::cast<llvm::Instruction>(v)]);
      }

      for(int i = 0 , size = OutgoingEdges.size() ; i < size ; i++){
        LiveInfo* newInfo = new LiveInfo();
        newInfo->definedInstructions = tempInfo->definedInstructions;
        Infos.push_back(newInfo);
      }

      delete tempInfo;

    }

    if(opcode == 53){     //Phi
        // tempInfo->definedInstructions.erase(InstrToIndex[llvm::cast<llvm::Instruction>(I->getOperand(0))]);
        // tempInfo->definedInstructions.erase(InstrToIndex[llvm::cast<llvm::Instruction>(I->getOperand(1))]);
        // tempInfo->definedInstructions.insert(instrIdx);
      BasicBlock* block = I->getParent();
      for (auto ii = block->begin(), ie = block->end(); ii != ie; ++ii) {
        Instruction * instr = &*ii;
        if (isa<PHINode>(instr)){
          tempInfo->definedInstructions.erase(InstrToIndex[instr]);
        }
      }
      for(int j = 0, je = OutgoingEdges.size() ; j < je ; j++){
        Infos[j] = new LiveInfo();
        Infos[j]->definedInstructions = tempInfo->definedInstructions;
      }

      for (auto ii = block->begin(), ie = block->end(); ii != ie; ++ii) {
        Instruction * instr = &*ii;
        if (isa<PHINode>(instr)){
          PHINode* phiInstr = lvm::cast<llvm::PHINode> instr;
          for(unsigned i = 0, ie = phiInstr->getNumIncomingValues() ;
              i < ie ; i++){
            Instruction* phiValue = phiInstr->getIncomingValue(i);

            BasicBlock* phiLabel = phiInstr->getIncomingBlock(i);
            Instruction* prevInstr = (Instruction *)phiLabel->getTerminator();
            unsigned prevInstrIndex = InstrToIndex[prevInstr];

            for(int j = 0, je = OutgoingEdges.size() ; j < je ; j++){
              if(OutgoingEdges[j] == prevInstrIndex){
                Infos[j]->definedInstructions.insert(InstrToIndex[phiValue]);
              }
            }
          }
        }
      }

      delete tempInfo;
    }



  }
};




struct LivenessAnalysisPass : public FunctionPass {

  static char ID;

  LivenessAnalysisPass() : FunctionPass(ID) {
  }

  bool runOnFunction(Function &F) override {
    LiveInfo baseInfo;
    LivenessAnalysis analysis(baseInfo);
    analysis.runWorklistAlgorithm(&F);
    analysis.print();
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

}; // end of struct Hello

}  // end of anonymous namespace

char LivenessAnalysisPass::ID = 0;
static RegisterPass<LivenessAnalysisPass> X("cse231-liveness", "LivenessAnalysis",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
