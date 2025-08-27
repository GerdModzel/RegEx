#pragma once

#include "nfa/State.h"

#include <optional>
#include <vector>
#include <memory>

namespace regex {

  struct NfaFragment;

  namespace nfa {

class StateBuilder {
public:
  void setType(State::Type type);
  void setType(std::optional<char> value);
  void connectToFragment(NfaFragment& frag);
  void createDanglingConnection();
  void cutOffConnections();
  std::unique_ptr<State> build();
private:
  State::Type stateType;
  std::optional<char> stateValue = std::nullopt;
  std::vector<State*> nextStates;
};

}
}

