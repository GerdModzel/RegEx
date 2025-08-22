#pragma once

#include "Expression.h"

#include <stack>

namespace regex {

  /** \brief Converts a string to an op::Vector and orders it, preparing it for further processing.
  *
  * First, the characters are replaced by a vector of Operators. Then, the implicit concatenation operators
  * are added between characters. Then, the vector is ordered "arguments first, Operator last". That means
  * "ab|c" becomes "ab&c|", where | is the alternation Operator and & is the concatenation Operator.
  * Ordering also removes the grouping operators "()", so the final expression is a flat sequence of
  * Operators. This is the format expected by the NFA builder.
  *
  * @param searchString The string to convert.
  * @returns An op::Vector containing the ordered Operators representing the characters in the string.
  */
  op::Vector buildExpressionArgumentsFirstOperatorLast(std::string_view searchString);


  /// brief Adds explicit concatenation Operators to an op::Vector.
  op::Vector addConcatenationOperators(const op::Vector& input);

  ///  converts a vector of Operators to a vector of vector of Operators. This is needed to handle grouping and operator precedence.
  op::DoubleVector convertToVectorExpression(op::Vector& arg);

  /// Finds all outer groupings in an op::DoubleVector and returns a stack of pairs of iterators representing the start and end of each grouping.
  /// E.g. "(()) returns the two outer parentheses.
  std::stack<std::pair<op::DoubleVector::iterator, op::DoubleVector::iterator>> findOuterGroupings(op::DoubleVector::iterator begin, op::DoubleVector::iterator end);

  /// Removes all groupings from the op::DoubleVector, merging the contents of each grouping into the surrounding context.
  void mergeGroupings(op::DoubleVector::iterator begin, op::DoubleVector::iterator end);

  /// Returns the previous character in the op::DoubleVector, skipping empty vectors.
  op::DoubleVector::iterator getPreviousCharacter(op::DoubleVector::iterator it, op::DoubleVector::iterator begin);

  /// Returns the next character in the op::DoubleVector, skipping empty vectors.
  op::DoubleVector::iterator getNextCharacter(op::DoubleVector::iterator it, op::DoubleVector::iterator end);

  /// Merges binary operators with the preceding two arguments.
  void mergeBinaryOperators(const op::DoubleVector::iterator begin, const op::DoubleVector::iterator end, bool (*typeCheck)(const OperatorType));

  /// Merges alternation operators with the preceding two arguments.
  void mergeAlternations(op::DoubleVector::iterator begin, op::DoubleVector::iterator end);

  /// Merges concatenation operators with the preceding two arguments.
  void mergeConcatenations(op::DoubleVector::iterator begin, op::DoubleVector::iterator end);

  /// Merges repetition operators with the preceding argument.
  void mergeRepetitions(op::DoubleVector::iterator begin, op::DoubleVector::iterator end);

  /// Orders the op::DoubleVector by merging groupings, repetitions, concatenations, and alternations.
  void orderExpression(op::DoubleVector::iterator begin, op::DoubleVector::iterator end);

}
