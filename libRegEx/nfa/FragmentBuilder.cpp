#include "nfa/FragmentBuilder.h"

#include "nfa/Fragment.h"

#include <cassert>

namespace regex::nfa {

  void FragmentBuilder::setStartState(State* state) {
    startState = state;
  }

  void FragmentBuilder::attachToConnections(const Fragment& frag) {
    for (auto& nextState : frag.nextStates)
      attachToConnection(nextState);
  }

  void FragmentBuilder::attachToConnection(State** nextState) {
    assert(*nextState == nullptr);
    nextStates.push_back(nextState);
  }

  void FragmentBuilder::setEndStates(std::vector<State*>& ptrList) {
    for (auto& ptr : ptrList)
      nextStates.push_back(&ptr);
  }

  Fragment FragmentBuilder::build() {
    return Fragment{ startState, nextStates };
  }


}
