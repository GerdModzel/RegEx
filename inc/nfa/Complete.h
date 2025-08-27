#pragma once

#include "NfaFragment.h"

namespace regex::nfa {

  struct State;

  class Complete {
  public:
    Complete(NfaFragment fragment, std::vector<std::unique_ptr<nfa::State>> stateManager);
    ~Complete() = default;

    Complete(const Complete& rhs) = delete;
    Complete& operator=(const Complete& rhs) = delete;
    Complete(Complete&& rhs) noexcept = default;
    Complete& operator=(Complete&& rhs) noexcept = default;

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

