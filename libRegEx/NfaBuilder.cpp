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

void NfaBuilder::visit(op::Concatenation const* const op) {
  assert(fragmentStack.size() >= 2);
  NfaFragment fragSecond = std::move(fragmentStack.top());
  fragmentStack.pop();
  NfaFragment fragFirst = std::move(fragmentStack.top());
  fragmentStack.pop();

  connectStates(fragFirst.nextStates, fragSecond.startState);

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(fragFirst.startState);
  fragBuilder.takeOverConnectionsFrom(fragSecond);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::Alternation const* const op) {
  assert(fragmentStack.size() >= 2);
  NfaFragment fragSecond = std::move(fragmentStack.top());
  fragmentStack.pop();
  NfaFragment fragFirst = std::move(fragmentStack.top());
  fragmentStack.pop();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(fragFirst);
  stateBuilder.connectToFragment(fragSecond);
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.takeOverConnectionsFrom(fragFirst);
  fragBuilder.takeOverConnectionsFrom(fragSecond);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::Wildcard const* const op) {
  StateBuilder stateBuilder;
  stateBuilder.setType(std::nullopt);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.attachToStateConnections(newState.get());
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::Literal const* const op) {
  StateBuilder stateBuilder;
  stateBuilder.setType(op->getValue());
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.attachToStateConnections(newState.get());
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::ZeroOrMore const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(frag);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  connectStates(frag.nextStates, newState.get());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.takeOverConnection(&newState->nextStates[1]);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::OneOrMore const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(frag);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  connectStates(frag.nextStates, newState.get());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(frag.startState);
  fragBuilder.takeOverConnection(&newState->nextStates[1]);
  fragmentStack.push(fragBuilder.build());
}

void NfaBuilder::visit(op::ZeroOrOne const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  StateBuilder stateBuilder;
  stateBuilder.setType(NfaState::Type::split);
  stateBuilder.connectToFragment(frag);
  stateBuilder.createDanglingConnection();
  auto& newState = stateManager.emplace_back(stateBuilder.build());

  FragmentBuilder fragBuilder;
  fragBuilder.setStartState(newState.get());
  fragBuilder.takeOverConnection(&newState->nextStates[1]);
  fragBuilder.takeOverConnection(frag.nextStates[0]);
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
