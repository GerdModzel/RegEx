#pragma once

#include <string_view>
#include <stack>

#include "NfaState.h"
#include "Expression.h"
#include "NfaFragment.h"

namespace regex {

  using FragmentStack = std::stack<NfaFragment>;

  class NfaBuilder {
  public:
    /** 
     * \brief Creates an NFA (Non-deterministic Finite Automaton) fragment from a regular expression.
     * 
     * The NFA fragment is built incrementally based on the operators in the expression.
     * The resulting fragment can be used to search for matches in a given text.
     * The method is described at https://swtch.com/~rsc/regexp/regexp1.html.
     * 
     * \param expr The regular expression to convert into an NFA fragment.
     * \return An NfaFragment representing the NFA for the given expression.
    */
    static NfaFragment createNfaFragment(const regex::Expression& expr);

    /// creates a new NFA fragment for the concatenation operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addConcatenationFragmentTo(FragmentStack& fragmentStack);
    /// creates a new NFA fragment for the alternation operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addAlternationFragmentTo(FragmentStack& fragmentStack);
    /// creates a new NFA fragment for the wildcard operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addWildcardFragmentTo(FragmentStack& fragmentStack);
    /// creates a new NFA fragment for the literal operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addLiteralFragmentTo(FragmentStack& fragmentStack);
    /// creates a new NFA fragment for the zero-or-one operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addZeroOrOneFragmentTo(FragmentStack& fragmentStack);
    /// creates a new NFA fragment for the one-or-more operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addOneOrMoreFragmentTo(FragmentStack& fragmentStack);
    /// creates a new NFA fragment for the zero-or-more operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addZeroOrMoreFragmentTo(FragmentStack& fragmentStack);
    /// creates a new NFA fragment that represents a successful match; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    static void addSuccessStateTo(FragmentStack& fragmentStack);
  };

}



