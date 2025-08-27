#include "nfa/StateBuilder.h"

#include "NfaFragment.h"

namespace regex::nfa {

  void StateBuilder::setType(State::Type type) {
    stateType = type;
    stateValue = std::nullopt;
  }

  void StateBuilder::setType(std::optional<char> value) {
    stateType = State::Type::ch;
    stateValue = value; 
  }

  void StateBuilder::connectToFragment(NfaFragment& frag) {
    nextStates.push_back(frag.startState); 
  }

  void StateBuilder::createDanglingConnection() {
    nextStates.push_back(nullptr); 
  }

  void StateBuilder::cutOffConnections() {
    nextStates = {}; 
  }

  std::unique_ptr<State> StateBuilder::build() {
    return std::make_unique<State>(stateType, stateValue, nextStates); 
  }

}
