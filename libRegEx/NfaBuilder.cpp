#include "NfaBuilder.h"

#include "op/Match.h"
#include "op/Literal.h"
#include "NfaComplete.h"

#include <cassert>

using namespace regex;

namespace {

  void connectStates(std::vector<NfaState**>& ptrList, NfaState* state) {
    for (auto& nextState : ptrList)
      *nextState = state;
  }

}

void NfaBuilder::reset() {
  while (!fragmentStack.empty())
    fragmentStack.pop();
  stateManager.clear();
}

NfaFragment NfaBuilder::popOneFragmentFromStack() {
  assert(fragmentStack.size() >= 1);
  NfaFragment fragment = std::move(fragmentStack.top());
  fragmentStack.pop();
  return fragment;
}

std::pair<NfaFragment, NfaFragment> NfaBuilder::popTwoFragmentsFromStack() {
  assert(fragmentStack.size() >= 2);
  NfaFragment fragSecond = std::move(fragmentStack.top());
  fragmentStack.pop();
  NfaFragment fragFirst = std::move(fragmentStack.top());
  fragmentStack.pop();
  return { std::move(fragFirst), std::move(fragSecond) };
}

void NfaBuilder::visit(op::Concatenation const* const op) {
  auto [olderFragment, oldFragment] = popTwoFragmentsFromStack();

  connectStates(olderFragment.nextStates, oldFragment.startState);

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(olderFragment.startState);
  fragBuilder.takeOverConnectionsFrom(oldFragment);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::Alternation const* const op) {
  auto [olderFragment, oldFragment] = popTwoFragmentsFromStack();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(olderFragment);
  stateBuilder.connectToFragment(oldFragment);
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.takeOverConnectionsFrom(olderFragment);
  fragBuilder.takeOverConnectionsFrom(oldFragment);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::Wildcard const* const op) {
  StateBuilder stateBuilder;
  stateBuilder.setType(std::nullopt);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.setEndStates(newState->nextStates);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::Literal const* const op) {
  StateBuilder stateBuilder;
  stateBuilder.setType(op->getValue());
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.setEndStates(newState->nextStates);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::ZeroOrMore const* const op) {
  auto oldFragment = popOneFragmentFromStack();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(oldFragment);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  connectStates(oldFragment.nextStates, newState.get());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.takeOverConnection(&newState->nextStates[1]);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::OneOrMore const* const op) {
  auto oldFragment = popOneFragmentFromStack();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(oldFragment);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  connectStates(oldFragment.nextStates, newState.get());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(oldFragment.startState);
  fragBuilder.takeOverConnection(&newState->nextStates[1]);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::ZeroOrOne const* const op) {
  NfaFragment oldFragment = popOneFragmentFromStack();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(oldFragment);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.takeOverConnection(&newState->nextStates[1]);
  fragBuilder.takeOverConnection(oldFragment.nextStates[0]);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::Match const* const op) {
  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::match);
  stateBuilder.cutOffConnections();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  connectStates(fragmentStack.top().nextStates, newState.get());
}

NfaComplete NfaBuilder::createNfaFragment(const regex::Expression& expr) {
  reset();

  for (auto cit = expr.cbegin(); cit != expr.cend(); ++cit) {
    regex::op::Operator* op = cit->get();
    op->accept(this);
  }
  op::Match matchOperator;
  matchOperator.accept(this);

  assert(fragmentStack.size() == 1);
  NfaComplete nfaComplete{ std::move(fragmentStack.top()), std::move(stateManager) };

  return nfaComplete;
}
