#include "op/Utils.h"

#include "op/AllOperatorIncludes.h"

namespace regex::op {


  std::string convertOpVectorToString(const op::Vector& operators) {
    std::string result;
    for (const auto& op : operators) {
      result += op->toChar();
    }
    return result;
  }


  std::unique_ptr<Operator> convertCharToOperator(char ch) {
    switch (ch) {
    case '.': return std::make_unique<Wildcard>();
    case '+': return std::make_unique<OneOrMore>();
    case '*': return std::make_unique<ZeroOrMore>();
    case '?': return std::make_unique<ZeroOrOne>();
      // there is no explicit representation of concatenation in the expression
    case '|': return std::make_unique<Alternation>();
    case '(': return std::make_unique<GroupingStart>();
    case ')': return std::make_unique<GroupingEnd>();
    default: return std::make_unique<Literal>(ch); // Default to literal for any other character
    }
  }

  op::Vector convertStringToOpVector(std::string_view searchString) {
    op::Vector result;
    for (const auto& ch : searchString)
      result.push_back(convertCharToOperator(ch));
    return result;
  }


}
