#include "NfaBuilder.h"

#include <cassert>

namespace regex {

	namespace {
	  void patch(std::vector<NfaState**>& ptrList, NfaState* state) {
    for (auto& nextState : ptrList)
      *nextState = state;
	  }
	}

	[[nodiscard]] NfaStateOwner transferOwnership(NfaStateOwner& oldOwner0, NfaStateOwner& oldOwner1) {
	  NfaStateOwner newOwner;
    newOwner.insert(newOwner.end(), std::make_move_iterator(oldOwner0.begin()), std::make_move_iterator(oldOwner0.end()));
    newOwner.insert(newOwner.end(), std::make_move_iterator(oldOwner1.begin()), std::make_move_iterator(oldOwner1.end()));
    return newOwner;
	}

	NfaFragment createNfaFragment(const regex::Expression& expr) {
    FragmentStack fragmentStack;

    for (auto cit = expr.cbegin(); cit!=expr.cend(); ++cit) {
      const regex::Operator ch = *cit;
      switch (ch.getType()) {
        case OperatorType::Concatenation: {
          assert(fragmentStack.size() >= 2);
          NfaFragment fragSecond = std::move(fragmentStack.top());
          fragmentStack.pop();
          NfaFragment fragFirst = std::move(fragmentStack.top());
          fragmentStack.pop();

          auto stateManager = transferOwnership(fragFirst.stateManager, fragSecond.stateManager);

          patch(fragFirst.nextStates, fragSecond.startState);
          fragmentStack.emplace(fragFirst.startState, fragSecond.nextStates, std::move(stateManager));
          break;
        }
        case OperatorType::Alternation: {
          assert(fragmentStack.size() >= 2);
          NfaFragment fragSecond = std::move(fragmentStack.top());
          fragmentStack.pop();
          NfaFragment fragFirst = std::move(fragmentStack.top());
          fragmentStack.pop();

          auto stateManager = transferOwnership(fragFirst.stateManager, fragSecond.stateManager);
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{fragFirst.startState, fragSecond.startState}));

          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{fragFirst.nextStates[0], fragSecond.nextStates[0]}, std::move(stateManager));
          break;
        }
        case OperatorType::Wildcard: {
          auto wildcardState = std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, std::vector<NfaState*>{1, nullptr});
          auto& output = wildcardState->nextStates[0];
          NfaStateOwner stateManager;
          stateManager.emplace_back( std::move(wildcardState) );
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&output}, std::move(stateManager));
          break;
        }
        case OperatorType::ZeroOrMore: {
          assert(fragmentStack.size() >= 1);
          NfaFragment frag = std::move(fragmentStack.top());
          fragmentStack.pop();
          std::vector<std::unique_ptr<NfaState>> stateManager{ std::move(frag.stateManager) };
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr}));
          auto zeroOrMoreState = stateManager.back().get();
          patch(frag.nextStates, zeroOrMoreState);
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&zeroOrMoreState->nextStates[1]}, std::move(stateManager));
          break;
       }
        case OperatorType::OneOrMore: {
          assert(fragmentStack.size() >= 1);
          NfaFragment frag = std::move(fragmentStack.top());
          fragmentStack.pop();
          auto oneOrMoreState = std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr});
          auto& outputNextState = oneOrMoreState->nextStates[1];
          std::vector<std::unique_ptr<NfaState>> stateManager{ std::move(frag.stateManager) };
          stateManager.emplace_back(std::move(oneOrMoreState));
          patch(frag.nextStates, stateManager.back().get());
          fragmentStack.emplace(frag.startState, std::vector<NfaState**>{&outputNextState}, std::move(stateManager));
          break;
        }
        case OperatorType::ZeroOrOne: {
          assert(fragmentStack.size() >= 1);
          NfaFragment frag = std::move(fragmentStack.top());
          fragmentStack.pop();
          std::vector<std::unique_ptr<NfaState>> stateManager{ std::move(frag.stateManager) };
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr}));
          auto zeroOrOneState = stateManager.back().get();
          fragmentStack.emplace(zeroOrOneState, std::vector<NfaState**>{&zeroOrOneState->nextStates[1], frag.nextStates[0]}, std::move(stateManager));
          break;
        }
        case OperatorType::Literal: {
          std::vector<std::unique_ptr<NfaState>> stateManager;
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::ch, ch.getValue(), std::vector<NfaState*>{1, nullptr}));
          auto literalState = stateManager.back().get();
          auto& output = literalState->nextStates[0];
          fragmentStack.emplace(literalState, std::vector<NfaState**>{&output}, std::move(stateManager));
          break;
        }
        default:
          throw std::invalid_argument("Unsupported character type in NFA construction");
      }
    }
    auto matchState = std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, std::vector<NfaState*>{});
    patch(fragmentStack.top().nextStates, matchState.get());
    fragmentStack.top().stateManager.emplace_back(std::move(matchState));

    assert(fragmentStack.size() == 1);
    return std::move(fragmentStack.top());
  }

}