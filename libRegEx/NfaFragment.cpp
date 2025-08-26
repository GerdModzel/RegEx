#include "NfaFragment.h"

#include "NfaState.h"

namespace regex {

  NfaFragment::NfaFragment(NfaState* startState, std::vector<NfaState**> nextStates)
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
