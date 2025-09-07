#include "nfa/StateBuilder.h"

#include "nfa/Fragment.h"

namespace regex::nfa {

  StateBuilder& StateBuilder::setType(State::Type type) {
    stateType = type;
    stateValue = std::nullopt;
    return *this;
  }

  StateBuilder& StateBuilder::setType(std::optional<char> value) {
    stateType = State::Type::ch;
    stateValue = value; 
    return *this;
  }

  StateBuilder& StateBuilder::connectToState(State* state) {
    nextStates.push_back(state); 
    return *this;
  }

  StateBuilder& StateBuilder::connectToFragment(Fragment& frag) {
    return connectToState(frag.startState);
  }

  StateBuilder& StateBuilder::createDanglingConnection() {
    nextStates.push_back(nullptr); 
    return *this;
  }

  StateBuilder& StateBuilder::cutOffConnections() {
    nextStates = {}; 
    return *this;
  }

  std::unique_ptr<State> StateBuilder::build() {
    return std::make_unique<State>(stateType, stateValue, nextStates); 
  }

}
