#include <gtest/gtest.h>

#include "ParseResult.h"

#include <algorithm>
#include <ranges>
#include <vector>

TEST(ParseResultTest, ConstructorAndGet) {
  const regex::ParseResult parseResult{5, 10};
  ASSERT_EQ(parseResult.position(), 5);
  ASSERT_EQ(parseResult.size(), 10);
}

