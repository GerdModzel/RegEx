#include "NfaComplete.h"

#include "nfa/State.h"

namespace regex {

  NfaComplete::NfaComplete(NfaFragment fragment, std::vector<std::unique_ptr<nfa::State>> stateManager)
    : fragment(std::move(fragment))
    , stateManager(std::move(stateManager)) {
  }

}
