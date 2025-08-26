#include "NfaBuilder.h"

#include "op/Match.h"
#include "op/Literal.h"
#include "NfaComplete.h"

#include <cassert>

using namespace regex;

namespace {

  void connect(std::vector<NfaState**>& ptrList, NfaState* state) {
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

  connect(fragFirst.nextStates, fragSecond.startState);

  NfaState* newFragStartState = fragFirst.startState;
  std::vector<NfaState**> newFragNextStates{ fragSecond.nextStates };
  NfaFragment newFragment{ newFragStartState, newFragNextStates };
  fragmentStack.push(std::move(newFragment));
}

struct BuilderInfo {
  bool newState;
  NfaState::Type stateType;
  std::optional<char> stateValue;
  NfaState* nextState;
  bool newFragment;
  
};


void NfaBuilder::visit(op::Alternation const* const op) {
  assert(fragmentStack.size() >= 2);
  NfaFragment fragSecond = std::move(fragmentStack.top());
  fragmentStack.pop();
  NfaFragment fragFirst = std::move(fragmentStack.top());
  fragmentStack.pop();

  std::vector<NfaState*> newStateNextStates{ fragFirst.startState, fragSecond.startState };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ fragFirst.nextStates[0], fragSecond.nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::Wildcard const* const op) {
  std::vector<NfaState*> newStateNextStates{ 1, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::Literal const* const op) {
  std::vector<NfaState*> newStateNextStates{ 1, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::ch, op->getValue(), newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::ZeroOrMore const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  std::vector<NfaState*> newStateNextStates{ frag.startState, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  connect(frag.nextStates, newState.get());

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[1] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::OneOrMore const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  std::vector<NfaState*> newStateNextStates{ frag.startState, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  connect(frag.nextStates, newState.get());

  NfaState* newFragStartState = frag.startState;
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[1] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::ZeroOrOne const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();


  std::vector<NfaState*> newStateNextStates{ frag.startState, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[1], frag.nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::Match const* const op) {
  std::vector<NfaState*> newStateNextStates{};
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, newStateNextStates));

  connect(fragmentStack.top().nextStates, newState.get());
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
