#include "gtest/gtest.h"

#include "ExpressionBuildFunctions.h"
#include "Expression.h"
#include "op/Literal.h"
#include "op/Concatenation.h"

#include <string>
#include <vector>
#include <stack>

using namespace regex;

namespace {

  TEST(BuildExpressionArgumentsFirstOperatorLast, Empty) {
    std::string_view input = "";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_TRUE(result.empty());
  }


  TEST(BuildExpressionArgumentsFirstOperatorLast, BasicAlternation) {
    std::string_view input = "a|b";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "ab|");
  }

  TEST(BuildExpressionArgumentsFirstOperatorLast, BasicConcatenation) {
    std::string_view input = "ab";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "ab&");
  }

 TEST(BuildExpressionArgumentsFirstOperatorLast, BasicRepetition) {
    std::string_view input = "a+";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "a+");
  }


 TEST(BuildExpressionArgumentsFirstOperatorLast, MultipleAlternations) {
    std::string_view input = "a|b|c";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "ab|c|");
  }

 TEST(BuildExpressionArgumentsFirstOperatorLast, MultipleConcatenations) {
    std::string_view input = "abc";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "ab&c&");
  }

 TEST(BuildExpressionArgumentsFirstOperatorLast, MultipleRepetitions) {
    std::string_view input = "a+b*";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    // Check that the last operator is concatenation if implemented as such
    EXPECT_EQ(convertOpVectorToString(result), "a+b*&");
  }


 TEST(BuildExpressionArgumentsFirstOperatorLast, ComplexConcatenation) {
    std::string_view input = "ab(ci|d+)e*f?g.h";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "ab&ci&d+|&e*&f?&g&.&h&");
  }


  TEST(BuildExpressionArgumentsFirstOperatorLast, UnmatchedGroupingStart) {
    std::string_view input = "(()c";
    EXPECT_THROW(buildExpressionArgumentsFirstOperatorLast(input), std::invalid_argument);
  }

  TEST(BuildExpressionArgumentsFirstOperatorLast, UnmatchedGroupingEnd) {
    std::string_view input = "()c)";
    EXPECT_THROW(buildExpressionArgumentsFirstOperatorLast(input), std::invalid_argument);
  }

  TEST(BuildExpressionArgumentsFirstOperatorLast, EmptyGrouping) {
    std::string_view input = "a()d";
    EXPECT_THROW(buildExpressionArgumentsFirstOperatorLast(input), std::invalid_argument);
  }

  TEST(BuildExpressionArgumentsFirstOperatorLast, ComplexGrouping) {
    std::string_view input = "e(d((a|b)c))((fg)*)";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "edab|c&&&fg&*&");
  }


  TEST(buildExpressionArgumentsFirstOperatorLast, LeadingBinaryOperator) {
    std::string_view input = "x(|a)y";
    EXPECT_THROW(buildExpressionArgumentsFirstOperatorLast(input), std::invalid_argument);
  }

  TEST(buildExpressionArgumentsFirstOperatorLast, TrailingBinaryOperator) {
    std::string_view input = "x(a|)y";
    EXPECT_THROW(buildExpressionArgumentsFirstOperatorLast(input), std::invalid_argument);
  }

  TEST(buildExpressionArgumentsFirstOperatorLast, NotTrailingBinaryOperator) {
    std::string_view input = "x(a|(bc))y";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "xabc&|&y&");
  }

 TEST(buildExpressionArgumentsFirstOperatorLast, LeadingUnaryOperator) {
    std::string_view input = "x(+a)y";
    EXPECT_THROW(buildExpressionArgumentsFirstOperatorLast(input), std::invalid_argument);
  }

  TEST(buildExpressionArgumentsFirstOperatorLast, TrailingUnaryOperator) {
    std::string_view input = "x(ab*)y";
    op::Vector result = buildExpressionArgumentsFirstOperatorLast(input);
    EXPECT_EQ(convertOpVectorToString(result), "xab*&&y&");
  }

  TEST(buildExpressionArgumentsFirstOperatorLast, DoubleBinaryOperators) {
    std::string_view input = "a||b";
    EXPECT_THROW(buildExpressionArgumentsFirstOperatorLast(input), std::invalid_argument);
  }



  TEST(addConcatenationOperators, ConcatenationInInput) {
    op::Vector input;
    input.push_back(std::make_unique<op::Literal>('a'));
    input.push_back(std::make_unique<op::Concatenation>());
    input.push_back(std::make_unique<op::Literal>('b'));
    EXPECT_THROW(addConcatenationOperators(input), std::invalid_argument);
  }


}
