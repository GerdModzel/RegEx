#pragma once

#include <vector>
#include <memory>

namespace regex {

  struct NfaState;

  /**
   * \brief Represents a fragment of an NFA (Non-deterministic Finite Automaton).
   * 
   * A fragment consists of a starting state and a list of next states that can be reached from the start state.
   * NfaFragments are used to build the NFA incrementally. The end result is a fragment that represents
   * the complete NFA and can be used to search for matches in a given text.
   * 
   * Only the NfaBuilder class can create NfaFragments.
   */
  struct NfaFragment {
  private:
    NfaFragment(NfaState* startState, std::vector<NfaState**> nextStates);
  public:
    ~NfaFragment();
    NfaFragment(const NfaFragment& rhs) = delete;
    NfaFragment& operator=(const NfaFragment& rhs) = delete;
    NfaFragment(NfaFragment&& rhs) noexcept;
    NfaFragment& operator=(NfaFragment&& rhs) noexcept;
   NfaState* startState;
    std::vector<NfaState**> nextStates;
  private:
    friend class NfaBuilder;
  };

}
