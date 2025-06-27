#include <gtest/gtest.h>

#include "Expression.h"



void assertCharacter(const regex::Character& realCh, regex::CharacterType expectedType, std::optional<char> expectedValue) {
  ASSERT_EQ(realCh.type, expectedType);
  if (expectedValue.has_value()) {
    ASSERT_TRUE(realCh.value.has_value());
    ASSERT_EQ(realCh.value.value(), expectedValue.value());
  } else {
    ASSERT_FALSE(realCh.value.has_value());
  }
}

TEST(ExpressionTest, Empty) {
  regex::Expression expr("");
  ASSERT_TRUE(expr.cbegin() == expr.cend());
}

TEST(ExpressionTest, SingleCharacter) {
  regex::Expression expr("a");
  ASSERT_EQ(std::distance(expr.cbegin(), expr.cend()), 1);
  ASSERT_EQ(*expr.cbegin(), regex::Character{ 'a' });
  ASSERT_NE(*expr.cbegin(), regex::Character{ 'b' });
  ASSERT_NE(*expr.cbegin(), regex::Character{ regex::CharacterType::OneOrMore });
}

TEST(ExpressionTest, ManyCharacters) {
  regex::Expression expr("abc");
  ASSERT_EQ(std::distance(expr.cbegin(), expr.cend()), 5); // 3 characters + 2 concatenations
  auto it = expr.cbegin();
  ASSERT_EQ(*it, regex::Character{ 'a' });
  ASSERT_NE(*it, regex::Character{ 'b' });
  ASSERT_NE(*it, regex::Character{ regex::CharacterType::Concatenation });
  ++it;
  ASSERT_EQ(*it, regex::Character{ 'b' });
  ++it;
  ASSERT_NE(*it, regex::Character{ 'c' });
  ASSERT_EQ(*it, regex::Character{ regex::CharacterType::Concatenation });
  ++it;
  ASSERT_EQ(*it, regex::Character{ 'c' });
  ++it;
  ASSERT_EQ(*it, regex::Character{ regex::CharacterType::Concatenation });
  ++it;
  ASSERT_EQ(it, expr.cend());
}

