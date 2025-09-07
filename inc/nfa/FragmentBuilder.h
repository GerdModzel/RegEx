#pragma once

#include <vector>

namespace regex {


  namespace nfa {

    struct State;
    struct Fragment;

    /// A builder class to help construct NFA fragments.
    /// Only this class can create Fragment instances.
    class FragmentBuilder {
    public:
      FragmentBuilder& setStartState(State* state);
      // The double pointers that lead to the next states are copied to the new fragment.
      FragmentBuilder& attachToConnections(const Fragment& frag);
      /// The double pointer to the next state is copied to the new fragment so the destination state can be set later.
      /** The pointer must be dangling (i.e. pointing to nullptr), else an existing connection would be cut. */
      FragmentBuilder& attachToConnection(State** nextState);
      FragmentBuilder& setEndStates(std::vector<State*>& ptrList);
      Fragment build();
    private:
      State* startState;
      std::vector<State**> nextStates;
    };

  }

}
