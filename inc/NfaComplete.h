#pragma once

#include "NfaFragment.h"

namespace regex {

  struct NfaState;

  class NfaComplete {
  public:
    NfaComplete(NfaFragment fragment, std::vector<std::unique_ptr<NfaState>> stateManager);
    ~NfaComplete() = default;

    NfaComplete(const NfaComplete& rhs) = delete;
    NfaComplete& operator=(const NfaComplete& rhs) = delete;
    NfaComplete(NfaComplete&& rhs) noexcept = default;
    NfaComplete& operator=(NfaComplete&& rhs) noexcept = default;

    NfaState* getStartState() const {
      return fragment.startState;
    }
    std::vector<NfaState**> getNextStates() const {
      return fragment.nextStates; 
    }
  private:
    NfaFragment fragment;
    std::vector<std::unique_ptr<NfaState>> stateManager;
  };

}

