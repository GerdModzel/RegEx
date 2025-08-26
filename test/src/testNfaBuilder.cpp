#include <gtest/gtest.h>

#include "NfaBuilder.h"
#include "Expression.h"
#include "op/Operator.h"

using namespace regex;

//TEST(NfaBuilderTest, ConcatenationFragment) {
//  regex::Expression expr{ { Element('a'), Element('b'), Element(OperatorType::Concatenation) } };
//  auto fragment = NfaBuilder::createNfaFragment(expr);
//  ASSERT_EQ(fragment.startState->type, regex::NfaState::Type::ch);
//  ASSERT_EQ(fragment.startState->ch, 'a');
//  ASSERT_EQ(fragment.nextStates.size(), 1);
//
//  NfaState* nextState1 = *fragment.nextStates[1];
//  ASSERT_EQ(nextState1, nullptr);
//
//  NfaState* nextState0 = *fragment.nextStates[0];
//  ASSERT_EQ(nextState0->type, regex::NfaState::Type::ch);
//  ASSERT_EQ(nextState0->ch, 'b');
//  ASSERT_EQ(nextState0->nextStates[0], *fragment.nextStates[0]);
//  ASSERT_EQ(nextState0->nextStates[1], *fragment.nextStates[1]);
//}
