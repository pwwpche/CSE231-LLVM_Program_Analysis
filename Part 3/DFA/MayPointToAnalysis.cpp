#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "231DFA.h"
#include <set>
#include <algorithm>
#include <string>

#define M(n) (n | (1 << 16))
#define R(n) (n)
#define Str(n) ( (n & (1 << 16)) ? "M" + to_string(n - (1 << 16)) : "R" + to_string(n) )

using namespace llvm;
using namespace std;

namespace llvm{

class MayInfo : public Info{
public:

  ~MayInfo() {}

  /*
   * Print out the information
   *
   * Direction:
   *   In your subclass you should implement this function according to the project specifications.
   */
  void print(){

    for(auto p = pointsTo.begin(), pe = pointsTo.end() ; p != pe ; p++){
      if(p->second.size() == 0){
        continue;
      }
      errs() << Str(p->first) << "->(";
      for(auto dfa : p->second){
        errs() << Str(dfa) << "/";
      }
      errs() << ")|";
    }
    errs() << "\n";
  }

  /*
   * Compare two pieces of information
   *
   * Direction:
   *   In your subclass you need to implement this function.
   */
  static bool equals(MayInfo * info1, MayInfo * info2){
    return info1->pointsTo.size() == info2->pointsTo.size()
      && std::equal(info1->pointsTo.begin(), info1->pointsTo.end(), info2->pointsTo.begin());
  }
  /*
   * Join two pieces of information.
   * The third parameter points to the result.
   *
   * Direction:
   *   In your subclass you need to implement this function.
   */
  static MayInfo* join(MayInfo * info1, MayInfo * info2, MayInfo * result){
    return result;
  }

  static void join(MayInfo * info1, MayInfo * result){
    for(auto it = info1->pointsTo.begin(), iend = info1->pointsTo.end() ; it != iend ; it++){
      unsigned instr = it->first;
      if(result->pointsTo.find(instr) == result->pointsTo.end()){
        result->pointsTo[instr] = info1->pointsTo[instr];
      }else{
        result->pointsTo[instr].insert(info1->pointsTo[instr].begin(), info1->pointsTo[instr].end());
      }
    }
  }

  map<unsigned, set<unsigned>> pointsTo;
};


class MayPointToAnalysis : public DataFlowAnalysis<MayInfo, true>{

public:
  MayPointToAnalysis(MayInfo info):
    DataFlowAnalysis<MayInfo, true>(info, info){}

    void flowfunction(Instruction * I,
    													std::vector<unsigned> & IncomingEdges,
															std::vector<unsigned> & OutgoingEdges,
															std::vector<MayInfo *> & Infos) {

    unsigned instrIdx = InstrToIndex[I];
    MayInfo* tempInfo = new MayInfo();
    Infos.resize(OutgoingEdges.size());

    for (auto incoming : IncomingEdges){
      Edge edge = Edge(incoming, instrIdx);
      MayInfo* incomingInfo = EdgeToInfo[edge];
      MayInfo::join(incomingInfo, tempInfo);
    }

    unsigned opcode = I->getOpcode();

    if(opcode == 29){   //alloca
      set<unsigned> newDFA;
      newDFA.insert(M(instrIdx));
      tempInfo->pointsTo[R(instrIdx)] = newDFA;
    }

    if(opcode == 32 ){ //getelementptr
      Instruction* oprInstr = (Instruction*)(((GetElementPtrInst*)I)->getPointerOperand());
      unsigned oprIdx = InstrToIndex[oprInstr];
      set<unsigned> newDFA = tempInfo->pointsTo[oprIdx];
      tempInfo->pointsTo[instrIdx] = newDFA;
    }

    if(opcode == 47 ){ //bitcast
      Instruction* oprInstr = (Instruction*)(((CastInst*)I)->getOperand(0));
      unsigned oprIdx = InstrToIndex[oprInstr];
      set<unsigned> newDFA = tempInfo->pointsTo[oprIdx];
      tempInfo->pointsTo[instrIdx] = newDFA;
    }

    if(opcode == 30){ //Load
      Instruction* pointerInstr = (Instruction*)(((LoadInst*)I)->getPointerOperand());
      unsigned oprIdx = InstrToIndex[pointerInstr];
      set<unsigned> points = tempInfo->pointsTo[oprIdx];

      set<unsigned> maypoints;
      for(auto ending : points){
        maypoints.insert(tempInfo->pointsTo[ending].begin(), tempInfo->pointsTo[ending].end());
      }
      if(isa<PointerType>(I->getType())){
          tempInfo->pointsTo[instrIdx] = maypoints;
      }
    }

    if(opcode == 31 ){ //Store
      Instruction* pointerInstr = (Instruction*)(((StoreInst*)I)->getPointerOperand());
      Instruction* valueInstr = (Instruction*)(((StoreInst*)I)->getValueOperand());
      if( InstrToIndex.find(pointerInstr) != InstrToIndex.end() &&
         InstrToIndex.find(valueInstr) != InstrToIndex.end() &&
         !isa<Constant>(valueInstr)){
        set<unsigned> pointerSet = tempInfo->pointsTo[InstrToIndex[pointerInstr]];
        set<unsigned> valueSet = tempInfo->pointsTo[InstrToIndex[valueInstr]];

        for(auto pointer : pointerSet){
          tempInfo->pointsTo[pointer].insert(valueSet.begin(), valueSet.end());
        }
      }
    }

    if(opcode == 55){     //Select
      Instruction* trueInstr = (Instruction*)(((SelectInst*)I)->getTrueValue());
      Instruction* falseInstr = (Instruction*)(((SelectInst*)I)->getFalseValue());

      set<unsigned> trueSet, falseSet;

      trueSet = tempInfo->pointsTo[InstrToIndex[trueInstr]];
      falseSet = tempInfo->pointsTo[InstrToIndex[falseInstr]];
      trueSet.insert(falseSet.begin(), falseSet.end());

      tempInfo->pointsTo[instrIdx] = trueSet;
    }

    if(opcode == 53){ //PHINode
      BasicBlock* block = I->getParent();


      for (auto ii = block->begin(), ie = block->end(); ii != ie; ++ii) {
        Instruction * instr = &*ii;
        set<unsigned> resultSet;
        if (isa<PHINode>(instr)){
          PHINode* phiInstr = (PHINode*) instr;
          for(unsigned i = 0, ie = phiInstr->getNumIncomingValues() ;
              i < ie ; i++){
            Instruction* phiValue = (Instruction*)(phiInstr->getIncomingValue(i));
            set<unsigned> prevSet = tempInfo->pointsTo[InstrToIndex[phiValue]];
            resultSet.insert(prevSet.begin(), prevSet.end());

          }
          tempInfo->pointsTo[InstrToIndex[instr]] = resultSet;
        }
      }

    }


    for(int i = 0 , size = OutgoingEdges.size() ; i < size ; i++){
      MayInfo* newInfo = new MayInfo();
      newInfo->pointsTo = tempInfo->pointsTo;
      Infos[i] = newInfo;
    }

  }
};




struct MayPointToAnalysisPass : public FunctionPass {

  static char ID;

  MayPointToAnalysisPass() : FunctionPass(ID) {
  }

  bool runOnFunction(Function &F) override {
    MayInfo baseInfo;
    MayPointToAnalysis analysis(baseInfo);
    analysis.runWorklistAlgorithm(&F);
    analysis.print();
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

}; // end of struct Hello

}  // end of anonymous namespace

char MayPointToAnalysisPass::ID = 0;
static RegisterPass<MayPointToAnalysisPass> X("cse231-maypointto", "MayPointToAnalysis",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
