#include "createNfaFragment.h"

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
  fragmentStack.emplace(fragFirst.startState, fragSecond.nextStates, std::move(stateManager));
}

void NfaBuilder::addAlternationFragmentTo(FragmentStack& fragmentStack) {
  assert(fragmentStack.size() >= 2);
  NfaFragment fragSecond = std::move(fragmentStack.top());
  fragmentStack.pop();
  NfaFragment fragFirst = std::move(fragmentStack.top());
  fragmentStack.pop();

  auto stateManager = transferOwnership(fragFirst.stateManager, fragSecond.stateManager);
  stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{fragFirst.startState, fragSecond.startState}));

  fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{fragFirst.nextStates[0], fragSecond.nextStates[0]}, std::move(stateManager));
}

void NfaBuilder::addWildcardFragmentTo(FragmentStack& fragmentStack) {
  auto wildcardState = std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, std::vector<NfaState*>{1, nullptr});
  auto& output = wildcardState->nextStates[0];
  NfaStateOwner stateManager;
  stateManager.emplace_back(std::move(wildcardState));
  fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&output}, std::move(stateManager));
}

void NfaBuilder::addLiteralFragmentTo(FragmentStack& fragmentStack) {
  std::vector<std::unique_ptr<NfaState>> stateManager;
  stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, std::vector<NfaState*>{1, nullptr}));
  auto literalState = stateManager.back().get();
  auto& output = literalState->nextStates[0];
  fragmentStack.emplace(literalState, std::vector<NfaState**>{&output}, std::move(stateManager));
}

void NfaBuilder::addZeroOrMoreFragmentTo(FragmentStack& fragmentStack) {
  assert(fragmentStack.size() >= 1);
  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();
  std::vector<std::unique_ptr<NfaState>> stateManager{ std::move(frag.stateManager) };
  stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr}));
  auto zeroOrMoreState = stateManager.back().get();
  connect(frag.nextStates, zeroOrMoreState);
  fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&zeroOrMoreState->nextStates[1]}, std::move(stateManager));
}

void NfaBuilder::addOneOrMoreFragmentTo(FragmentStack& fragmentStack) {
  assert(fragmentStack.size() >= 1);
  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();
  auto oneOrMoreState = std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr});
  auto& outputNextState = oneOrMoreState->nextStates[1];
  std::vector<std::unique_ptr<NfaState>> stateManager{ std::move(frag.stateManager) };
  stateManager.emplace_back(std::move(oneOrMoreState));
  connect(frag.nextStates, stateManager.back().get());
  fragmentStack.emplace(frag.startState, std::vector<NfaState**>{&outputNextState}, std::move(stateManager));
}

void NfaBuilder::addZeroOrOneFragmentTo(FragmentStack& fragmentStack) {
  assert(fragmentStack.size() >= 1);
  NfaFragment frag = std::move(fragmentStack.top());
  fragmentStack.pop();
  std::vector<std::unique_ptr<NfaState>> stateManager{ std::move(frag.stateManager) };
  stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr}));
  auto zeroOrOneState = stateManager.back().get();
  fragmentStack.emplace(zeroOrOneState, std::vector<NfaState**>{&zeroOrOneState->nextStates[1], frag.nextStates[0]}, std::move(stateManager));
}

void NfaBuilder::addSuccessStateTo(FragmentStack& fragmentStack) {
  auto matchState = std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, std::vector<NfaState*>{});
  connect(fragmentStack.top().nextStates, matchState.get());
  fragmentStack.top().stateManager.emplace_back(std::move(matchState));
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
