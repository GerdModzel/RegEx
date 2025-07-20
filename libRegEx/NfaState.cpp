#include "nfaState.h"

namespace regex {

    NfaState::NfaState(Type type, std::optional<char> ch, std::vector<NfaState*> nextStates)
      : type(type)
      , ch(ch)
      , nextStates(std::move(nextStates))
      , lastList(-1) {
    }

}
