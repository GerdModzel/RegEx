#include "nfa/FragmentBuilder.h"

#include "nfa/Fragment.h"

#include <cassert>

namespace regex::nfa {

  FragmentBuilder& FragmentBuilder::setStartState(State* state) {
    startState = state;
    return *this;
  }

  FragmentBuilder& FragmentBuilder::attachToConnections(const Fragment& frag) {
    for (auto& nextState : frag.nextStates)
      attachToConnection(nextState);
    return *this;
  }

  FragmentBuilder& FragmentBuilder::attachToConnection(State** nextState) {
    assert(*nextState == nullptr);
    nextStates.push_back(nextState);
    return *this;
  }

  FragmentBuilder& FragmentBuilder::setEndStates(std::vector<State*>& ptrList) {
    for (auto& ptr : ptrList)
      nextStates.push_back(&ptr);
    return *this;
  }

  Fragment FragmentBuilder::build() {
    return Fragment{ startState, nextStates };
  }


}
