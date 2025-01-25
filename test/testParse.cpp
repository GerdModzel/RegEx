#include <gtest/gtest.h>

#include "parse.h"

TEST(ParseTest, EmptyExpression) {
  const auto results = regex::parse("sdfsdf", "");
  ASSERT_TRUE(results.empty());
}

TEST(ParseTest, EmptyText) {
  const auto results = regex::parse("", "*");
  ASSERT_TRUE(results.empty());
}

TEST(ParseTest, characterSearch) {
  const auto results = regex::parse("abcdef", "bcd");
  ASSERT_EQ(results.size(), 1);
  const auto result = results.at(1);
  ASSERT_EQ(result.position(), 1);
  ASSERT_EQ(result.size(), 3);
}
