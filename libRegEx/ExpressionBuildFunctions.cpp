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
      } while (it->empty());
      return it;
    }


    void mergeOperatorsWithTwoArguments(const OpDoubleVector::iterator begin, const OpDoubleVector::iterator end, bool (*typeCheck)(const OperatorType)) {
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
      mergeOperatorsWithTwoArguments(begin, end, &isAlternation);
    }

    void mergeConcatenations(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
      mergeOperatorsWithTwoArguments(begin, end, &isConcatenation);
    }

    void mergeRepetitions(OpDoubleVector::iterator begin, OpDoubleVector::iterator end) {
      for (auto it = begin; it != end; ++it) {
        if (it->size() == 1 && isRepition(it->at(0).getType())) {
          auto op = it;
          auto arg = getPreviousCharacter(it, begin);
          arg->insert(arg->end(), op->begin(), op->end()); // append operator to argument
          op->clear(); // delete operator (now superfluous)
        }
      }
    }

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
