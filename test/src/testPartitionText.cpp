#include <gtest/gtest.h>

#include "prepareTextPartitionByNumberOfHits.h"

TEST(PartitionTextTest, NoHits) {
  const size_t textLength = 123;
  const std::vector<regex::ParseResult> parseResults{};
  const auto results = prepareTextPartitionByNumberOfHits(parseResults, textLength);
  ASSERT_TRUE(results.empty());
}
