#include "ExpressionBuildFunctions.h"

#include <cassert>
#include <stack>
#include <vector>

namespace regex {

  OpVector addConcatenationOperators(const OpVector& input) {
    OpVector output;
    for (auto it = input.begin(); it != input.end(); ++it) {
      if ((**it).getType() == OperatorType::Concatenation)
        throw std::invalid_argument("there should be no explicit concatenation operator in the input");

      output.push_back(std::make_unique<Operator>(**it));

      if (
           (**it).getType() != OperatorType::Alternation               // do not add concatenation after an alternation (e.g. "a|b")
        && (**it).getType() != OperatorType::GroupingStart             // do not add concatenation after grouping start (e.g. "(a|b)" )
        && std::next(it) != input.end()                             // do not add concatenation at the end of the input
        && (**std::next(it)).getType() != OperatorType::GroupingEnd    // do not add concatenation before grouping end (e.g. "(a|b)")
        && !isOperation((**std::next(it)).getType())) {                // do not add concatenation before an operator (e.g. "a+")
        output.push_back(std::make_unique<Concatenation>());
      }
    }
    return output;
  }

  OpDoubleVector convertToVectorExpression(OpVector& arg) {
    OpDoubleVector result;
    for (auto& el : arg) {
      result.push_back({});
      result.back().push_back(std::move(el));
    }
    return result;
  }

  using GroupingIterators = std::pair<OpDoubleVector::iterator, OpDoubleVector::iterator>;

  /* The groupingStartStack is used to keep track of the opening parentheses. When a closing parenthesis is found,
  *  the opening parenthesis is removed from the stack. We only care for the outermost groupings, so only if that
  *  empties the stack do we add the position of both parentheses to the groupingStack.
  */
  std::stack<GroupingIterators> findOuterGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    Operator groupingStart{ GroupingStart{} };
    Operator groupingEnd{ GroupingEnd{} };
    std::stack<OpDoubleVector::iterator> groupingStartStack;
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
  void mergeGroupings(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    auto groupingStack = findOuterGroupings(begin, end);
    while (!groupingStack.empty()) {
      auto [groupBegin, groupEnd] = popGroupingFromStack(groupingStack);
      if (isGroupEmpty({ groupBegin, groupEnd }))
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
      if (it->size() == 1 && isBinaryOperation(it->at(0)->getType()))
        throw std::invalid_argument("two consecutive binary operations");
    } while (it->empty());
    return it;
  }

  namespace {
    void mergeElements(std::vector<OpDoubleVector::iterator> elements) {
      if (elements.size() < 2)
        throw std::invalid_argument("At least two elements are required to merge");

      OpVector& elementToFill = *elements.front();

      for (auto it = elements.begin() + 1; it != elements.end(); ++it) {
        OpVector& elementToEmpty = **it;
        elementToFill.insert(elementToFill.end(), std::make_move_iterator(elementToEmpty.begin()), std::make_move_iterator(elementToEmpty.end()));
        elementToEmpty.clear();
      }
    }
  }

  void mergeBinaryOperators(const OpDoubleVector::iterator begin, const OpDoubleVector::iterator end, bool (*typeCheck)(const OperatorType)) {
    for (auto it = begin; it != end;) { // iterator incrementation is done in the body
      OpVector& currentElement = *it;
      if (currentElement.size() == 1 && typeCheck(currentElement[0]->getType())) {
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

  void mergeAlternations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    mergeBinaryOperators(begin, end, &isAlternation);
  }

  void mergeConcatenations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
    mergeBinaryOperators(begin, end, &isConcatenation);
  }

  namespace {
    bool isElementRepetition(OpDoubleVector::iterator it) {
      return it->size() == 1 && isRepition(it->at(0)->getType());
    }

    bool isFirstElementRepetition(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
      return begin != end && isElementRepetition(begin);
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
    auto addedConcatenation = addConcatenationOperators(replacedCharacters);
    auto result = convertToVectorExpression(addedConcatenation);
    orderExpression(result.begin(), result.end());

    OpVector characters;
    for (auto& ch : result)
      characters.insert(characters.end(), std::make_move_iterator(ch.begin()), std::make_move_iterator(ch.end()));

    return characters;
  }

}
