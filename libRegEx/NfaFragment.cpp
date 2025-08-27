#include "NfaFragment.h"

#include "nfa/State.h"

namespace regex {

  NfaFragment::NfaFragment(nfa::State* startState, std::vector<nfa::State**> nextStates)
    : startState(startState)
    , nextStates(std::move(nextStates)) {
  }

  NfaFragment::~NfaFragment() = default;

  NfaFragment::NfaFragment(NfaFragment&& rhs) noexcept
    : startState(rhs.startState)
    , nextStates(std::move(rhs.nextStates)) {
  }

  NfaFragment& NfaFragment::operator=(NfaFragment&& rhs) noexcept {
    if (this != &rhs) {
      startState = rhs.startState;
      nextStates = std::move(rhs.nextStates);
    }
    return *this;
  }

}
