#include "NfaFragment.h"

#include "NfaState.h"

namespace regex {

  NfaFragment::NfaFragment(NfaState* startState, std::vector<NfaState**> nextStates, NfaStateOwner stateOwner)
    : startState(startState)
    , nextStates(std::move(nextStates))
    , stateManager(std::move(stateOwner)) {
  }

  NfaFragment::~NfaFragment() = default;

  NfaFragment::NfaFragment(NfaFragment&& rhs) noexcept
    : startState(rhs.startState)
    , nextStates(std::move(rhs.nextStates))
    , stateManager(std::move(rhs.stateManager)) {
  }

  NfaFragment& NfaFragment::operator=(NfaFragment&& rhs) noexcept {
    if (this != &rhs) {
      startState = rhs.startState;
      nextStates = std::move(rhs.nextStates);
      stateManager = std::move(rhs.stateManager);
    }
    return *this;
  }

}
