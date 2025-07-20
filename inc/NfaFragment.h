#pragma once

#include <vector>
#include <memory>

namespace regex {

  struct NfaState;
  class Expression;

  using NfaStateOwner  = std::vector<std::unique_ptr<NfaState>>;

  struct NfaFragment {
    NfaFragment(NfaState* startState, std::vector<NfaState**> nextStates, NfaStateOwner stateOwner);
    ~NfaFragment();
    NfaFragment(const NfaFragment& rhs) = delete;
    NfaFragment& operator=(const NfaFragment& rhs) = delete;
    NfaFragment(NfaFragment&& rhs) noexcept;
    NfaFragment& operator=(NfaFragment&& rhs) noexcept;

    NfaState* startState;
    std::vector<NfaState**> nextStates;
  private:
		NfaStateOwner stateManager;
    friend class NfaBuilder;
  };

}
