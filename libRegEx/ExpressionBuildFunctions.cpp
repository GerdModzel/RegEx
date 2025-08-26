#include "ExpressionBuildFunctions.h"

#include "op/Alternation.h"
#include "op/Concatenation.h"
#include "op/GroupingStart.h"
#include "op/GroupingEnd.h"

#include <cassert>
#include <stack>
#include <vector>
#include <stdexcept>

namespace regex {

  op::Vector addConcatenationOperators(const op::Vector& input) {
    op::Vector output;
    for (auto it = input.begin(); it != input.end(); ++it) {
      op::Operator& op = **it;

      if (op == op::Concatenation{})
        throw std::invalid_argument("there should be no explicit concatenation operator in the input");

      output.push_back(op.clone());

      const bool addConcatenation =
        op != op::Alternation{} &&                  // do not add concatenation after an alternation (e.g. "a|b")
        op != op::GroupingStart{} &&                // do not add concatenation after grouping start (e.g. "(a|b)" )
        std::next(it) != input.end() &&             // do not add concatenation at the end of the input 
        **std::next(it) != op::GroupingEnd{} &&     // do not add concatenation before grouping end (e.g. "(a|b)")
        !(**std::next(it)).isRepetition() &&        // do not add concatenation before a repetition operator (e.g. "a+")
        !(**std::next(it)).isBinaryOperation();     // do not add concatenation before a binary operator (e.g. "a|b")

      if (addConcatenation)
        output.push_back(std::make_unique<op::Concatenation>());
    }
    return output;
  }

  op::DoubleVector convertToDoubleVector(const op::Vector& arg) {
    op::DoubleVector result;
    for (auto& el : arg) {
      result.push_back({});
      result.back().push_back(el->clone());
    }
    return result;
  }

  op::Vector convertToSingleVector(const op::DoubleVector& doubleVec) {
    op::Vector result;
    for (const auto& singleVec : doubleVec) {
      for (const auto& element : singleVec)
        result.push_back(element->clone());
    }
    return result;
  }

  using GroupingIterators = std::pair<op::DoubleVector::iterator, op::DoubleVector::iterator>;

  /* The groupingStartStack is used to keep track of the opening parentheses. When a closing parenthesis is found,
  *  the opening parenthesis is removed from the stack. We only care for the outermost groupings, so only if that
  *  empties the stack do we add the position of both parentheses to the groupingStack.
  */
  std::stack<GroupingIterators> findOuterGroupings(op::DoubleVector::iterator begin, op::DoubleVector::iterator end) {
    op::GroupingStart groupingStart;
    op::GroupingEnd groupingEnd;
    std::stack<op::DoubleVector::iterator> groupingStartStack;
    std::stack<GroupingIterators> groupingStack;

    for (auto it = begin; it != end; ++it) {
      if (it->size() == 1 && *it->at(0) == groupingStart) {
        groupingStartStack.push(it);
      }
      else if (it->size() == 1 && *it->at(0) == groupingEnd) {
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

  namespace {
    GroupingIterators popGroupingFromStack(std::stack<GroupingIterators>& stack) {
      auto [groupBegin, groupEnd] = stack.top();
      stack.pop();
      groupBegin->clear();
      groupEnd->clear();
      return { groupBegin, groupEnd };
    }

    bool isGroupEmpty(GroupingIterators group) {
      return group.first + 1 >= group.second;
    }
  }

  /*  Each grouping is ordered separately, which ensures that each grouping will be processed independently during the later search.
   */
  void mergeGroupings(op::DoubleVector::iterator begin, op::DoubleVector::iterator end) {
    auto groupingStack = findOuterGroupings(begin, end);
    while (!groupingStack.empty()) {
      auto [groupBegin, groupEnd] = popGroupingFromStack(groupingStack);
      if (isGroupEmpty({ groupBegin, groupEnd }))
        throw std::invalid_argument("empty grouping");
      orderExpression(groupBegin + 1, groupEnd); // begin iterator points to first element of grouping, end iterator to group closing
    }
  }

  op::DoubleVector::iterator getPreviousCharacter(op::DoubleVector::iterator it, op::DoubleVector::iterator begin) {
    do {
      if (it == begin)
        throw std::invalid_argument("Expression/grouping starts with an operator");
      --it;
    } while (it->empty());
    return it;
  }

  op::DoubleVector::iterator getNextCharacter(op::DoubleVector::iterator it, op::DoubleVector::iterator end) {
    do {
      if (it + 1 == end)
        throw std::invalid_argument("Expression/grouping ends with an operator");
      ++it;
      if (it->size() == 1 && it->at(0)->isBinaryOperation())
        throw std::invalid_argument("two consecutive binary operations");
    } while (it->empty());
    return it;
  }

  namespace {
    void mergeElements(std::vector<op::DoubleVector::iterator> elements) {
      if (elements.size() < 2)
        throw std::invalid_argument("At least two elements are required to merge");

      op::Vector& elementToFill = *elements.front();

      for (auto it = elements.begin() + 1; it != elements.end(); ++it) {
        op::Vector& elementToEmpty = **it;
        elementToFill.insert(elementToFill.end(), std::make_move_iterator(elementToEmpty.begin()), std::make_move_iterator(elementToEmpty.end()));
        elementToEmpty.clear();
      }
    }
  }

  template <class T>
  void mergeBinaryOperators(const op::DoubleVector::iterator begin, const op::DoubleVector::iterator end) {
    for (auto it = begin; it != end;) { // iterator incrementation is done in the body
      op::Vector& currentElement = *it;
      if (currentElement.size() == 1 && *currentElement.at(0) == T{}) {
        auto operation = it;
        auto argLeft = getPreviousCharacter(it, begin);
        auto argRight = getNextCharacter(it, end);
        mergeElements({ argLeft, argRight, operation }); // merge the operator with its two arguments
        it = argRight + 1;
      }
      else
        ++it;
    }
  }

  void mergeAlternations(op::DoubleVector::iterator begin, op::DoubleVector::iterator end) {
    mergeBinaryOperators<op::Alternation>(begin, end);
  }

  void mergeConcatenations(op::DoubleVector::iterator begin, op::DoubleVector::iterator end) {
    mergeBinaryOperators<op::Concatenation>(begin, end);
  }

  namespace {
    bool isElementRepetition(op::DoubleVector::iterator it) {
      return it->size() == 1 && it->at(0)->isRepetition();
    }

    bool isFirstElementRepetition(op::DoubleVector::iterator begin, op::DoubleVector::iterator end) {
      return begin != end && isElementRepetition(begin);
    }

  }

  void mergeRepetitions(op::DoubleVector::iterator begin, op::DoubleVector::iterator end) {
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

  /* At the start of the function, the op::DoubleVector input contains one element in each of its vectors.
  * As the inner functions are called, elements are merged so that some vectors contain no element, while others contain multiple.
  * Each such multiple-element-vector contains a complete operation with an operator and all its arguments, e.g. "ab|" or "a+".
  * These are merged with others until all vectors except one are empty.
  */
  void orderExpression(op::DoubleVector::iterator begin, op::DoubleVector::iterator end) {
    mergeGroupings(begin, end);
    mergeRepetitions(begin, end);
    mergeConcatenations(begin, end);
    mergeAlternations(begin, end);
  }

  op::Vector buildExpressionArgumentsFirstOperatorLast(std::string_view searchString) {
    const auto replacedCharacters = op::convertStringToOpVector(searchString);
    const auto addedConcatenation = addConcatenationOperators(replacedCharacters);

    auto workingData = convertToDoubleVector(addedConcatenation);
    orderExpression(workingData.begin(), workingData.end());
    return convertToSingleVector(workingData);
  }

}
