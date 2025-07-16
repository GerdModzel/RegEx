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

TEST(ParseTest, characterSearchWithWildcard) {
  const auto results = regex::parse("asbcdefbbbcccd", "b.c");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 8);
  ASSERT_EQ(results[0].size(), 3);
  ASSERT_EQ(results[1].position(), 9);
  ASSERT_EQ(results[1].size(), 3);
}

TEST(ParseTest, characterSearchWithOneOrMore) {
  const auto results = regex::parse("asbcdefbbbc", "b+c");
  ASSERT_EQ(results.size(), 4);
  ASSERT_EQ(results[0].position(), 2);
  ASSERT_EQ(results[0].size(), 2);
  ASSERT_EQ(results[1].position(), 7);
  ASSERT_EQ(results[1].size(), 4);
  ASSERT_EQ(results[2].position(), 7);
  ASSERT_EQ(results[2].size(), 4);
  ASSERT_EQ(results[3].position(), 7);
  ASSERT_EQ(results[3].size(), 4);
}

TEST(ParseTest, characterSearchWithZeroOrMore) {
  const auto results = regex::parse("asbcdefbbbc", "b*c");
  ASSERT_EQ(results.size(), 6);
  ASSERT_EQ(results[0].position(), 3);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 3);
  ASSERT_EQ(results[1].size(), 1);
  ASSERT_EQ(results[2].position(), 10);
  ASSERT_EQ(results[2].size(), 1);
  ASSERT_EQ(results[3], results[2]);
  ASSERT_EQ(results[4], results[2]);
  ASSERT_EQ(results[5], results[2]);
}

TEST(ParseTest, characterSearchWithZeroOrOne) {
  const auto results = regex::parse("asbcdefbbbc", "b?c");
  ASSERT_EQ(results.size(), 4);
  ASSERT_EQ(results[0].position(), 3);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 3);
  ASSERT_EQ(results[1].size(), 1);
  ASSERT_EQ(results[2].position(), 10);
  ASSERT_EQ(results[2].size(), 1);
  ASSERT_EQ(results[3].position(), 10);
  ASSERT_EQ(results[3].size(), 1);

}

TEST(Benchmark, characterSearch) {
  const std::string text = loadWikiTestFile();
  const auto results = regex::parse(text, "automata");
  ASSERT_EQ(results.size(), 5);
}
