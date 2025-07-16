#include "parse.h"

#include "NfaState.h"
#include "NfaBuilder.h"
#include "ExpressionBuilder.h"
#include "ExpressionBuildFunctions.h"

#include <iostream>
#include <span>
#include <future>

namespace regex {

  void addstate(std::vector<NfaState*>& list, NfaState* state, const size_t counter)
  {
    assert(state != nullptr);
    if (state->type == NfaState::Type::split) {
      for (const auto& nextState : state->nextStates)
        addstate(list, nextState, counter);
      return;
    }
    state->nSteps = counter;
    list.push_back(state);
  }

  void step(const std::vector<NfaState*>& currentStateList, std::vector<NfaState*>& nextStateList, char ch) {
    for (const auto currentState : currentStateList) {
      if (currentState->type == NfaState::Type::ch && currentState->ch.value_or(ch) == ch)
        for (const auto& nextState : currentState->nextStates)
          addstate(nextStateList, nextState, currentState->nSteps + 1);
    }
  }

  bool ismatch(const std::vector<const NfaState*>& stateList) {
    return std::any_of(stateList.cbegin(), stateList.cend(), [](const NfaState* state) {return state->type == NfaState::Type::match; });
  }

  void extractMatches(const std::vector<NfaState*>& stateList, std::vector<ParseResult>& matchList, const size_t counter) {
    for (const auto& state : stateList) {
      if (state->type == NfaState::Type::match)
        matchList.emplace_back(counter - (state->nSteps - 1), state->nSteps);
    }
  }

  std::vector<ParseResult> walkThroughNfa(NfaState* startState, std::string_view text) {
    std::vector<ParseResult> resultList;
    std::vector<NfaState*> cStateList, nStateList;
    std::vector<NfaState*>& currentStateList = cStateList;
    std::vector<NfaState*>& nextStateList = nStateList;
    size_t characterCounter = 0;
    
    for (const auto ch : text) {
      currentStateList.swap(nextStateList);
      nextStateList.clear();
      addstate(currentStateList, startState, 0);
      step(currentStateList, nextStateList, ch);
      extractMatches(nextStateList, resultList, characterCounter);
      ++characterCounter;
    }
    return resultList;
  }

  std::vector<ParseResult> parse(std::string_view text, std::string_view searchString) {
    if (text.empty() || searchString.empty())
      return {};

    ExpressionBuilder builder(&buildExpressionArgumentsFirstOperatorLast);
    Expression expr = builder.build(searchString);

    NfaBuilder nfaBuilder(expr);
    NfaFragment nfa = nfaBuilder.build();
    std::vector<ParseResult> results;
    
    return walkThroughNfa(nfa.startState, text);
  }

}
