#pragma once

#include <vector>

namespace regex {

  struct NfaFragment;

  namespace nfa {

    struct State;

    class FragmentBuilder {
    public:
      void setStartState(State* state);
      void takeOverConnectionsFrom(const NfaFragment& frag);
      void takeOverConnection(State** frag);
      void setEndStates(std::vector<State*>& ptrList);
      NfaFragment build();
    private:
      State* startState;
      std::vector<State**> nextStates;
    };

  }

}
