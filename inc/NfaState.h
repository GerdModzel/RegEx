#pragma once

#include <string_view>
#include <stack>
#include <optional>
#include <memory>
#include <vector>
#include <cassert>

namespace regex {

  struct NfaState
  {
    enum class Type {ch, split, match};

    Type type;
    std::optional<char> ch;
    NfaState* nextStateUpper;
    NfaState* nextStateLower;
    int lastList;
  };

  struct NfaFragment {
     NfaState* startState;
    std::vector<NfaState*> nextStates;
  };

  void patch(std::vector<NfaState*>& ptrList, NfaState* state) {
    for (auto ptr : ptrList) {
      ptr->nextStateUpper = state;
      ptr->nextStateLower = state;
    }
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
          stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::ch, std::make_optional(ch), nullptr, nullptr));
          auto state = stateManager.back().get();
          auto output = state->nextStateUpper;
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState*>{output});
          break;
        }
      }
    }
    auto fragment = std::move(fragmentStack.top());
    fragmentStack.pop();
    stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, nullptr, nullptr));
    auto matchState = stateManager.back().get();
 
    patch(fragmentStack.top().nextStates, matchState);
    assert(fragmentStack.size() == 1);
    return fragmentStack.top();
  }

}