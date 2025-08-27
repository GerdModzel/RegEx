#include "nfa/MainBuilder.h"

#include "op/Match.h"
#include "op/Literal.h"
#include "nfa/Complete.h"
#include "nfa/FragmentBuilder.h"
#include "nfa/StateBuilder.h"

#include <cassert>


namespace {

  void connectStates(std::vector<regex::nfa::State**>& ptrList, regex::nfa::State* state) {
    for (auto& nextState : ptrList)
      *nextState = state;
  }
}

namespace regex::nfa {

  void MainBuilder::reset() {
    while (!fragmentStack.empty())
      fragmentStack.pop();
    stateManager.clear();
  }

  Fragment MainBuilder::popOneFragmentFromStack() {
    assert(fragmentStack.size() >= 1);
    Fragment fragment = std::move(fragmentStack.top());
    fragmentStack.pop();
    return fragment;
  }

  std::pair<Fragment, Fragment> MainBuilder::popTwoFragmentsFromStack() {
    assert(fragmentStack.size() >= 2);
    Fragment fragSecond = std::move(fragmentStack.top());
    fragmentStack.pop();
    Fragment fragFirst = std::move(fragmentStack.top());
    fragmentStack.pop();
    return { std::move(fragFirst), std::move(fragSecond) };
  }

  void MainBuilder::visit(op::Concatenation const* const op) {
    auto [olderFragment, oldFragment] = popTwoFragmentsFromStack();

    connectStates(olderFragment.nextStates, oldFragment.startState);

    nfa::FragmentBuilder fragBuilder;
    fragBuilder.setStartState(olderFragment.startState);
    fragBuilder.takeOverConnectionsFrom(oldFragment);
    fragmentStack.push(fragBuilder.build());
  }

  void MainBuilder::visit(op::Alternation const* const op) {
    auto [olderFragment, oldFragment] = popTwoFragmentsFromStack();

    nfa::StateBuilder stateBuilder;
    stateBuilder.setType(State::Type::split);
    stateBuilder.connectToFragment(olderFragment);
    stateBuilder.connectToFragment(oldFragment);
    auto& newState = stateManager.emplace_back(stateBuilder.build());

    nfa::FragmentBuilder fragBuilder;
    fragBuilder.setStartState(newState.get());
    fragBuilder.takeOverConnectionsFrom(olderFragment);
    fragBuilder.takeOverConnectionsFrom(oldFragment);
    fragmentStack.push(fragBuilder.build());
  }

  void MainBuilder::visit(op::Wildcard const* const op) {
    nfa::StateBuilder stateBuilder;
    stateBuilder.setType(std::nullopt);
    stateBuilder.createDanglingConnection();
    auto& newState = stateManager.emplace_back(stateBuilder.build());

    nfa::FragmentBuilder fragBuilder;
    fragBuilder.setStartState(newState.get());
    fragBuilder.setEndStates(newState->nextStates);
    fragmentStack.push(fragBuilder.build());
  }

  void MainBuilder::visit(op::Literal const* const op) {
    nfa::StateBuilder stateBuilder;
    stateBuilder.setType(op->getValue());
    stateBuilder.createDanglingConnection();
    auto& newState = stateManager.emplace_back(stateBuilder.build());

    nfa::FragmentBuilder fragBuilder;
    fragBuilder.setStartState(newState.get());
    fragBuilder.setEndStates(newState->nextStates);
    fragmentStack.push(fragBuilder.build());
  }

  void MainBuilder::visit(op::ZeroOrMore const* const op) {
    auto oldFragment = popOneFragmentFromStack();

    nfa::StateBuilder stateBuilder;
    stateBuilder.setType(State::Type::split);
    stateBuilder.connectToFragment(oldFragment);
    stateBuilder.createDanglingConnection();
    auto& newState = stateManager.emplace_back(stateBuilder.build());

    connectStates(oldFragment.nextStates, newState.get());

    nfa::FragmentBuilder fragBuilder;
    fragBuilder.setStartState(newState.get());
    fragBuilder.takeOverConnection(&newState->nextStates[1]);
    fragmentStack.push(fragBuilder.build());
  }

  void MainBuilder::visit(op::OneOrMore const* const op) {
    auto oldFragment = popOneFragmentFromStack();

    nfa::StateBuilder stateBuilder;
    stateBuilder.setType(State::Type::split);
    stateBuilder.connectToFragment(oldFragment);
    stateBuilder.createDanglingConnection();
    auto& newState = stateManager.emplace_back(stateBuilder.build());

    connectStates(oldFragment.nextStates, newState.get());

    nfa::FragmentBuilder fragBuilder;
    fragBuilder.setStartState(oldFragment.startState);
    fragBuilder.takeOverConnection(&newState->nextStates[1]);
    fragmentStack.push(fragBuilder.build());
  }

  void MainBuilder::visit(op::ZeroOrOne const* const op) {
    Fragment oldFragment = popOneFragmentFromStack();

    nfa::StateBuilder stateBuilder;
    stateBuilder.setType(State::Type::split);
    stateBuilder.connectToFragment(oldFragment);
    stateBuilder.createDanglingConnection();
    auto& newState = stateManager.emplace_back(stateBuilder.build());

    nfa::FragmentBuilder fragBuilder;
    fragBuilder.setStartState(newState.get());
    fragBuilder.takeOverConnection(&newState->nextStates.at(1));
    fragBuilder.takeOverConnection(oldFragment.nextStates.at(0));
    fragmentStack.push(fragBuilder.build());
  }

  void MainBuilder::visit(op::Match const* const op) {
    nfa::StateBuilder stateBuilder;
    stateBuilder.setType(State::Type::match);
    stateBuilder.cutOffConnections();
    auto& newState = stateManager.emplace_back(stateBuilder.build());

    connectStates(fragmentStack.top().nextStates, newState.get());
  }

  Complete MainBuilder::createNfaFragment(const regex::Expression& expr) {
    reset();

    for (auto cit = expr.cbegin(); cit != expr.cend(); ++cit) {
      regex::op::Operator* op = cit->get();
      op->accept(this);
    }
    op::Match matchOperator;
    matchOperator.accept(this);

    assert(fragmentStack.size() == 1);
    Complete nfaComplete{ std::move(fragmentStack.top()), std::move(stateManager) };

    return nfaComplete;
  }

}
