#include "nfa/FragmentBuilder.h"

#include "nfa/Fragment.h"

namespace regex::nfa {

  void FragmentBuilder::setStartState(State* state) {
    startState = state;
  }

  void FragmentBuilder::takeOverConnectionsFrom(const Fragment& frag) {
    nextStates.insert(nextStates.end(), frag.nextStates.begin(), frag.nextStates.end());
  }

  void FragmentBuilder::takeOverConnection(State** frag) {
    nextStates.push_back(frag);
  }

  void FragmentBuilder::setEndStates(std::vector<State*>& ptrList) {
    for (auto& ptr : ptrList)
      nextStates.push_back(&ptr);
  }

  Fragment FragmentBuilder::build() {
    return Fragment{ startState, nextStates };
  }


}
