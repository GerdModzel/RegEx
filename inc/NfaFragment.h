#pragma once

#include <vector>
#include <memory>

namespace regex {

  namespace nfa {
    class FragmentBuilder;
    struct State;
  }


  /**
   * \brief Represents a fragment of an NFA (Non-deterministic Finite Automaton).
   * 
   * A fragment consists of a starting state and a list of next states that can be reached from the start state.
   * NfaFragments are used to build the NFA incrementally. The end result is a fragment that represents
   * the complete NFA and can be used to search for matches in a given text.
   * 
   * Only the FragmentBuilder class can create NfaFragments.
   */
  struct NfaFragment {
  private:
    NfaFragment(nfa::State* startState, std::vector<nfa::State**> nextStates);
  public:
    ~NfaFragment();
    NfaFragment(const NfaFragment& rhs) = delete;
    NfaFragment& operator=(const NfaFragment& rhs) = delete;
    NfaFragment(NfaFragment&& rhs) noexcept;
    NfaFragment& operator=(NfaFragment&& rhs) noexcept;
    nfa::State* startState;
    std::vector<nfa::State**> nextStates;
  private:
    friend class nfa::FragmentBuilder;
  };

}
