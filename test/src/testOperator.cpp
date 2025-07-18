#include <gtest/gtest.h>

#include "Operator.h"

using namespace regex;

TEST(OperatorTest, LiteralConstructorAndGetters) {
  Operator op('a');
  EXPECT_EQ(op.getType(), OperatorType::Literal);
  ASSERT_TRUE(op.getValue().has_value());
  EXPECT_EQ(op.getValue().value(), 'a');
}

TEST(OperatorTest, TypeConstructorAndGetters) {
  Operator op(OperatorType::Wildcard);
  EXPECT_EQ(op.getType(), OperatorType::Wildcard);
  EXPECT_FALSE(op.getValue().has_value());
}

TEST(OperatorTest, EqualityOperator) {
  Operator op1('a');
  Operator op2('a');
  Operator op3('b');
  Operator op4(OperatorType::Wildcard);

  EXPECT_EQ(op1, op2);
  EXPECT_NE(op1, op3);
  EXPECT_NE(op1, op4);
}
TEST(OperatorTest, StreamOutput) {
  Operator op('a');
  std::ostringstream oss;
  oss << op;
  EXPECT_EQ(oss.str(), "Character(0, 'a')");
  Operator op2(OperatorType::Wildcard);
  oss.str(""); // Clear the stream
  oss << op2;
  EXPECT_EQ(oss.str(), "Character(1)");
}
