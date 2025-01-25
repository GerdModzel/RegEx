#pragma once

#include <string_view>
#include <stack>
#include <optional>
#include <memory>
#include <vector>
#include <cassert>
#include <algorithm>

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

  void addstate(std::vector<NfaState*>& list, NfaState* state, const unsigned listId)
  {
    if (state == nullptr || state->lastList == listId)
      return;
    state->lastList = listId;
    if (state->type == NfaState::Type::split) {
      /* follow unlabeled arrows */
      for (const auto& nextState : state->nextStates)
        addstate(list, nextState, listId);
      return;
    }
    list.push_back(state);
  }

  std::vector<NfaState*> step(const std::vector<NfaState*>& oldStateList, char ch, const unsigned listId) {
    std::vector<NfaState*> newStateList;
 
    for (const auto oldState : oldStateList) {
      if (oldState->type == NfaState::Type::ch && oldState->ch == ch)
        addstate(newStateList, oldState->nextStates[0], listId);
    }
    return newStateList;
  }

  bool walkThroughNfa(NfaState* startState, std::string_view expr) {
    unsigned listId = 1;
    std::vector<NfaState*> stateList;
    
    addstate(stateList, startState, listId);
    for (const auto ch : expr) {
      ++listId;
      stateList = step(stateList, ch, listId);
    }

    return std::any_of(stateList.begin(), stateList.end(), [](const NfaState* state) {return state->type == NfaState::Type::match; });
  }

}