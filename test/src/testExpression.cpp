#include <gtest/gtest.h>

#include "Convenience.h"

using namespace regex;

TEST(ExpressionTest, AllInOne) {
  const OpVector ops = createOpVector( {Operator('c'), Operator(OperatorType::Alternation) });
  Expression expr{ ops };
  EXPECT_EQ(expr.size(), 2);
  EXPECT_EQ(expr.empty(), false);

  OpVector::const_iterator inputIt = ops.cbegin();
  for (auto outputIt = expr.cbegin(); outputIt != expr.cend(); ++outputIt) {
    EXPECT_EQ(**inputIt, **outputIt);
    ++inputIt;
  }

  for (size_t i = 0; i < expr.size(); ++i)
    EXPECT_EQ(*expr[i], *ops[i]);

  EXPECT_EQ(expr.toString(), "c|");
}

