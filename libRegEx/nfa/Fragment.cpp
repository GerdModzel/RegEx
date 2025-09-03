#include "nfa/Fragment.h"

#include "nfa/State.h"

namespace regex::nfa {

  Fragment::Fragment(State* startState, std::vector<State**> nextStates)
    : startState(startState)
    , nextStates(std::move(nextStates)) {
  }

  Fragment::~Fragment() = default;

}
