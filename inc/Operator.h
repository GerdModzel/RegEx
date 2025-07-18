#pragma once

#include "OperatorType.h"

#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace regex {

  class Operator;
  using OpVector = std::vector<Operator>;
  using OpDoubleVector = std::vector<OpVector>;


  class Operator {
  public:
    explicit Operator(char i_value)
    : type(OperatorType::Literal)
    , value(i_value)
    {}
    explicit Operator(OperatorType i_type)
    : type(i_type)
    , value(std::nullopt)
    {}
    OperatorType const getType() const {
      return type;
    }
    std::optional<char> const getValue() const {
      return value;
    }
  private:
    OperatorType type;
    std::optional<char> value;
  };


  inline bool operator==(const Operator& lhs, const Operator& rhs) {
    return lhs.getType() == rhs.getType() && lhs.getValue() == rhs.getValue();
  }

  inline std::ostream& operator<<(std::ostream& os, const Operator& character) {
    if (character.getValue().has_value()) {
      os << "Character(" << static_cast<int>(character.getType()) << ", '" << character.getValue().value() << "')";
    } else {
      os << "Character(" << static_cast<int>(character.getType()) << ")";
    }
    return os;
  }

   inline char convertOperatorToChar(const Operator op) {
     switch (op.getType()) {
     case OperatorType::Literal: return *op.getValue();
     case OperatorType::Wildcard: return '.';
     case OperatorType::OneOrMore: return '+';
     case OperatorType::ZeroOrMore: return '*';
     case OperatorType::ZeroOrOne: return '?';
     case OperatorType::Concatenation: return '&'; // Not explicitly represented; currently not distinguishable from real '&' character
     case OperatorType::Alternation: return '|';
     case OperatorType::GroupingStart: return '(';
     case OperatorType::GroupingEnd: return ')';
     default: return '\0'; // Not an operator
     }
   }

   inline std::string convertOpVectorToString(const OpVector& operators) {
     std::string result;
     for (const auto& op : operators) {
       result += convertOperatorToChar(op);
     }
     return result;
   }

   inline Operator convertCharToOperator(char ch) {
     switch (ch) {
     case '.': return Operator{ OperatorType::Wildcard };
     case '+': return Operator{ OperatorType::OneOrMore };
     case '*': return Operator{ OperatorType::ZeroOrMore };
     case '?': return Operator{ OperatorType::ZeroOrOne };
             // there is no explicit representation of concatenation in the expression
     case '|': return Operator{ OperatorType::Alternation };
     case '(': return Operator{ OperatorType::GroupingStart };
     case ')': return Operator{ OperatorType::GroupingEnd };
     default: return Operator{ ch }; // Default to literal for any other character
     }
   }

   inline OpVector convertStringToOpVector(std::string_view searchString) {
     OpVector result;
     for (const auto& ch : searchString)
       result.push_back(convertCharToOperator(ch));
     return result;
   }


}
