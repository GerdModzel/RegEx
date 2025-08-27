#pragma once

#include "NfaState.h"
#include "Expression.h"
#include "NfaFragment.h"
#include "NfaComplete.h"
#include "op/OperatorVisitor.h"

#include <stack>

namespace regex {

class StateBuilder {
public:
  void setType(NfaState::Type type) { stateType = type; stateValue = std::nullopt; }
  void setType(std::optional<char> value) { stateType = NfaState::Type::ch; stateValue = value; }
  void connectToFragment(NfaFragment& frag) { nextStates.push_back(frag.startState); }
  void createDanglingConnection() { nextStates.push_back(nullptr); }
  void cutOffConnections() { nextStates = {}; }
  std::unique_ptr<NfaState> build() { return std::make_unique<NfaState>(stateType, stateValue, nextStates); }
private:
  NfaState::Type stateType;
  std::optional<char> stateValue = std::nullopt;
  std::vector<NfaState*> nextStates;
};



  using FragmentStack = std::stack<NfaFragment>;

  class NfaBuilder : public op::OperatorVisitor {
  public:
    NfaBuilder() = default;

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
    std::vector<std::unique_ptr<NfaState>> stateManager;
  };

}



