#include "parse.h"

#include "NfaState.h"

#include <iostream>
#include <span>
#include <future>

namespace regex {

  void patch(std::vector<NfaState**>& ptrList, NfaState* state) {
    for (auto& nextState : ptrList)
      *nextState = state;
  }

  std::pair<NfaFragment, std::vector<std::unique_ptr<NfaState>>>  buildNfaList(const regex::Expression& expr) {
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
          auto& output = state->nextStates[0];
          fragmentStack.emplace(stateManager.back().get(), std::vector<NfaState**>{&output});
          break;
        }
      }
    }
    stateManager.push_back(std::make_unique<NfaState>(NfaState::Type::match, std::nullopt, std::vector<NfaState*>{}, 0));
    auto matchState = stateManager.back().get();
 
    patch(fragmentStack.top().nextStates, matchState);
    assert(fragmentStack.size() == 1);
    return {fragmentStack.top(), std::move(stateManager)};
  }

  void addstate(std::vector<const NfaState*>& list, const NfaState* state, const unsigned listId)
  {
    assert(state != nullptr);
//    if (state->lastList == listId)
 //     return;
  //  state->lastList = listId;
    if (state->type == NfaState::Type::split) {
      /* follow unlabeled arrows */
      for (const auto& nextState : state->nextStates)
        addstate(list, nextState, listId);
      return;
    }
    list.push_back(state);
  }

  std::vector<const NfaState*> step(const std::vector<const NfaState*>& oldStateList, char ch, const unsigned listId) {
    std::vector<const NfaState*> newStateList;
 
    for (const auto oldState : oldStateList) {
      if (oldState->type == NfaState::Type::ch && oldState->ch == ch)
        for (const auto& nextState : oldState->nextStates)
          addstate(newStateList, nextState, listId);
    }
    return newStateList;
  }

  bool ismatch(const std::vector<const NfaState*>& stateList) {
    return std::any_of(stateList.cbegin(), stateList.cend(), [](const NfaState* state) {return state->type == NfaState::Type::match; });
  }

  unsigned walkThroughNfa(const NfaState* startState, std::string_view text) {
    unsigned listId = 1;
    std::vector<const NfaState*> stateList;
    
    addstate(stateList, startState, listId);

    for (const auto ch : text) {
      ++listId;
      stateList = step(stateList, ch, listId);
      if (ismatch(stateList))
        return listId - 1;
      if (stateList.empty())
        return 0;
    }
    return 0;
  }




  std::vector<ParseResult> parse(std::string_view text, const regex::Expression& expr) {
    if (text.empty() || expr.empty())
      return {};

    const auto [nfa, stateManager] = buildNfaList(expr);
    std::vector<ParseResult> results;
    
    for (int i = 0; i < text.size(); ++i) {
      std::string_view subText = text.substr(i, text.size());
 
//      auto future = std::async(std::launch::async, [&nfa, subText]() {})

     const unsigned size = walkThroughNfa(nfa.startState, subText);
      if (size > 0)
        results.emplace_back(i, size);
    }
    return results;
  }

}
