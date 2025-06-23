#include <gtest/gtest.h>

#include "parse.h"
#include "storageUtils.h"

#include <iostream>

TEST(ParseTest, EmptyExpression) {
  const auto results = regex::parse("sdfsdf", "");
  ASSERT_TRUE(results.empty());
}

TEST(ParseTest, EmptyText) {
  const auto results = regex::parse("", "k");
  ASSERT_TRUE(results.empty());
}

TEST(ParseTest, characterSearch) {
  const auto results = regex::parse("asbcdefbbcd", "bcd");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 2);
  ASSERT_EQ(results[0].size(), 3);
  ASSERT_EQ(results[1].position(), 8);
  ASSERT_EQ(results[1].size(), 3);
}

TEST(Benchmark, characterSearch) {
  const std::string text = loadWikiTestFile();
  const auto results = regex::parse(text, "automata");
  ASSERT_EQ(results.size(), 5);
}
