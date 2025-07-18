#include <gtest/gtest.h>

#include "find.h"
#include "storageUtils.h"

#include <iostream>

TEST(FindTest, EmptyExpression) {
  ASSERT_THROW(regex::find("sdfsdf", ""), std::invalid_argument);
}

TEST(FindTest, EmptyText) {
  ASSERT_THROW(regex::find("", "k"), std::invalid_argument);
}

TEST(FindTest, characterSearch) {
  const auto results = regex::find("asbcdefbbcd", "bcd");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 4);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 10);
  ASSERT_EQ(results[1].size(), 1);
}

TEST(FindTest, characterSearchWithWildcard) {
  const auto results = regex::find("asbcdefbbbcccd", "b.c");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 10);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 11);
  ASSERT_EQ(results[1].size(), 1);
}

TEST(FindTest, characterSearchWithOneOrMore) {
  const auto results = regex::find("asbcdefbbbc", "b+c");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 3);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 10);
  ASSERT_EQ(results[1].size(), 1);
}

TEST(FindTest, characterSearchWithZeroOrMore) {
  const auto results = regex::find("ascdesbbbc", "sb*c");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 2);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 9);
  ASSERT_EQ(results[1].size(), 1);
}

TEST(FindTest, characterSearchWithZeroOrOne) {
  const auto results = regex::find("ascdesbcdesbbbc", "sb?c");
  ASSERT_EQ(results.size(), 2);
  ASSERT_EQ(results[0].position(), 2);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 7);
  ASSERT_EQ(results[1].size(), 1);
}

TEST(FindTest, characterSearchWithAlternation) {
  const auto results = regex::find("asbcdefbbbc", "b|c");
  ASSERT_EQ(results.size(), 6);
  ASSERT_EQ(results[0].position(), 2);
  ASSERT_EQ(results[0].size(), 1);
  ASSERT_EQ(results[1].position(), 3);
  ASSERT_EQ(results[1].size(), 1);
  ASSERT_EQ(results[2].position(), 7);
  ASSERT_EQ(results[2].size(), 1);
  ASSERT_EQ(results[3].position(), 8);
  ASSERT_EQ(results[3].size(), 1);
  ASSERT_EQ(results[4].position(), 9);
  ASSERT_EQ(results[4].size(), 1);
  ASSERT_EQ(results[5].position(), 10);
  ASSERT_EQ(results[5].size(), 1);
}

TEST(Benchmark, characterSearch) {
  const std::string text = loadWikiTestFile();
  const auto results = regex::find(text, "automata");
  ASSERT_EQ(results.size(), 5);
}
