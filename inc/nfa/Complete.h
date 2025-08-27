#pragma once

#include "nfa/Fragment.h"

namespace regex::nfa {

  struct State;

  class Complete {
  public:
    Complete(Fragment fragment, std::vector<std::unique_ptr<State>> stateManager);
    ~Complete() = default;

    Complete(const Complete& rhs) = delete;
    Complete& operator=(const Complete& rhs) = delete;
    Complete(Complete&& rhs) noexcept = default;
    Complete& operator=(Complete&& rhs) noexcept = default;

    State* getStartState() const {
      return fragment.startState;
    }
    std::vector<State**> getNextStates() const {
      return fragment.nextStates; 
    }
  private:
    Fragment fragment;
    std::vector<std::unique_ptr<State>> stateManager;
  };

}

