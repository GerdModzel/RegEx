#include <gtest/gtest.h>

#include "parse.h"

TEST(ParseTest, BasicTest) {
  const auto result = regex::parse("sdfsdf", "sdf");
  ASSERT_TRUE(result.empty());
}
