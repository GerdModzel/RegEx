#pragma once

#include "nfa/State.h"

#include <optional>
#include <vector>
#include <memory>

namespace regex {


  namespace nfa {

  struct Fragment;

class StateBuilder {
public:
  StateBuilder& setType(State::Type type);
  StateBuilder& setType(std::optional<char> value);
  StateBuilder& connectToState(State* state);
  StateBuilder& connectToFragment(Fragment& frag);
  StateBuilder& createDanglingConnection();
  StateBuilder& cutOffConnections();
  std::unique_ptr<State> build();
private:
  State::Type stateType;
  std::optional<char> stateValue = std::nullopt;
  std::vector<State*> nextStates;
};

}
}

