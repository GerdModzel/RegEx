#include <gtest/gtest.h>

#include "Convenience.h"

void assertCharacter(const regex::Operator& realCh, regex::OperatorType expectedType, std::optional<char> expectedValue) {
  ASSERT_EQ(realCh.getType(), expectedType);
  if (expectedValue.has_value()) {
    ASSERT_TRUE(realCh.getValue().has_value());
    ASSERT_EQ(realCh.getValue().value(), expectedValue.value());
  } else {
    ASSERT_FALSE(realCh.getValue().has_value());
  }
}

TEST(ExpressionTest, Groupings) {
  regex::Expression expr = regex::buildNfaExpression("(a|b)*c");
  ASSERT_EQ(expr.toString(), "ab|*c&");
}

TEST(ExpressionTest, ReplaceCharsWithOperators) {
  auto characters = regex::convertStringToOpVector("a.+*?e");
  ASSERT_EQ(characters.size(), 6);
  assertCharacter(characters[0], regex::OperatorType::Literal, 'a');
  assertCharacter(characters[1], regex::OperatorType::Wildcard, std::nullopt);
  assertCharacter(characters[2], regex::OperatorType::OneOrMore, std::nullopt);
  assertCharacter(characters[3], regex::OperatorType::ZeroOrMore, std::nullopt);
  assertCharacter(characters[4], regex::OperatorType::ZeroOrOne, std::nullopt);
  assertCharacter(characters[5], regex::OperatorType::Literal, 'e');
}

TEST(ExpressionTest, AddConcatenationOperators) {
  std::vector<regex::Operator> input = {
    regex::Operator('a')
  , regex::Operator('b')
  , regex::Operator('c')
  , regex::Operator(regex::OperatorType::Alternation)
  , regex::Operator('d')
  , regex::Operator(regex::OperatorType::OneOrMore)
  , regex::Operator('e')
  };
  auto output = regex::addConcatenationOperators(input);
  ASSERT_EQ(output.size(), 10);
  assertCharacter(output[0], regex::OperatorType::Literal, 'a');
  assertCharacter(output[1], regex::OperatorType::Concatenation, std::nullopt);
  assertCharacter(output[2], regex::OperatorType::Literal, 'b');
  assertCharacter(output[3], regex::OperatorType::Concatenation, std::nullopt);
  assertCharacter(output[4], regex::OperatorType::Literal, 'c');
  assertCharacter(output[5], regex::OperatorType::Alternation, std::nullopt);
  assertCharacter(output[6], regex::OperatorType::Literal, 'd');
  assertCharacter(output[7], regex::OperatorType::OneOrMore, std::nullopt);
  assertCharacter(output[8], regex::OperatorType::Concatenation, std::nullopt);
  assertCharacter(output[9], regex::OperatorType::Literal, 'e');
}

TEST(ExpressionTest, Empty) {
  regex::Expression expr = regex::buildNfaExpression("");
  ASSERT_TRUE(expr.cbegin() == expr.cend());
}

TEST(ExpressionTest, SingleCharacter) {
  regex::Expression expr = regex::buildNfaExpression("a");
  ASSERT_EQ(expr.toString(), "a");
}

//TEST(ExpressionTest, Concatenation) {
 // regex::Expression expr("abc");
  //ASSERT_EQ(expr.toString(), "ab&c&");
//}


