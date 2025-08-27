#include "nfa/StateBuilder.h"

#include "NfaFragment.h"

namespace regex::nfa {

  void StateBuilder::setType(NfaState::Type type) {
    stateType = type;
    stateValue = std::nullopt;
  }

  void StateBuilder::setType(std::optional<char> value) {
    stateType = NfaState::Type::ch;
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

  std::unique_ptr<NfaState> StateBuilder::build() {
    return std::make_unique<NfaState>(stateType, stateValue, nextStates); 
  }

}
