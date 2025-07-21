#include "NfaFragmentSearch.h"

#include "NfaState.h"
#include "NfaFragment.h"

#include <vector>
#include <cassert>


namespace regex {

  namespace {

    /* Adds the state to the list if it has not been added yet.
    */
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

    /* Processes the current state list and checks if the character matches any of the NfaStates.
    */
    void step(const std::vector<NfaState*>& currentStateList, std::vector<NfaState*>& nextStateList, char ch, const int counter) {
      for (const auto currentState : currentStateList) {
        if (currentState->type == NfaState::Type::ch && currentState->ch.value_or(ch) == ch) { //  wildcard always matches (wildcard has no value)
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

  } // anonymous namespace

  /* When a character is processed, a check is made whether it matches any of the NfaStates in currentStateList.
  *  If it does, the state's next states are added to nextStateList. If any of the next states is a match state,
  *  a match was found and a ParseResult is created. For the next loop, nextStateList becomes the new currentStateList,
  *  and the new nextStateList is cleared except for the start state. The start state is always on the list.
  */
  std::vector<ParseResult> executeSearch(std::string_view text, NfaFragment* fragment) {
    std::vector<ParseResult> resultList;
    std::vector<NfaState*> cStateList, nStateList;
    std::vector<NfaState*>& currentStateList = cStateList;
    std::vector<NfaState*>& nextStateList = nStateList;
    int characterCounter = 0;

    for (const auto ch : text) {
      currentStateList.swap(nextStateList);
      nextStateList.clear();
      addStateToList(currentStateList, fragment->startState, characterCounter);
      step(currentStateList, nextStateList, ch, characterCounter);
      extractMatches(nextStateList, resultList, characterCounter);
      ++characterCounter;
    }
    return resultList;
  }

} // namespace regex
