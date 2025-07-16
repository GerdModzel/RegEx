#include <gtest/gtest.h>

#include "Expression.h"
#include "ExpressionBuilder.h"
#include "ExpressionBuildFunctions.h"


void assertCharacter(const regex::Character& realCh, regex::CharacterType expectedType, std::optional<char> expectedValue) {
  ASSERT_EQ(realCh.getType(), expectedType);
  if (expectedValue.has_value()) {
    ASSERT_TRUE(realCh.getValue().has_value());
    ASSERT_EQ(realCh.getValue().value(), expectedValue.value());
  } else {
    ASSERT_FALSE(realCh.getValue().has_value());
  }
}

TEST(ExpressionTest, Groupings) {
  regex::ExpressionBuilder builder{ &regex::buildExpressionArgumentsFirstOperatorLast };
  regex::Expression expr = builder.build("(a|b)*c");
  ASSERT_EQ(expr.toString(), "ab|*c&");
}

TEST(ExpressionTest, ReplaceCharsWithOperators) {
  auto characters = regex::convertStringToOperatorVector("a.+*?e");
  ASSERT_EQ(characters.size(), 6);
  assertCharacter(characters[0], regex::CharacterType::Literal, 'a');
  assertCharacter(characters[1], regex::CharacterType::Wildcard, std::nullopt);
  assertCharacter(characters[2], regex::CharacterType::OneOrMore, std::nullopt);
  assertCharacter(characters[3], regex::CharacterType::ZeroOrMore, std::nullopt);
  assertCharacter(characters[4], regex::CharacterType::ZeroOrOne, std::nullopt);
  assertCharacter(characters[5], regex::CharacterType::Literal, 'e');
}

TEST(ExpressionTest, AddConcatenationOperators) {
  std::vector<regex::Character> input = {
    regex::Character('a')
  , regex::Character('b')
  , regex::Character('c')
  , regex::Character(regex::CharacterType::Alternation)
  , regex::Character('d')
  , regex::Character(regex::CharacterType::OneOrMore)
  , regex::Character('e')
  };
  auto output = regex::addConcatenationOperators(input);
  ASSERT_EQ(output.size(), 10);
  assertCharacter(output[0], regex::CharacterType::Literal, 'a');
  assertCharacter(output[1], regex::CharacterType::Concatenation, std::nullopt);
  assertCharacter(output[2], regex::CharacterType::Literal, 'b');
  assertCharacter(output[3], regex::CharacterType::Concatenation, std::nullopt);
  assertCharacter(output[4], regex::CharacterType::Literal, 'c');
  assertCharacter(output[5], regex::CharacterType::Alternation, std::nullopt);
  assertCharacter(output[6], regex::CharacterType::Literal, 'd');
  assertCharacter(output[7], regex::CharacterType::OneOrMore, std::nullopt);
  assertCharacter(output[8], regex::CharacterType::Concatenation, std::nullopt);
  assertCharacter(output[9], regex::CharacterType::Literal, 'e');
}

TEST(ExpressionTest, Empty) {
  regex::ExpressionBuilder builder{ &regex::buildExpressionArgumentsFirstOperatorLast };
  regex::Expression expr = builder.build("");
  ASSERT_TRUE(expr.cbegin() == expr.cend());
}

TEST(ExpressionTest, SingleCharacter) {
  regex::ExpressionBuilder builder{ &regex::buildExpressionArgumentsFirstOperatorLast };
  regex::Expression expr = builder.build("a");
  ASSERT_EQ(expr.toString(), "a");
}

//TEST(ExpressionTest, Concatenation) {
 // regex::Expression expr("abc");
  //ASSERT_EQ(expr.toString(), "ab&c&");
//}


