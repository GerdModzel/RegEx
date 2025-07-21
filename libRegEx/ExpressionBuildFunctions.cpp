#include "ExpressionBuildFunctions.h"

#include <cassert>
#include <stack>
#include <vector>

namespace regex {

  OpVector addConcatenationOperators(const OpVector& input) {
    OpVector output;
    for (auto it = input.begin(); it != input.end(); ++it) {
      if (it->getType() == OperatorType::Concatenation)
        throw std::invalid_argument("Unexpected concatenation operator in input");

      output.push_back(*it);

      if (
        it->getType() != OperatorType::Alternation
        && it->getType() != OperatorType::GroupingStart
        && std::next(it) != input.end()
        && std::next(it)->getType() != OperatorType::GroupingEnd
        && !isOperation(std::next(it)->getType())) {
        output.emplace_back(OperatorType::Concatenation);
      }
    }
    return output;
  }

  OpDoubleVector convertToVectorExpression(const OpVector& arg) {
    OpDoubleVector result;
    for (const auto& el : arg)
      result.emplace_back(1, el);
    return result;
  }

  std::stack<std::pair<OpDoubleVector::iterator, OpDoubleVector::iterator>> findOuterGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    OpVector groupingStart{ 1, Operator{OperatorType::GroupingStart} };
    OpVector groupingEnd{ 1, Operator{OperatorType::GroupingEnd} };
    std::stack<OpDoubleVector::iterator> groupingStartStack;
    std::stack<std::pair<OpDoubleVector::iterator, OpDoubleVector::iterator>> groupingStack;
    for (auto it = begin; it != end; ++it) {
      if (*it == groupingStart) {
        groupingStartStack.push(it);
      }
      else if (*it == groupingEnd) {
        if (groupingStartStack.empty())
          throw std::invalid_argument("Unmatched grouping end");
        auto start = groupingStartStack.top();
        groupingStartStack.pop();
        if (groupingStartStack.empty())
          groupingStack.push({ start, it });
      }
    }
    if (!groupingStartStack.empty())
      throw std::invalid_argument("Unmatched grouping start");
    return groupingStack;
  }

  /* OpDoubleVector means that
   */
  void mergeGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    auto groupingStack = findOuterGroupings(begin, end);
    while (!groupingStack.empty()) {
      auto [groupBegin, groupEnd] = groupingStack.top();
      groupingStack.pop();
      groupBegin->clear();
      groupEnd->clear();
      if (groupBegin + 1 >= groupEnd) // group is empty
        throw std::invalid_argument("empty grouping");
      orderExpression(groupBegin + 1, groupEnd); // begin iterator points to first element of grouping, end iterator to group closing
    }
  }

  OpDoubleVector::iterator getPreviousCharacter(OpDoubleVector::iterator it, OpDoubleVector::iterator begin) {
    do {
      if (it == begin)
        throw std::invalid_argument("Expression/grouping starts with an operator");
      --it;
    } while (it->empty());
    return it;
  }

  OpDoubleVector::iterator getNextCharacter(OpDoubleVector::iterator it, OpDoubleVector::iterator end) {
    do {
      if (it + 1 == end)
        throw std::invalid_argument("Expression/grouping ends with an operator");
      ++it;
      if (it->size() == 1 && isBinaryOperation(it->at(0).getType()))
        throw std::invalid_argument("two consecutive binary operations");
    } while (it->empty());
    return it;
  }


  void mergeBinaryOperators(const OpDoubleVector::iterator begin, const OpDoubleVector::iterator end, bool (*typeCheck)(const OperatorType)) {
    for (auto it = begin; it != end;) { // iterator incrementation is done in the body
      if (it->size() == 1 && typeCheck(it->at(0).getType())) {
        auto op = it;
        auto argLeft = getPreviousCharacter(it, begin);
        auto argRight = getNextCharacter(it, end);
        argLeft->insert(argLeft->end(), argRight->begin(), argRight->end()); // append right argument
        argLeft->insert(argLeft->end(), op->begin(), op->end()); // append operator at the end
        op->clear(); // delete operator (now superfluous)
        argRight->clear(); // delete right argument (now superfluous)
        it = argRight + 1;
      }
      else
        ++it;
    }
  }

  void mergeAlternations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    mergeBinaryOperators(begin, end, &isAlternation);
  }

  void mergeConcatenations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    mergeBinaryOperators(begin, end, &isConcatenation);
  }

  namespace {

    bool isElementRepetition(OpDoubleVector::iterator it) {
      return it->size() == 1 && isRepition(it->at(0).getType());
    }

    bool isFirstElementRepetition(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
      return begin != end && isElementRepetition(begin);
    }

    void mergeElements(std::vector<OpDoubleVector::iterator> elements) {
      if (elements.size() < 2)
        throw std::invalid_argument("At least two elements are required to merge");

      std::vector<Operator>& elementToFill = *elements.front();

      for (auto it = elements.begin() + 1; it != elements.end(); ++it) {
        std::vector<Operator>& elementToEmpty = **it;
        elementToFill.insert(elementToFill.end(), elementToEmpty.begin(), elementToEmpty.end());
        elementToEmpty.clear();
      }
    }
  }

  void mergeRepetitions(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    if (isFirstElementRepetition(begin, end))
      throw std::invalid_argument("Expression/grouping starts with a repetition operator");

    for (auto it = begin; it != end; ++it) {
      if (isElementRepetition(it)) {
        auto operation = it;
        auto argument = getPreviousCharacter(it, begin);
        mergeElements({ argument, operation }); // merge the repetition operator with its argument
      }
    }
  }

  /* At the start of the function, the OpDoubleVector input contains one element in each of its vectors.
  * As the inner functions are called, elements are merged so that some vectors contain no element, while others contain multiple.
  * Each such multiple-element-vector contains a complete operation with an operator and all its arguments, e.g. "ab|" or "a+".
  * These are merged with others until all vectors except one are empty.
  */
  void orderExpression(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    mergeGroupings(begin, end);
    mergeRepetitions(begin, end);
    mergeConcatenations(begin, end);
    mergeAlternations(begin, end);
  }

  OpVector buildExpressionArgumentsFirstOperatorLast(std::string_view searchString) {
    const auto replacedCharacters = convertStringToOpVector(searchString);
    const auto addedConcatenation = addConcatenationOperators(replacedCharacters);
    auto result = convertToVectorExpression(addedConcatenation);
    orderExpression(result.begin(), result.end());

    OpVector characters;
    for (const auto& ch : result)
      characters.insert(characters.end(), ch.begin(), ch.end());

    return characters;
  }

}
