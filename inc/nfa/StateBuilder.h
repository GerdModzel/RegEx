#pragma once

#include "NfaState.h"

#include <optional>
#include <vector>
#include <memory>

namespace regex {

  struct NfaFragment;

  namespace nfa {

class StateBuilder {
public:
  void setType(NfaState::Type type);
  void setType(std::optional<char> value);
  void connectToFragment(NfaFragment& frag);
  void createDanglingConnection();
  void cutOffConnections();
  std::unique_ptr<NfaState> build();
private:
  NfaState::Type stateType;
  std::optional<char> stateValue = std::nullopt;
  std::vector<NfaState*> nextStates;
};

}
}

