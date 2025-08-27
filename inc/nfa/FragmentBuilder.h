#pragma once

#include <vector>

namespace regex {


  namespace nfa {

    struct State;
    struct Fragment;

    class FragmentBuilder {
    public:
      void setStartState(State* state);
      void takeOverConnectionsFrom(const Fragment& frag);
      void takeOverConnection(State** frag);
      void setEndStates(std::vector<State*>& ptrList);
      Fragment build();
    private:
      State* startState;
      std::vector<State**> nextStates;
    };

  }

}
