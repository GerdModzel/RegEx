#include <gtest/gtest.h>

#include "nfa/FragmentSearch.h"
#include "nfa/State.h"
#include "nfa/Complete.h"
#include "nfa/FragmentBuilder.h"
#include "nfa/StateBuilder.h"

#include <memory>
#include <optional>
#include <array>
#include <vector>
#include <tuple>

using namespace regex::nfa;

namespace {

  std::unique_ptr<State> createAlternation(State* aState, State* bState) {
    StateBuilder splitBuilder;
    splitBuilder.setType(State::Type::split);
    splitBuilder.connectToState(aState);
    splitBuilder.connectToState(bState);
    return splitBuilder.build();
  }

  void resetStateChain(State* currentState) {
    std::set<State*> visited;
    currentState->lastList = -1;
    visited.insert(currentState);
    for (auto& state : currentState->nextStates) {
      if (visited.find(state) == visited.end()) {
        resetStateChain(state);
      }
    }
  }

  void addMatchState(Fragment& fragment, std::vector<std::unique_ptr<State>>& stateManager) {
    StateBuilder matchBuilder;
    matchBuilder.setType(State::Type::match);
    matchBuilder.cutOffConnections();
    stateManager.push_back(matchBuilder.build());
    for (auto& nextState : fragment.nextStates)
      *nextState = stateManager.back().get();
  }

  template<typename T>
  std::unique_ptr<State> createState(const T& input, State* nextState) {
    StateBuilder builder;
    builder.setType(input);
    return builder.build();
  }

  template<typename Tuple, size_t... ints>
  constexpr auto createArray(const Tuple& tuple, std::index_sequence<ints...> series) {
    constexpr size_t size = sizeof...(ints);
    std::array<std::unique_ptr<State>, size> states;

    ((states[ints] = createState(std::get<ints>(tuple), nullptr)), ...);
    ([&] {
      if (ints + 1 < size)
        states[ints]->nextStates.push_back(states[ints + 1].get());
      } (), ...);

    return states;
  }

  template<typename... Types>
  constexpr auto createDaisyChainedStates(Types&& ...types) {
    std::tuple<Types...> typeTuple{ types... };
    std::make_integer_sequence<size_t, sizeof...(types)> intSeries;
    return createArray(typeTuple, intSeries);
  }

} // anonymous namespace




TEST(FragmentSearch, Literal) {
  auto [literalState, matchState] = createDaisyChainedStates('a', State::Type::match);
  {
    auto results = executeSearch("a", literalState.get());
    ASSERT_EQ(results.size(), 1);
  } {
    resetStateChain(literalState.get());
    auto results = executeSearch("b", literalState.get());
    ASSERT_TRUE(results.empty());
  }
}

TEST(FragmentSearch, Wildcard) {
  auto [wildcardState, matchState] = createDaisyChainedStates(std::nullopt, State::Type::match);
  {
    auto results = executeSearch("a", wildcardState.get());
    ASSERT_EQ(results.size(), 1);
  } {
    resetStateChain(wildcardState.get());
    auto results = executeSearch("b", wildcardState.get());
    ASSERT_EQ(results.size(), 1);
  }
}

TEST(FragmentSearch, Concatenation) {
  // Construct a simple NFA equivalent to the regex "ab" 
  auto [aState, bState, matchState] = createDaisyChainedStates('a', 'b', State::Type::match);

  {
    auto results = executeSearch("ab", aState.get());
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].position(), 1);
  } {
    resetStateChain(aState.get());
    auto results = executeSearch("ba", aState.get());
    ASSERT_TRUE(results.empty());
  }
}

TEST(FragmentSearch, AlternationMatch) {
  // Construct a simple NFA equivalent to the regex "a|b"
  auto [aState, aMatchState] = createDaisyChainedStates('a', State::Type::match);
  auto [bState, bMatchState] = createDaisyChainedStates('b', State::Type::match);
  auto splitState = createAlternation(aState.get(), bState.get());

  {
    auto results = executeSearch("a", splitState.get());
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].position(), 0);
  } {
    resetStateChain(splitState.get());
    auto results = executeSearch("b", splitState.get());
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].position(), 0);
  } {
    resetStateChain(splitState.get());
    auto results = executeSearch("c", splitState.get());
    ASSERT_EQ(results.size(), 0);
  } {
    resetStateChain(splitState.get());
    auto results = executeSearch("cab", splitState.get());
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].position(), 1);
    EXPECT_EQ(results[1].position(), 2);
  }
}

TEST(FragmentSearch, LastListConcatenation) {
  // Construct a simple NFA equivalent to the regex "ab" 
  auto [aState, bState, matchState] = createDaisyChainedStates('a', 'b', State::Type::match);

  {
    auto results = executeSearch("ab", aState.get());
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].position(), 1);
    EXPECT_EQ(aState->lastList, 1);
    EXPECT_EQ(bState->lastList, 0);
    EXPECT_EQ(matchState->lastList, 1);
  } {
    resetStateChain(aState.get());
    auto results = executeSearch("ba", aState.get());
    ASSERT_TRUE(results.empty());
    EXPECT_EQ(aState->lastList, 1);
    EXPECT_EQ(bState->lastList, 1);
    EXPECT_EQ(matchState->lastList, -1);
  }
}

TEST(FragmentSearch, LastListAlternation) {
  // Construct a simple NFA equivalent to the regex "a|b"
  auto [aState, aMatchState] = createDaisyChainedStates('a', State::Type::match);
  auto [bState, bMatchState] = createDaisyChainedStates('b', State::Type::match);
  auto splitState = createAlternation(aState.get(), bState.get());

  auto results = executeSearch("a", splitState.get());
  ASSERT_EQ(results.size(), 1);
  EXPECT_EQ(results[0].position(), 0);
  EXPECT_EQ(splitState->lastList, -1);
  EXPECT_EQ(aState->lastList, 0);
  EXPECT_EQ(bState->lastList, 0);
  EXPECT_EQ(aMatchState->lastList, 0);
  EXPECT_EQ(bMatchState->lastList, -1);
}
