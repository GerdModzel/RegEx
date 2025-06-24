#include <gtest/gtest.h>

#include "prepareTextPartitionByNumberOfHits.h"

TEST(PartitionTextTest, PositionOutOfBounds) {
const size_t textLength = 123;
  const std::vector<regex::ParseResult> parseResults{
      regex::ParseResult{150, 1} };
  ASSERT_THROW(prepareTextPartitionByNumberOfHits(parseResults, textLength), std::out_of_range);
}

TEST(PartitionTextTest, LengthOutOfBounds) {
  const size_t textLength = 123;
  const std::vector<regex::ParseResult> parseResults{
      regex::ParseResult{5, 1000} };
  ASSERT_THROW(prepareTextPartitionByNumberOfHits(parseResults, textLength), std::out_of_range);
}

TEST(PartitionTextTest, NoHits) {
  const size_t textLength = 123;
  const std::vector<regex::ParseResult> parseResults{};
  const auto results = prepareTextPartitionByNumberOfHits(parseResults, textLength);
  ASSERT_EQ(results.size(), 1);
  const regex::TextPartitionInfo part{.partPosition = 0, .partLength = textLength, .nHits = 0};
  EXPECT_EQ(results[0], part);
}

TEST(PartitionTextTest, TwoBorderHits) {
  const size_t textLength = 123;
  const std::vector<regex::ParseResult> parseResults{
    regex::ParseResult{0, 10}
  , regex::ParseResult{113, 5}
  };
  const std::vector<regex::TextPartitionInfo> results = prepareTextPartitionByNumberOfHits(parseResults, textLength);
  ASSERT_EQ(results.size(), 3);
  auto toCompare = results.begin();
  {
    const regex::TextPartitionInfo part{ .partPosition = 0, .partLength = 10, .nHits = 1 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  }{
    const regex::TextPartitionInfo part{ .partPosition = 10, .partLength = textLength - 15, .nHits = 0 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  }{
    const regex::TextPartitionInfo part{ .partPosition = textLength - 5, .partLength = 5, .nHits = 1 };
    EXPECT_EQ(*toCompare, part);
  }
}


TEST(PartitionTextTest, OneMiddleHit) {
  const size_t textLength = 123;
  const std::vector<regex::ParseResult> parseResults{{5, 1}};
  const std::vector<regex::TextPartitionInfo> results = prepareTextPartitionByNumberOfHits(parseResults, textLength);
  ASSERT_EQ(results.size(), 3);
  auto toCompare = results.begin();
  {
    const regex::TextPartitionInfo part{ .partPosition = 0, .partLength = 5, .nHits = 0 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  }{
    const regex::TextPartitionInfo part{ .partPosition = 5, .partLength = 1, .nHits = 1 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  }{
    const regex::TextPartitionInfo part{ .partPosition = 6, .partLength = textLength - 6, .nHits = 0 };
    EXPECT_EQ(*toCompare, part);
  }
}

TEST(PartitionTextTest, OverlappingHits) {
  const size_t textLength = 123;
  const std::vector<regex::ParseResult> parseResults{
    regex::ParseResult{5, 8}
  , regex::ParseResult{10, 4}
  };
  const std::vector<regex::TextPartitionInfo> results = prepareTextPartitionByNumberOfHits(parseResults, textLength);
  ASSERT_EQ(results.size(), 3);
  auto toCompare = results.begin();
  {
    const regex::TextPartitionInfo part{.partPosition = 0, .partLength = 5, .nHits = 0 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  } {
    const regex::TextPartitionInfo part{.partPosition = 5, .partLength = 5, .nHits = 1 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  } {
    const regex::TextPartitionInfo part{.partPosition = 10, .partLength = 3, .nHits = 2 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  } {
    const regex::TextPartitionInfo part{.partPosition = 13, .partLength = 2, .nHits = 1 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  } {
    const regex::TextPartitionInfo part{.partPosition = 14, .partLength = textLength - 14, .nHits = 0 };
    EXPECT_EQ(*toCompare, part);
    ++toCompare;
  }
}
