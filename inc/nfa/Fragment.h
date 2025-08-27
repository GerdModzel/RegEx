#pragma once

#include <vector>
#include <memory>

namespace regex::nfa {

  class FragmentBuilder;
  struct State;


  /**
   * \brief Represents a fragment of an NFA (Non-deterministic Finite Automaton).
   * 
   * A fragment consists of a starting state and a list of next states that can be reached from the start state.
   * NfaFragments are used to build the NFA incrementally. The end result is a fragment that represents
   * the complete NFA and can be used to search for matches in a given text.
   * 
   * Only the FragmentBuilder class can create NfaFragments.
   */
  struct Fragment {
  private:
    Fragment(State* startState, std::vector<State**> nextStates);
  public:
    ~Fragment();
    Fragment(const Fragment& rhs) = delete;
    Fragment& operator=(const Fragment& rhs) = delete;
    Fragment(Fragment&& rhs) noexcept;
    Fragment& operator=(Fragment&& rhs) noexcept;
    State* startState;
    std::vector<State**> nextStates;
  private:
    friend class FragmentBuilder;
  };

}
