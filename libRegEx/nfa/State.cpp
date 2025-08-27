#include "nfa/State.h"

namespace regex::nfa {

    State::State(State::Type type, std::optional<char> ch, std::vector<State*> nextStates)
      : type(type)
      , ch(ch)
      , nextStates(std::move(nextStates))
      , lastList(-1) {
    }

}
