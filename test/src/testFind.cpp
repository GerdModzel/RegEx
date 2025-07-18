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
  const std::vector<regex::ParseResult> expected{ regex::ParseResult(4, 1), regex::ParseResult(10, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithWildcard) {
  const auto results = regex::find("asbcdefbbbcccd", "b.c");
  const std::vector<regex::ParseResult> expected{ regex::ParseResult(10, 1), regex::ParseResult(11, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithOneOrMore) {
  const auto results = regex::find("asbcdefbbbc", "b+c");
  const std::vector<regex::ParseResult> expected{ regex::ParseResult(3, 1), regex::ParseResult(10, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithZeroOrMore) {
  const auto results = regex::find("ascdesbbbc", "sb*c");
  const std::vector<regex::ParseResult> expected{ regex::ParseResult(2, 1), regex::ParseResult(9, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithZeroOrOne) {
  const auto results = regex::find("ascdesbcdesbbbc", "sb?c");
  const std::vector<regex::ParseResult> expected{ regex::ParseResult(2, 1), regex::ParseResult(7, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithAlternation) {
  const auto results = regex::find("asbcdefbbbc", "b|c");
  const std::vector<regex::ParseResult> expected{
    regex::ParseResult(2, 1),
    regex::ParseResult(3, 1),
    regex::ParseResult(7, 1),
    regex::ParseResult(8, 1),
    regex::ParseResult(9, 1),
    regex::ParseResult(10, 1)
  };
  ASSERT_EQ(results, expected);
}
TEST(FindTest, characterSearchWithGrouping) {
  const std::string text = "acdefabd";
  {
    const auto results = regex::find(text, "a(b|c)d");
    const std::vector<regex::ParseResult> expected{ regex::ParseResult(2, 1), regex::ParseResult(7, 1) };
    ASSERT_EQ(results, expected);
  } {
    const auto results = regex::find(text, "ab|cd");
    const std::vector<regex::ParseResult> expected{ regex::ParseResult(2, 1), regex::ParseResult(6, 1) };
    ASSERT_EQ(results, expected);
  }
}
TEST(Benchmark, characterSearch) {
  const std::string text = loadWikiTestFile();
  const auto results = regex::find(text, "automata");
  ASSERT_EQ(results.size(), 5);
}
