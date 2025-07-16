#pragma once

#include "Expression.h"

#include <stack>

namespace regex {

  VectorChar buildExpressionArgumentsFirstOperatorLast(std::string_view searchString);


  VectorChar addConcatenationOperators(const VectorChar& input);
  VectorExpr convertToVectorExpression(const VectorChar& arg);
  std::stack<std::pair<VectorExpr::iterator, VectorExpr::iterator>> findOuterGroupings(VectorExpr::iterator begin, VectorExpr::iterator end);
  void mergeGroupings(VectorExpr::iterator begin, VectorExpr::iterator end);
  VectorExpr::iterator getPreviousCharacter(VectorExpr::iterator it, VectorExpr::iterator begin);
  VectorExpr::iterator getNextCharacter(VectorExpr::iterator it, VectorExpr::iterator end);
  void mergeOperatorsWithTwoArguments(const VectorExpr::iterator begin, const VectorExpr::iterator end, bool (*typeCheck)(const CharacterType));
  void mergeAlternations(VectorExpr::iterator begin, VectorExpr::iterator end);
  void mergeConcatenations(VectorExpr::iterator begin, VectorExpr::iterator end);
  void mergeRepetitions(VectorExpr::iterator begin, VectorExpr::iterator end);
  void orderExpression(VectorExpr::iterator begin, VectorExpr::iterator end);

}
