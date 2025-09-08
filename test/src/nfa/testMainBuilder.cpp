#include <gtest/gtest.h>

#include "nfa/MainBuilderForTest.h"
#include "op/AllOperatorIncludes.h"

#include <algorithm>
#include <utility>

using namespace regex::nfa;
using namespace regex::op;

class FragmentAnalyzer {
public:
  FragmentAnalyzer(Fragment nfa)
    : nfa(std::move(nfa)) {
  }
  size_t nConnections() const {
    return nfa.nextStates.size();
  }
  bool isStartState(State const * const state) const {
    return state == nfa.startState;
  }
  bool areConnectionsEqual(const State* state) const {
    auto nextStates = nfa.nextStates;
    if (nextStates.size() != state->nextStates.size())
      return false;
    for (size_t i = 0; i < nextStates.size(); ++i) {
      if (*nextStates[i] != state->nextStates[i])
        return false;
    }
    return true;
  }
  bool firstConnectionLeadsTo(State const * const state) const {
    return !nfa.nextStates.empty() && *nfa.nextStates.at(0) == state;
  }
  bool secondConnectionLeadsTo(State const * const state) const {
    return nfa.nextStates.size() >= 2 && *nfa.nextStates.at(1) == state;
  }

private:
  Fragment nfa;
};

class StateAnalyzer {
public:
  StateAnalyzer(State const * const state)
    : state(state) {
  }
  bool isType(const State::Type type) const {
    return state->type == type;
  }
  bool hasValue(const std::optional<char> ch) const {
    return state->ch == ch;
  }
  bool firstConnectionLeadsTo(State const * const nextState) const {
    return !state->nextStates.empty() && state->nextStates.at(0) == nextState;
  }
  bool secondConnectionLeadsTo(State const * const nextState) const {
    return state->nextStates.size() >= 2 && state->nextStates.at(1) == nextState;
  }

  bool hasNoConnections() const {
    return state->nextStates.empty();
  }
  bool firstConnectionIsDangling() const {
    return state->nextStates.size() == 1 && state->nextStates[0] == nullptr;
  }
  bool secondConnectionIsDangling() const {
    return state->nextStates.size() == 2 && state->nextStates[1] == nullptr;
  }
private:
  State const* const state;
};


TEST(NfaBuilder, LiteralFragment) {
  MainBuilderForTest builder = setUpNfaBuilder(Literal('a'));

  const size_t expectedStateCount = 1;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [literalState] = builder.getTheMostRecentStates<expectedStateCount>();

  StateAnalyzer stateAnalyzer(literalState);
  ASSERT_TRUE(stateAnalyzer.isType(State::Type::ch));
  ASSERT_TRUE(stateAnalyzer.hasValue('a'));
  ASSERT_TRUE(stateAnalyzer.firstConnectionIsDangling());

  ASSERT_EQ(builder.getFragmentStackSize(), 1);

  FragmentAnalyzer analyzer{ builder.popOneFragmentFromStack()};
  ASSERT_TRUE(analyzer.isStartState(literalState));
  ASSERT_TRUE(analyzer.areConnectionsEqual(literalState));
}

TEST(NfaBuilder, WildcardFragment) {
  MainBuilderForTest builder = setUpNfaBuilder(Wildcard());

  const size_t expectedStateCount = 1;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [wildcardState] = builder.getTheMostRecentStates<expectedStateCount>();

  StateAnalyzer stateAnalyzer(wildcardState);
  ASSERT_TRUE(stateAnalyzer.isType(State::Type::ch));
  ASSERT_TRUE(stateAnalyzer.hasValue(std::nullopt));
  ASSERT_TRUE(stateAnalyzer.firstConnectionIsDangling());

  ASSERT_EQ(builder.getFragmentStackSize(), 1);
  FragmentAnalyzer analyzer{ builder.popOneFragmentFromStack() };

  ASSERT_TRUE(analyzer.isStartState(wildcardState));
  ASSERT_TRUE(analyzer.areConnectionsEqual(wildcardState));
}

TEST(NfaBuilder, MatchFragment) {
  MainBuilderForTest builder = setUpNfaBuilder(Literal('a'), Match());

  const size_t expectedStateCount = 2;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [matchState, literalState] = builder.getTheMostRecentStates<expectedStateCount>();

  StateAnalyzer stateAnalyzer(matchState);
  ASSERT_TRUE(stateAnalyzer.isType(State::Type::match));
  ASSERT_TRUE(stateAnalyzer.hasNoConnections());

  ASSERT_EQ(builder.getFragmentStackSize(), 1);
  FragmentAnalyzer fragmentAnalyzer{ builder.popOneFragmentFromStack() };

  ASSERT_TRUE(fragmentAnalyzer.isStartState(literalState));
  ASSERT_TRUE(fragmentAnalyzer.areConnectionsEqual(literalState));
}

TEST(NfaBuilder, ConcatenationFragment) {
  MainBuilderForTest builder = setUpNfaBuilder(Literal('a'), Literal('b'), Concatenation());

  const size_t expectedStateCount = 2;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [literalState1, literalState0] = builder.getTheMostRecentStates<expectedStateCount>();

  ASSERT_EQ(builder.getFragmentStackSize(), 1);
  FragmentAnalyzer analyzer{ builder.popOneFragmentFromStack() };

  ASSERT_TRUE(analyzer.isStartState(literalState0));
  ASSERT_TRUE(analyzer.areConnectionsEqual(literalState1));
}


TEST(NfaBuilder, AlternationFragment) {
  MainBuilderForTest builder = setUpNfaBuilder(Literal('a'), Literal('b'), Alternation());

  const size_t expectedStateCount = 3;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [splitState, literalState1, literalState0] = builder.getTheMostRecentStates<expectedStateCount>();

  StateAnalyzer stateAnalyzer(splitState);
  ASSERT_TRUE(stateAnalyzer.isType(State::Type::split));
  ASSERT_TRUE(stateAnalyzer.firstConnectionLeadsTo(literalState0));
  ASSERT_TRUE(stateAnalyzer.secondConnectionLeadsTo(literalState1));

  ASSERT_EQ(builder.getFragmentStackSize(), 1);
  FragmentAnalyzer fragmentAnalyzer(builder.popOneFragmentFromStack());

  ASSERT_TRUE(fragmentAnalyzer.isStartState(splitState));
  ASSERT_TRUE(fragmentAnalyzer.nConnections() == 2);
  ASSERT_TRUE(fragmentAnalyzer.firstConnectionLeadsTo(literalState0->nextStates.at(0)));
  ASSERT_TRUE(fragmentAnalyzer.secondConnectionLeadsTo(literalState1->nextStates.at(0)));
}

TEST(NfaBuilder, ZeroOrMoreFragment) {
 MainBuilderForTest builder = setUpNfaBuilder(Literal('a'), ZeroOrMore());

  const size_t expectedStateCount = 2;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [splitState, literalState] = builder.getTheMostRecentStates<expectedStateCount>();

  StateAnalyzer stateAnalyzer(splitState);
  ASSERT_TRUE(stateAnalyzer.isType(State::Type::split));
  ASSERT_TRUE(stateAnalyzer.firstConnectionLeadsTo(literalState));
  ASSERT_TRUE(stateAnalyzer.secondConnectionIsDangling());

  ASSERT_EQ(builder.getFragmentStackSize(), 1);
  FragmentAnalyzer fragmentAnalyzer{ builder.popOneFragmentFromStack() };
  ASSERT_TRUE(fragmentAnalyzer.isStartState(splitState));
  ASSERT_TRUE(fragmentAnalyzer.nConnections() == 1);
  ASSERT_TRUE(fragmentAnalyzer.firstConnectionLeadsTo(splitState->nextStates.at(1)));
}

TEST(NfaBuilder, OneOrMoreFragment) {
  MainBuilderForTest builder = setUpNfaBuilder(Literal('a'), OneOrMore());

  const size_t expectedStateCount = 2;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [splitState, literalState] = builder.getTheMostRecentStates<expectedStateCount>();

  StateAnalyzer stateAnalyzer(splitState);
  ASSERT_TRUE(stateAnalyzer.isType(State::Type::split));
  ASSERT_TRUE(stateAnalyzer.firstConnectionLeadsTo(literalState));
  ASSERT_TRUE(stateAnalyzer.secondConnectionIsDangling());

  ASSERT_EQ(builder.getFragmentStackSize(), 1);
  FragmentAnalyzer fragmentAnalyzer{ builder.popOneFragmentFromStack() };
  ASSERT_TRUE(fragmentAnalyzer.isStartState(literalState));
  ASSERT_TRUE(fragmentAnalyzer.nConnections() == 1);
  ASSERT_TRUE(fragmentAnalyzer.firstConnectionLeadsTo(splitState->nextStates.at(1)));
}

TEST(NfaBuilder, ZeroOrOneFragment) {
  MainBuilderForTest builder = setUpNfaBuilder(Literal('a'), ZeroOrOne());

  const size_t expectedStateCount = 2;
  ASSERT_EQ(builder.getStateManagerSize(), expectedStateCount);
  auto [splitState, literalState] = builder.getTheMostRecentStates<expectedStateCount>();

  StateAnalyzer stateAnalyzer(splitState);
  ASSERT_TRUE(stateAnalyzer.isType(State::Type::split));
  ASSERT_TRUE(stateAnalyzer.firstConnectionLeadsTo(literalState));
  ASSERT_TRUE(stateAnalyzer.secondConnectionIsDangling());

  ASSERT_EQ(builder.getFragmentStackSize(), 1);
  FragmentAnalyzer fragmentAnalyzer{ builder.popOneFragmentFromStack() };
  ASSERT_TRUE(fragmentAnalyzer.isStartState(splitState));
  ASSERT_TRUE(fragmentAnalyzer.nConnections() == 2);
  ASSERT_TRUE(fragmentAnalyzer.secondConnectionLeadsTo(splitState->nextStates.at(1)));
  ASSERT_TRUE(fragmentAnalyzer.firstConnectionLeadsTo(literalState->nextStates.at(0)));
}

