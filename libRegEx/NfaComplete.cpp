#include "NfaComplete.h"

#include "NfaState.h"

namespace regex {

  NfaComplete::NfaComplete(NfaFragment fragment, std::vector<std::unique_ptr<NfaState>> stateManager)
    : fragment(std::move(fragment))
    , stateManager(std::move(stateManager)) {
  }

}
