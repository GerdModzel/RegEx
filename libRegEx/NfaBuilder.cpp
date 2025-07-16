#include "NfaBuilder.h"

namespace regex {

	namespace {
	  void patch(std::vector<NfaState**>& ptrList, NfaState* state) {
    for (auto& nextState : ptrList)
      *nextState = state;
	  }
	}


  NfaBuilder::NfaBuilder(const regex::Expression& i_expr)
  : expr(i_expr) {
  }

  NfaFragment NfaBuilder::build() {
    FragmentStack fragmentStack;

    for (auto cit = expr.cbegin(); cit!=expr.cend(); ++cit) {
      const regex::Operator ch = *cit;
      switch (ch.getType()) {
        case OperatorType::Concatenation: {
          assert(fragmentStack.size() >= 2);
          NfaFragment fragSecond = fragmentStack.top();
          fragmentStack.pop();
          NfaFragment fragFirst = fragmentStack.top();
          fragmentStack.pop();
          patch(fragFirst.nextStates, fragSecond.startState);
          fragmentStack.emplace(fragFirst.startState, fragSecond.nextStates);
          break;
        }
        case OperatorType::Wildcard: {
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::ch, std::nullopt, std::vector<NfaState*>{1, nullptr}, 0));
          auto state = stateManager.back().get();
          auto& output = state->nextStates[0];
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&output});
          break;
        }
        case OperatorType::ZeroOrMore: {
          assert(fragmentStack.size() >= 1);
          NfaFragment frag = fragmentStack.top();
          fragmentStack.pop();
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr}, 0));
          auto zeroOrMoreState = stateManager.back().get();
          patch(frag.nextStates, zeroOrMoreState);
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&zeroOrMoreState->nextStates[1]});
          break;
        }
        case OperatorType::OneOrMore: {
          assert(fragmentStack.size() >= 1);
          NfaFragment frag = fragmentStack.top();
          fragmentStack.pop();
          auto oneOrMoreState = std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr}, 0);
          auto& outputNextState = oneOrMoreState->nextStates[1];
          stateManager.push_back(std::move(oneOrMoreState));
          patch(frag.nextStates, stateManager.back().get());
          fragmentStack.emplace(frag.startState, std::vector<NfaState**>{&outputNextState});
          break;
        }
        case OperatorType::ZeroOrOne: {
          assert(fragmentStack.size() >= 1);
          NfaFragment frag = fragmentStack.top();
          fragmentStack.pop();
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::split, std::nullopt, std::vector<NfaState*>{frag.startState, nullptr}, 0));
          auto zeroOrOneState = stateManager.back().get();
          fragmentStack.emplace(zeroOrOneState, std::vector<NfaState**>{&zeroOrOneState->nextStates[1], frag.nextStates[0]});
          break;
        }
        case OperatorType::Literal: {
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::ch, ch.getValue(), std::vector<NfaState*>{1, nullptr}, 0));
          auto state = stateManager.back().get();
          auto& output = state->nextStates[0];
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&output});
          break;
        }
        default:
          throw std::exception("Unsupported character type in NFA construction");
      }
    }
    stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, std::vector<NfaState*>{}, 0));
    auto matchState = stateManager.back().get();
 
    patch(fragmentStack.top().nextStates, matchState);
    assert(fragmentStack.size() == 1);
    return fragmentStack.top();
  }

}