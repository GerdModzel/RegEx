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

TEST(ExpressionTest_Empty, SingleCharacter) {
  regex::Expression expr("a");
  ASSERT_EQ(std::distance(expr.cbegin(), expr.cend()), 1);
  assertCharacter(*expr.cbegin(), regex::CharacterType::Literal, 'a');
}

TEST(ExpressionTest, ManyCharacters) {
  regex::Expression expr("abcde");
  ASSERT_EQ(std::distance(expr.cbegin(), expr.cend()), 9); // 5 characters + 4 concatenations
  auto it = expr.cbegin();
  assertCharacter(*it, regex::CharacterType::Literal, 'a');
  ++it;
  assertCharacter(*it, regex::CharacterType::Literal, 'b');
  ++it;
  assertCharacter(*it, regex::CharacterType::Concatenation, std::nullopt);
}

TEST(ExpressionTest, UnicodeCharacter) {
  regex::Expression expr("a\u00E9b");
  ASSERT_EQ(std::distance(expr.cbegin(), expr.cend()), 5); // 3 characters + 2 concatenations
  auto it = expr.cbegin();
  assertCharacter(*it, regex::CharacterType::Literal, 'a');
  ++it;
  assertCharacter(*it, regex::CharacterType::Literal, '\u00E9'); // é character
  ++it;
  assertCharacter(*it, regex::CharacterType::Concatenation, std::nullopt);
  ++it;
  assertCharacter(*it, regex::CharacterType::Literal, 'b');
  ++it;
  assertCharacter(*it, regex::CharacterType::Concatenation, std::nullopt);
}
