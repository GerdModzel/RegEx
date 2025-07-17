#pragma once

#include "Expression.h"

#include <stack>

namespace regex {

  OpVector buildExpressionArgumentsFirstOperatorLast(std::string_view searchString);


  OpVector addConcatenationOperators(const OpVector& input);
  OpDoubleVector convertToVectorExpression(const OpVector& arg);
  std::stack<std::pair<OpDoubleVector::iterator, OpDoubleVector::iterator>> findOuterGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);
  void mergeGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);
  OpDoubleVector::iterator getPreviousCharacter(OpDoubleVector::iterator it, OpDoubleVector::iterator begin);
  OpDoubleVector::iterator getNextCharacter(OpDoubleVector::iterator it, OpDoubleVector::iterator end);
  void mergeBinaryOperators(const OpDoubleVector::iterator begin, const OpDoubleVector::iterator end, bool (*typeCheck)(const OperatorType));
  void mergeAlternations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);
  void mergeConcatenations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);
  void mergeRepetitions(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);
  void orderExpression(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);

}
