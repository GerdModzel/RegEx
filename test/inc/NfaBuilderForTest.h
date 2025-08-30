#pragma once

#include "nfa/MainBuilder.h"

#include <array>

using namespace regex::nfa;

// A subclass of MainBuilder to expose internal states for testing
class NfaBuilderForTest : public MainBuilder {
public:
  NfaBuilderForTest() = default;

  size_t getStateManagerSize() const {
    return stateManager.size();
  }
  size_t getFragmentStackSize() const {
    return fragmentStack.size();
  }

  // Returns the most recent `size` states from the state manager;
  // std::array return type so structed bindings can be used in tests
  template<size_t size>
  std::array<State*, size> getTheMostRecentStates() {
    if (stateManager.size() < size) {
      throw std::out_of_range("Not enough states in state manager");
    }
    std::array<State*, size> states{};
    for (int i = 0; i < size; ++i) {
      states[size - i - 1] = stateManager[i].get();
    }
    return states;
  }

  // Expose protected method from MainBuilder
  Fragment popOneFragmentFromStack() {
    return MainBuilder::popOneFragmentFromStack();
  }
  // Expose protected method from MainBuilder
  std::pair<Fragment, Fragment> popTwoFragmentsFromStack() {
    return MainBuilder::popTwoFragmentsFromStack();
  }
};

// Helper function to set up an NfaBuilderForTest with a sequence of operators
// The parameter pack allows passing any number of parameters that are derived from regex::op::Operator
template<typename... Types>
  requires (std::derived_from<Types, regex::op::Operator> && ...)
NfaBuilderForTest setUpNfaBuilder(Types&& ...types) {
  NfaBuilderForTest builder;
  (builder.visit(std::make_unique<Types>(std::move(types)).get()), ...);
  return builder;
}

