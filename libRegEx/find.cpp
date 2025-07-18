#include "find.h"

#include "NfaState.h"
#include "NfaBuilder.h"
#include "ExpressionBuilder.h"
#include "ExpressionBuildFunctions.h"

#include <iostream>
#include <span>
#include <future>

namespace regex {

  void addStateToList(std::vector<NfaState*>& list, NfaState* state, const int counter)
  {
    assert(state != nullptr);
    if (state->lastList == counter)
      return; // already added to the list
    if (state->type == NfaState::Type::split) {
      for (const auto& nextState : state->nextStates)
        addStateToList(list, nextState, counter);
      return;
    }
    state->lastList = counter;
    list.push_back(state);
  }

  void step(const std::vector<NfaState*>& currentStateList, std::vector<NfaState*>& nextStateList, char ch, const int counter) {
    for (const auto currentState : currentStateList) {
      if (currentState->type == NfaState::Type::ch && currentState->ch.value_or(ch) == ch) { // matches character and wildcard (wildcard has no value)
        assert(currentState->nextStates.size() == 1);
        addStateToList(nextStateList, currentState->nextStates[0], counter);
      }
    }
  }

  void extractMatches(const std::vector<NfaState*>& stateList, std::vector<ParseResult>& matchList, const int counter) {
    for (const auto& state : stateList) {
      if (state->type == NfaState::Type::match)
        matchList.emplace_back(static_cast<size_t>(counter), 1u);
    }
  }

  std::vector<ParseResult> walkThroughNfa(NfaState* startState, std::string_view text) {
    std::vector<ParseResult> resultList;
    std::vector<NfaState*> cStateList, nStateList;
    std::vector<NfaState*>& currentStateList = cStateList;
    std::vector<NfaState*>& nextStateList = nStateList;
    int characterCounter = 0;
    
    for (const auto ch : text) {
      currentStateList.swap(nextStateList);
      nextStateList.clear();
      addStateToList(currentStateList, startState, characterCounter);
      step(currentStateList, nextStateList, ch, characterCounter);
      extractMatches(nextStateList, resultList, characterCounter);
      ++characterCounter;
    }
    return resultList;
  }

  std::vector<ParseResult> find(std::string_view text, std::string_view searchString) {
    if (text.empty() || searchString.empty())
      throw std::invalid_argument("text or search string cannot be empty");

    ExpressionBuilder builder(&buildExpressionArgumentsFirstOperatorLast);
    Expression expr = builder.build(searchString);

    NfaBuilder nfaBuilder(expr);
    NfaFragment nfa = nfaBuilder.build();
    std::vector<ParseResult> results;
    
    return walkThroughNfa(nfa.startState, text);
  }

}
