#include "nfa/Fragment.h"

#include "nfa/State.h"

namespace regex::nfa {

  Fragment::Fragment(State* startState, std::vector<State**> nextStates)
    : startState(startState)
    , nextStates(std::move(nextStates)) {
  }

  Fragment::~Fragment() = default;

  Fragment::Fragment(Fragment&& rhs) noexcept
    : startState(rhs.startState)
    , nextStates(std::move(rhs.nextStates)) {
  }

  Fragment& Fragment::operator=(Fragment&& rhs) noexcept {
    if (this != &rhs) {
      startState = rhs.startState;
      nextStates = std::move(rhs.nextStates);
    }
    return *this;
  }

}
