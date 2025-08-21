#pragma once

#include "Expression.h"

#include <stack>

namespace regex {

  /** \brief Converts a string to an OpVector and orders it, preparing it for further processing.
  *
  * First, the characters are replaced by a vector of Operators. Then, the implicit concatenation operators
  * are added between characters. Then, the vector is ordered "arguments first, Operator last". That means
  * "ab|c" becomes "ab&c|", where | is the alternation Operator and & is the concatenation Operator.
  * Ordering also removes the grouping operators "()", so the final expression is a flat sequence of
  * Operators. This is the format expected by the NFA builder.
  *
  * @param searchString The string to convert.
  * @returns An OpVector containing the ordered Operators representing the characters in the string.
  */
  OpVector buildExpressionArgumentsFirstOperatorLast(std::string_view searchString);


  /// brief Adds explicit concatenation Operators to an OpVector.
  OpVector addConcatenationOperators(const OpVector& input);

  ///  converts a vector of Operators to a vector of vector of Operators. This is needed to handle grouping and operator precedence.
  OpDoubleVector convertToVectorExpression(OpVector& arg);

  /// Finds all outer groupings in an OpDoubleVector and returns a stack of pairs of iterators representing the start and end of each grouping.
  /// E.g. "(()) returns the two outer parentheses.
  std::stack<std::pair<OpDoubleVector::iterator, OpDoubleVector::iterator>> findOuterGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);

  /// Removes all groupings from the OpDoubleVector, merging the contents of each grouping into the surrounding context.
  void mergeGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);

  /// Returns the previous character in the OpDoubleVector, skipping empty vectors.
  OpDoubleVector::iterator getPreviousCharacter(OpDoubleVector::iterator it, OpDoubleVector::iterator begin);

  /// Returns the next character in the OpDoubleVector, skipping empty vectors.
  OpDoubleVector::iterator getNextCharacter(OpDoubleVector::iterator it, OpDoubleVector::iterator end);

  /// Merges binary operators with the preceding two arguments.
  void mergeBinaryOperators(const OpDoubleVector::iterator begin, const OpDoubleVector::iterator end, bool (*typeCheck)(const OperatorType));

  /// Merges alternation operators with the preceding two arguments.
  void mergeAlternations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);

  /// Merges concatenation operators with the preceding two arguments.
  void mergeConcatenations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);

  /// Merges repetition operators with the preceding argument.
  void mergeRepetitions(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);

  /// Orders the OpDoubleVector by merging groupings, repetitions, concatenations, and alternations.
  void orderExpression(OpDoubleVector::iterator begin, OpDoubleVector::iterator end);

}
