#include <gtest/gtest.h>

#include "op/Types.h"
#include "Expression.h"
#include "op/Alternation.h"
#include "op/Literal.h"

using namespace regex;

TEST(ExpressionTest, AllInOne) {
  regex::op::Vector ops;
  ops.push_back(std::make_unique<regex::op::Literal>('c'));
  ops.push_back(std::make_unique<regex::op::Alternation>());
  Expression expr{ ops };

  EXPECT_EQ(expr.size(), 2);
  EXPECT_EQ(expr.empty(), false);

  op::Vector::const_iterator inputIt = ops.cbegin();
  for (auto outputIt = expr.cbegin(); outputIt != expr.cend(); ++outputIt) {
    EXPECT_EQ(**inputIt, **outputIt);
    ++inputIt;
  }

  for (size_t i = 0; i < expr.size(); ++i)
    EXPECT_EQ(*expr[i], *ops[i]);

  EXPECT_EQ(expr.toString(), "c|");
}

