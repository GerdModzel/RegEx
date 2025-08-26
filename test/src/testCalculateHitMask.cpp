#include <gtest/gtest.h>

#include "calculateHitMask.h"

TEST(CalculateHitMaskTest, PositionOutOfBounds) {
const size_t textLength = 123;
  const std::vector<regex::SearchResult> input{
      regex::SearchResult{150, 1} };

  ASSERT_THROW(calculateHitMask(input, textLength), std::out_of_range);
}

TEST(CalculateHitMaskTest, LengthOutOfBounds) {
  const size_t textLength = 123;
  const std::vector<regex::SearchResult> input{
      regex::SearchResult{5, 1000} };

  ASSERT_THROW(calculateHitMask(input, textLength), std::out_of_range);
}

TEST(CalculateHitMaskTest, NoHits) {
  const size_t textLength = 4;
  const std::vector<regex::SearchResult> input{};

  const regex::HitMask expectedReturn{0, 0, 0, 0};
  const regex::HitMask actualReturn = calculateHitMask(input, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}

TEST(CalculateHitMaskTest, TwoBorderHits) {
  const size_t textLength = 7;
  const std::vector<regex::SearchResult> input{
    regex::SearchResult{0, 2}
  , regex::SearchResult{4, 3}
  };

  const regex::HitMask expectedReturn{ 1, 1, 0, 0, 1, 1, 1 };
  const regex::HitMask actualReturn = calculateHitMask(input, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}


TEST(CalculateHitMaskTest, OneMiddleHit) {
  const size_t textLength = 7;
  const std::vector<regex::SearchResult> input{{3, 1}};

  const regex::HitMask expectedReturn{ 0, 0, 0, 1, 0, 0, 0 };
  const regex::HitMask actualReturn = calculateHitMask(input, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}

TEST(CalculateHitMaskTest, OverlappingHits) {
  const size_t textLength = 11;
  const std::vector<regex::SearchResult> parseResults{
    regex::SearchResult{1, 6}
  , regex::SearchResult{1, 2}
  , regex::SearchResult{2, 7}
  , regex::SearchResult{10, 1}
  };

  const regex::HitMask expectedReturn{ 0, 2, 3, 2, 2, 2, 2, 1, 1, 0, 1 };
  const regex::HitMask actualReturn = calculateHitMask(parseResults, textLength);

  EXPECT_EQ(actualReturn, expectedReturn);
}

