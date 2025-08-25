#include <gtest/gtest.h>

#include "op/AllOperatorIncludes.h"
#include "op/Operator.h"
#include "op/Types.h"

using namespace regex;

TEST(Operator, AllTypes) {
  op::Vector values;
  values.push_back(std::make_unique<op::Concatenation>());
  values.push_back(std::make_unique<op::Alternation>());
  values.push_back(std::make_unique<op::Wildcard>());
  values.push_back(std::make_unique<op::Literal>('a'));
  values.push_back(std::make_unique<op::ZeroOrOne>());
  values.push_back(std::make_unique<op::ZeroOrMore>());
  values.push_back(std::make_unique<op::OneOrMore>());
  values.push_back(std::make_unique<op::Match>());
  values.push_back(std::make_unique<op::GroupingStart>());
  values.push_back(std::make_unique<op::GroupingEnd>());

  const std::vector<bool> isRepetitionExp{ false, false, false, false, true, true, true, false, false, false };
  const std::vector<bool> isBinaryExp{ true, true, false, false, false, false, false, false, false, false };
  const std::string representationExp{ "&|.a?*+#()" };

  std::vector<bool> isRepetitionResult, isOperationResult, isBinaryResult;
  std::string representationResult;
  for (const auto& val : values) {
    isRepetitionResult.push_back(val->isRepetition());
    isBinaryResult.push_back(val->isBinaryOperation());
    representationResult.push_back(val->toChar());
  }

  EXPECT_EQ(isRepetitionResult, isRepetitionExp);
  EXPECT_EQ(isBinaryResult, isBinaryExp);
  EXPECT_EQ(representationResult, representationExp);
}

TEST(OperatorTest, LiteralGetter) {
  op::Literal op('a');
  EXPECT_EQ(op.getValue(), 'a');
}

TEST(OperatorTest, EqualityOperator) {
  op::Literal op1('a');
  op::Literal op2('a');
  op::Literal op3('b');
  op::Wildcard op4;

  EXPECT_EQ(op1, op2);
  EXPECT_NE(op1, op3);
  EXPECT_NE(op1, op4);
}
TEST(OperatorTest, StreamOutput) {
  op::Literal op('a');
  std::ostringstream oss;
  oss << op;
  EXPECT_EQ(oss.str(), "a");
  op::Wildcard op2;
  oss.str(""); // Clear the stream
  oss << op2;
  EXPECT_EQ(oss.str(), ".");
}
