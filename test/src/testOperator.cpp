#include <gtest/gtest.h>

#include "Operator.h"

using namespace regex;

TEST(Operator, AllTypes) {
  OpVector values;
  values.push_back(std::make_unique<Concatenation>());
  values.push_back(std::make_unique<Alternation>());
  values.push_back(std::make_unique<Wildcard>());
  values.push_back(std::make_unique<Literal>('a'));
  values.push_back(std::make_unique<ZeroOrOne>());
  values.push_back(std::make_unique<ZeroOrMore>());
  values.push_back(std::make_unique<OneOrMore>());
  //values.push_back(std::make_unique<Match>());
  values.push_back(std::make_unique<GroupingStart>());
  values.push_back(std::make_unique<GroupingEnd>());

  const std::vector<bool> isRepetitionExp{ false, false, false, false, true, true, true, /*false,*/ false, false };
  const std::vector<bool> isOperationExp{ true, true, false, false, true, true, true, /*false,*/ false, false };
  const std::vector<bool> isBinaryExp{ true, true, false, false, false, false, false, /*false,*/ false, false };
  const std::string representationExp{ "&|.a?*+()" };

  std::vector<bool> isRepetitionResult, isOperationResult, isBinaryResult;
  std::string representationResult;
  for (const auto& val : values) {
    isRepetitionResult.push_back(val->isRepetition());
    isOperationResult.push_back(val->isOperation());
    isBinaryResult.push_back(val->isBinaryOperation());
    representationResult.push_back(val->toChar());
  }

  EXPECT_EQ(isRepetitionResult, isRepetitionExp);
  EXPECT_EQ(isOperationResult, isOperationExp);
  EXPECT_EQ(isBinaryResult, isBinaryExp);
  EXPECT_EQ(representationResult, representationExp);
}

TEST(OperatorTest, LiteralConstructorAndGetters) {
  Literal op('a');
  EXPECT_EQ(op.getType(), OperatorType::Literal);
  EXPECT_EQ(op.getValue(), 'a');
}

TEST(OperatorTest, TypeConstructorAndGetters) {
  Wildcard op;
  EXPECT_EQ(op.getType(), OperatorType::Wildcard);
}

TEST(OperatorTest, EqualityOperator) {
  Literal op1('a');
  Literal op2('a');
  Literal op3('b');
  Wildcard op4;

  EXPECT_EQ(op1, op2);
  EXPECT_NE(op1, op3);
  EXPECT_NE(op1, op4);
}
TEST(OperatorTest, StreamOutput) {
  Literal op('a');
  std::ostringstream oss;
  oss << op;
  EXPECT_EQ(oss.str(), "a");
  Wildcard op2;
  oss.str(""); // Clear the stream
  oss << op2;
  EXPECT_EQ(oss.str(), ".");
}
