#include <gtest/gtest.h>

#include "Operator.h"

using namespace regex;

TEST(OperatorTest, LiteralConstructorAndGetters) {
  Literal op('a');
  EXPECT_EQ(op.getType(), OperatorType::Literal);
  ASSERT_TRUE(op.getValue().has_value());
  EXPECT_EQ(op.getValue().value(), 'a');
}

TEST(OperatorTest, TypeConstructorAndGetters) {
  Wildcard op;
  EXPECT_EQ(op.getType(), OperatorType::Wildcard);
  EXPECT_FALSE(op.getValue().has_value());
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
  EXPECT_EQ(oss.str(), "Character(0, 'a')");
  Wildcard op2;
  oss.str(""); // Clear the stream
  oss << op2;
  EXPECT_EQ(oss.str(), "Character(1)");
}
