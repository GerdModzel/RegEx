#include "nfa/FragmentBuilder.h"

#include "NfaFragment.h"

namespace regex::nfa {

  void FragmentBuilder::setStartState(NfaState* state) {
    startState = state;
  }

  void FragmentBuilder::takeOverConnectionsFrom(const NfaFragment& frag) {
    nextStates.insert(nextStates.end(), frag.nextStates.begin(), frag.nextStates.end());
  }

  void FragmentBuilder::takeOverConnection(NfaState** frag) {
    nextStates.push_back(frag);
  }

  void FragmentBuilder::setEndStates(std::vector<NfaState*>& ptrList) {
    for (auto& ptr : ptrList)
      nextStates.push_back(&ptr);
  }

  NfaFragment FragmentBuilder::build() {
    return NfaFragment{ startState, nextStates };
  }


}
