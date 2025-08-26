#include <gtest/gtest.h>

#include "find.h"
#include "storageUtils.h"

#include <iostream>

TEST(FindTest, EmptyExpression) {
  ASSERT_THROW(regex::search("sdfsdf", ""), std::invalid_argument);
}

TEST(FindTest, EmptyText) {
  ASSERT_THROW(regex::search("", "k"), std::invalid_argument);
}

TEST(FindTest, characterSearch) {
  const auto results = regex::search("asbcdefbbcd", "bcd");
  const std::vector<regex::SearchResult> expected{ regex::SearchResult(4, 1), regex::SearchResult(10, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithWildcard) {
  const auto results = regex::search("asbcdefbbbcccd", "b.c");
  const std::vector<regex::SearchResult> expected{ regex::SearchResult(10, 1), regex::SearchResult(11, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithOneOrMore) {
  const auto results = regex::search("asbcdefbbbc", "b+c");
  const std::vector<regex::SearchResult> expected{ regex::SearchResult(3, 1), regex::SearchResult(10, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithZeroOrMore) {
  const auto results = regex::search("ascdesbbbc", "sb*c");
  const std::vector<regex::SearchResult> expected{ regex::SearchResult(2, 1), regex::SearchResult(9, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithZeroOrOne) {
  const auto results = regex::search("ascdesbcdesbbbc", "sb?c");
  const std::vector<regex::SearchResult> expected{ regex::SearchResult(2, 1), regex::SearchResult(7, 1) };
  ASSERT_EQ(results, expected);
}

TEST(FindTest, characterSearchWithAlternation) {
  const auto results = regex::search("asbcdefbbbc", "b|c");
  const std::vector<regex::SearchResult> expected{
    regex::SearchResult(2, 1),
    regex::SearchResult(3, 1),
    regex::SearchResult(7, 1),
    regex::SearchResult(8, 1),
    regex::SearchResult(9, 1),
    regex::SearchResult(10, 1)
  };
  ASSERT_EQ(results, expected);
}
TEST(FindTest, characterSearchWithGrouping) {
  const std::string text = "acdefabd";
  {
    const auto results = regex::search(text, "a(b|c)d");
    const std::vector<regex::SearchResult> expected{ regex::SearchResult(2, 1), regex::SearchResult(7, 1) };
    ASSERT_EQ(results, expected);
  } {
    const auto results = regex::search(text, "ab|cd");
    const std::vector<regex::SearchResult> expected{ regex::SearchResult(2, 1), regex::SearchResult(6, 1) };
    ASSERT_EQ(results, expected);
  }
}
TEST(Benchmark, characterSearch) {
  const std::string text = loadWikiTestFile();
  const auto results = regex::search(text, "automata");
  ASSERT_EQ(results.size(), 5);
}
