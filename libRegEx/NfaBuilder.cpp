#include "NfaBuilder.h"

#include "op/Match.h"
#include "op/Literal.h"

#include <cassert>

using namespace regex;

namespace {

  void connect(std::vector<NfaState**>& ptrList, NfaState* state) {
    for (auto& nextState : ptrList)
      *nextState = state;
  }

  [[nodiscard]] NfaStateOwner transferOwnership(NfaStateOwner& oldOwner0, NfaStateOwner& oldOwner1) {
    NfaStateOwner newOwner;
    newOwner.insert(newOwner.end(), std::make_move_iterator(oldOwner0.begin()), std::make_move_iterator(oldOwner0.end()));
    newOwner.insert(newOwner.end(), std::make_move_iterator(oldOwner1.begin()), std::make_move_iterator(oldOwner1.end()));
    return newOwner;
  }

}

void NfaBuilder::visit(op::Concatenation const* const op) {
  assert(fragmentStack.size() >= 2);
  NfaFragment fragSecond = std::move(fragmentStack.top());
  fragmentStack.pop();
  NfaFragment fragFirst = std::move(fragmentStack.top());
  fragmentStack.pop();

  auto stateManager = transferOwnership(fragFirst.stateManager, fragSecond.stateManager);

  connect(fragFirst.nextStates, fragSecond.startState);

  NfaState* newFragStartState = fragFirst.startState;
  std::vector<NfaState**> newFragNextStates{ fragSecond.nextStates };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::Alternation const* const op) {
  assert(fragmentStack.size() >= 2);
  NfaFragment fragSecond = std::move(fragmentStack.top());
  fragmentStack.pop();
  NfaFragment fragFirst = std::move(fragmentStack.top());
  fragmentStack.pop();

  auto stateManager = transferOwnership(fragFirst.stateManager, fragSecond.stateManager);

  std::vector<NfaState*> newStateNextStates{ fragFirst.startState, fragSecond.startState };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ fragFirst.nextStates[0], fragSecond.nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::Wildcard const* const op) {
  NfaStateOwner stateManager;

  std::vector<NfaState*> newStateNextStates{ 1, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::Literal const* const op) {
  std::vector<std::unique_ptr<NfaState>> stateManager;

  std::vector<NfaState*> newStateNextStates{ 1, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::ch, op->getValue(), newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::ZeroOrMore const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  auto stateManager{ std::move(frag.stateManager) };

  std::vector<NfaState*> newStateNextStates{ frag.startState, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  connect(frag.nextStates, newState.get());

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[1] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::OneOrMore const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  auto stateManager{ std::move(frag.stateManager) };

  std::vector<NfaState*> newStateNextStates{ frag.startState, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  connect(frag.nextStates, newState.get());

  NfaState* newFragStartState = frag.startState;
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[1] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::ZeroOrOne const* const op) {
  assert(fragmentStack.size() >= 1);

  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();

  auto stateManager{ std::move(frag.stateManager) };

  std::vector<NfaState*> newStateNextStates{ frag.startState, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[1], frag.nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::visit(op::Match const* const op) {
  auto& stateManager = fragmentStack.top().stateManager;

  std::vector<NfaState*> newStateNextStates{};
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, newStateNextStates));

  connect(fragmentStack.top().nextStates, newState.get());
}

NfaFragment NfaBuilder::createNfaFragment(const regex::Expression& expr) {
  for (auto cit = expr.cbegin(); cit != expr.cend(); ++cit) {
    regex::op::Operator* ch = cit->get();
    ch->accept(this);
  }
  op::Match matchOperator;
  matchOperator.accept(this);

  assert(fragmentStack.size() == 1);
  return std::move(fragmentStack.top());
}
