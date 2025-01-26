#include <gtest/gtest.h>

#include "parse.h"

TEST(ParseTest, EmptyExpression) {
  const auto results = regex::parse("sdfsdf", "");
  ASSERT_TRUE(results.empty());
}

TEST(ParseTest, EmptyText) {
  const auto results = regex::parse("", "k");
  ASSERT_TRUE(results.empty());
}

TEST(ParseTest, characterSearch) {
  const auto results = regex::parse("asbcdefbbcd", "bc.d.");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 2);
  ASSERT_EQ(results[0].size(), 3);
  ASSERT_EQ(results[1].position(), 8);
  ASSERT_EQ(results[1].size(), 3);
}
