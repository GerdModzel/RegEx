#include "nfa/Complete.h"

#include "nfa/State.h"

namespace regex::nfa {

  Complete::Complete(NfaFragment fragment, std::vector<std::unique_ptr<nfa::State>> stateManager)
    : fragment(std::move(fragment))
    , stateManager(std::move(stateManager)) {
  }

}
