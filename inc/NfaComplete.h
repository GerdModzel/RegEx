#pragma once

#include "NfaFragment.h"

namespace regex {

  namespace nfa {
    struct State;
  }

  class NfaComplete {
  public:
    NfaComplete(NfaFragment fragment, std::vector<std::unique_ptr<nfa::State>> stateManager);
    ~NfaComplete() = default;

    NfaComplete(const NfaComplete& rhs) = delete;
    NfaComplete& operator=(const NfaComplete& rhs) = delete;
    NfaComplete(NfaComplete&& rhs) noexcept = default;
    NfaComplete& operator=(NfaComplete&& rhs) noexcept = default;

    nfa::State* getStartState() const {
      return fragment.startState;
    }
    std::vector<nfa::State**> getNextStates() const {
      return fragment.nextStates; 
    }
  private:
    NfaFragment fragment;
    std::vector<std::unique_ptr<nfa::State>> stateManager;
  };

}

