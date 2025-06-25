#include <gtest/gtest.h>

#include "prepareTextPartitionByNumberOfHits.h"

TEST(PartitionTextTest, PositionOutOfBounds) {
const size_t textLength = 123;
  const std::vector<regex::ParseResult> input{
      regex::ParseResult{150, 1} };

  ASSERT_THROW(calculateHitMask(input, textLength), std::out_of_range);
}

TEST(PartitionTextTest, LengthOutOfBounds) {
  const size_t textLength = 123;
  const std::vector<regex::ParseResult> input{
      regex::ParseResult{5, 1000} };

  ASSERT_THROW(calculateHitMask(input, textLength), std::out_of_range);
}

TEST(PartitionTextTest, NoHits) {
  const size_t textLength = 4;
  const std::vector<regex::ParseResult> input{};

  const regex::HitMask expectedReturn{0, 0, 0, 0};
  const regex::HitMask actualReturn = calculateHitMask(input, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}

TEST(PartitionTextTest, TwoBorderHits) {
  const size_t textLength = 7;
  const std::vector<regex::ParseResult> input{
    regex::ParseResult{0, 2}
  , regex::ParseResult{4, 3}
  };

  const regex::HitMask expectedReturn{ 1, 1, 0, 0, 1, 1, 1 };
  const regex::HitMask actualReturn = calculateHitMask(input, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}


TEST(PartitionTextTest, OneMiddleHit) {
  const size_t textLength = 7;
  const std::vector<regex::ParseResult> input{{3, 1}};

  const regex::HitMask expectedReturn{ 0, 0, 0, 1, 0, 0, 0 };
  const regex::HitMask actualReturn = calculateHitMask(input, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}

TEST(PartitionTextTest, OverlappingHits) {
  const size_t textLength = 11;
  const std::vector<regex::ParseResult> parseResults{
    regex::ParseResult{1, 6}
  , regex::ParseResult{1, 2}
  , regex::ParseResult{2, 7}
  , regex::ParseResult{10, 1}
  };

  const regex::HitMask expectedReturn{ 0, 2, 3, 2, 2, 2, 2, 1, 1, 0, 1 };
  const regex::HitMask actualReturn = calculateHitMask(parseResults, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}

