#include "parse.h"

#include "NfaState.h"
#include "NfaBuilder.h"

#include <iostream>
#include <span>
#include <future>

namespace regex {

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




  std::vector<ParseResult> parse(std::string_view text, std::string_view searchString) {
    if (text.empty() || searchString.empty())
      return {};

    Expression expr(searchString);

    NfaBuilder nfaBuilder(expr);
    const auto nfa = nfaBuilder.build();
    std::vector<ParseResult> results;
    
    for (int i = 0; i < text.size(); ++i) {
      std::string_view subText = text.substr(i, text.size());
 
     const unsigned size = walkThroughNfa(nfa.startState, subText);
     if (size > 0)
       results.emplace_back(i, size);
    }
    return results;
  }

}
