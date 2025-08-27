#pragma once

#include <vector>

namespace regex {

  struct NfaState;
  struct NfaFragment;

  namespace nfa {

    class FragmentBuilder {
    public:
      void setStartState(NfaState* state);
      void takeOverConnectionsFrom(const NfaFragment& frag);
      void takeOverConnection(NfaState** frag);
      void setEndStates(std::vector<NfaState*>& ptrList);
      NfaFragment build();
    private:
      NfaState* startState;
      std::vector<NfaState**> nextStates;
    };

  }

}
