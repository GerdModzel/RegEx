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
    std::stack<NfaFragment> fragmentStack;

    for (auto cit = expr.cbegin(); cit!=expr.cend(); ++cit) {
      const regex::Character ch = *cit;
      switch (ch.type) {
        case CharacterType::Concatenation: {
          assert(fragmentStack.size() >= 2);
          auto fragSecond = fragmentStack.top();
          fragmentStack.pop();
          auto fragFirst = fragmentStack.top();
          fragmentStack.pop();
          patch(fragFirst.nextStates, fragSecond.startState);
          fragmentStack.emplace(fragFirst.startState, fragSecond.nextStates);
          break;
        }
        case CharacterType::Literal: {
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::ch, ch.value, std::vector<NfaState*>{1, nullptr}, 0));
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