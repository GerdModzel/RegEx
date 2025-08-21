#pragma once

#include "OperatorType.h"

#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace regex {

  class Operator;
  using OpVector = std::vector<std::unique_ptr<Operator>>;
  using OpDoubleVector = std::vector<OpVector>;


  class Operator {
  protected:
    explicit Operator(char i_value)
    : type(OperatorType::Literal)
    , value(i_value)
    {}
    explicit Operator(OperatorType i_type)
    : type(i_type)
    , value(std::nullopt)
    {}
  public:
    OperatorType const getType() const {
      return type;
    }
    std::optional<char> const getValue() const {
      return value;
    }
  protected:
    OperatorType type;
    std::optional<char> value;
  };

  class Wildcard : public Operator {
  public:
    Wildcard()
      : Operator(OperatorType::Wildcard) {
    }
  };

  class Alternation : public Operator {
  public:
    Alternation()
      : Operator(OperatorType::Alternation) {
    }
  };

  class GroupingStart : public Operator {
  public:
    GroupingStart()
      : Operator(OperatorType::GroupingStart) {
    }
  };

  class GroupingEnd : public Operator {
  public:
    GroupingEnd()
      : Operator(OperatorType::GroupingEnd) {
    }
  };

  class ZeroOrMore : public Operator {
  public:
    ZeroOrMore()
      : Operator(OperatorType::ZeroOrMore) {
    }
  };

  class OneOrMore : public Operator {
  public:
    OneOrMore()
      : Operator(OperatorType::OneOrMore) {
    }
  };

  class ZeroOrOne : public Operator {
  public:
    ZeroOrOne()
      : Operator(OperatorType::ZeroOrOne) {
    }
  };

  class Concatenation : public Operator {
  public:
    Concatenation()
      : Operator(OperatorType::Concatenation) {
    }
  };

  class Literal : public Operator {
  public:
    explicit Literal(char i_value)
      : Operator(i_value) {
    }
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
       result += convertOperatorToChar(*op);
     }
     return result;
   }


   inline std::unique_ptr<Operator> convertCharToOperator(char ch) {
     std::unique_ptr<Operator> result{ nullptr };
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

   inline OpVector convertStringToOpVector(std::string_view searchString) {
     OpVector result;
     for (const auto& ch : searchString)
       result.push_back(convertCharToOperator(ch));
     return result;
   }

   inline OpVector createOpVector(std::vector<Operator> input) {
      OpVector result;
      for (auto& op : input) {
        result.push_back(std::make_unique<Operator>(std::move(op)));
      }
      return result;
   }


}
