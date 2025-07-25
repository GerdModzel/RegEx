#include <gtest/gtest.h>

#include "Convenience.h"

TEST(ExpressionTest, AllInOne) {
  const regex::OpVector ops = { regex::Operator('c'), regex::Operator(regex::OperatorType::Alternation) };
  regex::Expression expr{ regex::OpVector{ops} }; // copy because Expression moves the vector
  EXPECT_EQ(expr.size(), 2);
  EXPECT_EQ(expr.empty(), false);

  regex::OpVector::const_iterator inputIt = ops.cbegin();
  for (auto outputIt = expr.cbegin(); outputIt != expr.cend(); ++outputIt) {
    EXPECT_EQ(*inputIt, *outputIt);
    ++inputIt;
  }

  for (size_t i = 0; i < expr.size(); ++i)
    EXPECT_EQ(expr[i], ops[i]);

  EXPECT_EQ(expr.toString(), "c|");
}

