#pragma once

#include "nfa/State.h"
#include "Expression.h"
#include "NfaFragment.h"
#include "NfaComplete.h"
#include "op/OperatorVisitor.h"

#include <stack>

namespace regex::nfa {

  using FragmentStack = std::stack<NfaFragment>;

  class MainBuilder : public op::OperatorVisitor {
  public:
    MainBuilder() = default;

    /** 
     * \brief Creates an NFA (Non-deterministic Finite Automaton) fragment from a regular expression.
     * 
     * The NFA fragment is built incrementally based on the operators in the expression.
     * The resulting fragment can be used to search for matches in a given text.
     * The method is described at https://swtch.com/~rsc/regexp/regexp1.html.
     * 
     * \param expr The regular expression to convert into an NFA fragment.
     * \return An NfaComplete object representing the complete NFA for the given expression.
    */
    NfaComplete createNfaFragment(const regex::Expression& expr);
    NfaFragment popOneFragmentFromStack();
    std::pair<NfaFragment, NfaFragment> popTwoFragmentsFromStack();

    /// creates a new NFA fragment for the concatenation operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::Concatenation const* const op) override;
    /// creates a new NFA fragment for the alternation operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::Alternation const* const op) override;
    /// creates a new NFA fragment for the wildcard operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::Wildcard const* const op) override;
    /// creates a new NFA fragment for the literal operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::Literal const* const op) override;
    /// creates a new NFA fragment for the zero-or-one operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::ZeroOrOne const* const op) override;
    /// creates a new NFA fragment for the one-or-more operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::OneOrMore const* const op) override;
    /// creates a new NFA fragment for the zero-or-more operator; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::ZeroOrMore const* const op) override;
    /// creates a new NFA fragment that represents a successful match; for details, see https://swtch.com/~rsc/regexp/regexp1.html.
    void visit(op::Match const* const op) override;
  private:
    void reset();

    FragmentStack fragmentStack;
    std::vector<std::unique_ptr<State>> stateManager;
  };

}



