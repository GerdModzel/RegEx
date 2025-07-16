#include "ExpressionBuildFunctions.h"

#include <cassert>
#include <stack>
#include <vector>

namespace regex {

   VectorChar addConcatenationOperators(const VectorChar& input) {
      VectorChar output;
      for (auto it = input.begin(); it != input.end(); ++it) {
        if (it->getType() == CharacterType::Concatenation)
          throw std::runtime_error("Unexpected concatenation operator in input");

        output.push_back(*it);

        if (
          it->getType() != CharacterType::Alternation
       && it->getType() != CharacterType::GroupingStart
       && std::next(it) != input.end()
       && std::next(it)->getType() != CharacterType::GroupingEnd
       && !isOperation(std::next(it)->getType())) {
          output.emplace_back(CharacterType::Concatenation);
        }
      }
      return output;
    }

     VectorExpr convertToVectorExpression(const VectorChar& arg) {
      VectorExpr result;
      for (const auto& el : arg)
        result.emplace_back(1, el);
      return result;
    }

    std::stack<std::pair<VectorExpr::iterator, VectorExpr::iterator>> findOuterGroupings(VectorExpr::iterator begin, VectorExpr::iterator end) {
      VectorChar groupingStart{ 1, Character{CharacterType::GroupingStart} };
      VectorChar groupingEnd{ 1, Character{CharacterType::GroupingEnd} };
      std::stack<VectorExpr::iterator> groupingStartStack;
      std::stack<std::pair<VectorExpr::iterator, VectorExpr::iterator>> groupingStack;
      for (auto it = begin; it != end; ++it) {
        if (*it == groupingStart) {
          groupingStartStack.push(it);
        }
        else if (*it == groupingEnd) {
          if (groupingStartStack.empty())
            throw std::runtime_error("Unmatched grouping end");
          auto start = groupingStartStack.top();
          groupingStartStack.pop();
          if (groupingStartStack.empty())
            groupingStack.push({ start, it });
        }
      }
      if (!groupingStartStack.empty())
        throw std::runtime_error("Unmatched grouping start");
      return groupingStack;
    }

    void mergeGroupings(VectorExpr::iterator begin, VectorExpr::iterator end) {
      auto groupingStack = findOuterGroupings(begin, end);
      while (!groupingStack.empty()) {
        auto [groupBegin, groupEnd] = groupingStack.top();
        groupingStack.pop();
        groupBegin->clear();
        groupEnd->clear();
        if (groupBegin + 1 < groupEnd) // check that the group is not empty
          orderExpression(groupBegin + 1, groupEnd); // begin iterator points to first element, end iterator to group closing
     }
   }

    VectorExpr::iterator getPreviousCharacter(VectorExpr::iterator it, VectorExpr::iterator begin) {
      do {
        if (it == begin)
          throw std::runtime_error("Expression/grouping starts with an operator");
        --it;
      } while (it->empty());
      return it;
    }

    VectorExpr::iterator getNextCharacter(VectorExpr::iterator it, VectorExpr::iterator end) {
      do {
        if (it + 1 == end)
          throw std::runtime_error("Expression/grouping ends with an operator");
        ++it;
      } while (it->empty());
      return it;
    }


    void mergeOperatorsWithTwoArguments(const VectorExpr::iterator begin, const VectorExpr::iterator end, bool (*typeCheck)(const CharacterType)) {
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

    void mergeAlternations(VectorExpr::iterator begin, VectorExpr::iterator end) {
      mergeOperatorsWithTwoArguments(begin, end, &isAlternation);
    }

    void mergeConcatenations(VectorExpr::iterator begin, VectorExpr::iterator end) {
      mergeOperatorsWithTwoArguments(begin, end, &isConcatenation);
    }

    void mergeRepetitions(VectorExpr::iterator begin, VectorExpr::iterator end) {
      for (auto it = begin; it != end; ++it) {
        if (it->size() == 1 && isRepition(it->at(0).getType())) {
          auto op = it;
          auto arg = getPreviousCharacter(it, begin);
          arg->insert(arg->end(), op->begin(), op->end()); // append operator to argument
          op->clear(); // delete operator (now superfluous)
        }
      }
    }

    void orderExpression(VectorExpr::iterator begin, VectorExpr::iterator end) {
      mergeGroupings(begin, end);
      mergeRepetitions(begin, end);
      mergeConcatenations(begin, end);
      mergeAlternations(begin, end);
    }


  VectorChar buildExpressionArgumentsFirstOperatorLast(std::string_view searchString) {
    const auto replacedCharacters = convertStringToOperatorVector(searchString);
    const auto addedConcatenation = addConcatenationOperators(replacedCharacters);
    auto result = convertToVectorExpression(addedConcatenation);
    orderExpression(result.begin(), result.end());
  
    VectorChar characters;
    for (const auto& ch : result)
      characters.insert(characters.end(), ch.begin(), ch.end());

    return characters;
  }

}
