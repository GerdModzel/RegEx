#include "NfaBuilder.h"

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

void NfaBuilder::addConcatenationFragmentTo(FragmentStack& fragmentStack) {
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

void NfaBuilder::addAlternationFragmentTo(FragmentStack& fragmentStack) {
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

void NfaBuilder::addWildcardFragmentTo(FragmentStack& fragmentStack) {
  NfaStateOwner stateManager;

  std::vector<NfaState*> newStateNextStates{ 1, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::addLiteralFragmentTo(FragmentStack& fragmentStack) {
  std::vector<std::unique_ptr<NfaState>> stateManager;

  std::vector<NfaState*> newStateNextStates{ 1, nullptr };
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, newStateNextStates));

  NfaState* newFragStartState = newState.get();
  std::vector<NfaState**> newFragNextStates{ &newState->nextStates[0] };
  NfaFragment newFragment{ newFragStartState, newFragNextStates, std::move(stateManager) };
  fragmentStack.push(std::move(newFragment));
}

void NfaBuilder::addZeroOrMoreFragmentTo(FragmentStack& fragmentStack) {
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

void NfaBuilder::addOneOrMoreFragmentTo(FragmentStack& fragmentStack) {
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

void NfaBuilder::addZeroOrOneFragmentTo(FragmentStack& fragmentStack) {
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

void NfaBuilder::addSuccessStateTo(FragmentStack& fragmentStack) {
  auto& stateManager = fragmentStack.top().stateManager;

  std::vector<NfaState*> newStateNextStates{};
  auto& newState = stateManager.emplace_back(std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, newStateNextStates));

  connect(fragmentStack.top().nextStates, newState.get());
}

NfaFragment NfaBuilder::createNfaFragment(const regex::Expression& expr) {
  FragmentStack fragmentStack;

  for (auto cit = expr.cbegin(); cit != expr.cend(); ++cit) {
    const regex::Operator ch = *cit;
    switch (ch.getType()) {
    case OperatorType::Concatenation: {
      addConcatenationFragmentTo(fragmentStack);
      break;
    }
    case OperatorType::Alternation: {
      addAlternationFragmentTo(fragmentStack);
      break;
    }
    case OperatorType::Wildcard: {
      addWildcardFragmentTo(fragmentStack);
      break;
    }
    case OperatorType::ZeroOrMore: {
      addZeroOrMoreFragmentTo(fragmentStack);
      break;
    }
    case OperatorType::OneOrMore: {
      addOneOrMoreFragmentTo(fragmentStack);
      break;
    }
    case OperatorType::ZeroOrOne: {
      addZeroOrOneFragmentTo(fragmentStack);
      break;
    }
    case OperatorType::Literal: {
      addLiteralFragmentTo(fragmentStack);
      fragmentStack.top().stateManager.back()->ch = ch.getValue();
      break;
    }
    default:
      throw std::invalid_argument("Unsupported character type in NFA construction");
    }
  }
  addSuccessStateTo(fragmentStack);

  assert(fragmentStack.size() == 1);
  return std::move(fragmentStack.top());
}
