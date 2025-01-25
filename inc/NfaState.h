#pragma once

#include <string_view>
#include <stack>
#include <optional>
#include <memory>
#include <vector>
#include <cassert>

#include <ParseResult.h>

namespace regex {

  struct NfaState
  {
    enum class Type {ch, split, match};

    Type type;
    std::optional<char> ch;
    std::vector<NfaState*> nextStates;
    int lastList;
  };

  struct NfaFragment {
     NfaState* startState;
    std::vector<NfaState*> nextStates;
  };

  void patch(std::vector<NfaState*>& ptrList, NfaState* state) {
    for (auto ptr : ptrList)
      for (auto nextState : ptr->nextStates)
        nextState = state;
  }

  NfaFragment buildNfaList(std::string_view expr) {

    std::stack<NfaFragment> fragmentStack;

    std::vector<std::unique_ptr<NfaState>> stateManager;

    for (auto ch : expr) {
      switch (ch) {
        case '.': {
          auto fragSecond = fragmentStack.top();
          fragmentStack.pop();
          auto fragFirst = fragmentStack.top();
          fragmentStack.pop();
          patch(fragFirst.nextStates, fragSecond.startState);
          fragmentStack.emplace(fragFirst.startState, fragSecond.nextStates);
          break;
        }
        default: {
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::ch, std::make_optional(ch), std::vector<NfaState*>{1, nullptr}, 0));
          auto state = stateManager.back().get();
          auto output = state->nextStates[0];
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState*>{output});
          break;
        }
      }
    }
    auto fragment = std::move(fragmentStack.top());
    fragmentStack.pop();
    stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, std::vector<NfaState*>{}, 0));
    auto matchState = stateManager.back().get();
 
    patch(fragmentStack.top().nextStates, matchState);
    assert(fragmentStack.size() == 1);
    return fragmentStack.top();
  }

  regex::ParseResult walkThroughNfa() {
    
  }

}