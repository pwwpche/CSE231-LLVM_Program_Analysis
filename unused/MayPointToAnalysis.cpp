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
  static bool equals(MayInfo * info1, MayInfo * info2){
    return info1->definedInstructions == info2->definedInstructions;
  }
  /*
   * Join two pieces of information.
   * The third parameter points to the result.
   *
   * Direction:
   *   In your subclass you need to implement this function.
   */
  static MayInfo* join(MayInfo * info1, MayInfo * info2, MayInfo * result){
    result->definedInstructions.insert(info1->definedInstructions.begin(), info1->definedInstructions.end());
    result->definedInstructions.insert(info2->definedInstructions.begin(), info2->definedInstructions.end());
    return result;
  }

  static void join(MayInfo * info1, MayInfo * result){
    result->definedInstructions.insert(info1->definedInstructions.begin(), info1->definedInstructions.end());
  }

  set<unsigned> definedInstructions;
};


class MayPointToAnalysis : public DataFlowAnalysis<MayInfo, false>{

public:
  MayPointToAnalysis(MayInfo info):
    DataFlowAnalysis<MayInfo, false>(info, info){}

    void flowfunction(Instruction * I,
    													std::vector<unsigned> & IncomingEdges,
															std::vector<unsigned> & OutgoingEdges,
															std::vector<MayInfo *> & Infos) {


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
