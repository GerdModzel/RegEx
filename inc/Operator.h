#pragma once

#include "OperatorType.h"

#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace regex {

  class Character;
  using VectorChar = std::vector<Character>;
  using VectorExpr = std::vector<VectorChar>;


  class Character {
  public:
    explicit Character(char i_value)
    : type(CharacterType::Literal)
    , value(i_value)
    {}
    explicit Character(CharacterType i_type)
    : type(i_type)
    , value(std::nullopt)
    {}
    CharacterType const getType() const {
      return type;
    }
    std::optional<char> const getValue() const {
      return value;
    }
  private:
    CharacterType type;
    std::optional<char> value;
  };


  inline bool operator==(const Character& lhs, const Character& rhs) {
    return lhs.getType() == rhs.getType() && (!lhs.getValue().has_value() && !rhs.getValue().has_value() || lhs.getValue() == rhs.getValue());
  }

  inline std::ostream& operator<<(std::ostream& os, const Character& character) {
    if (character.getValue().has_value()) {
      os << "Character(" << static_cast<int>(character.getType()) << ", '" << character.getValue().value() << "')";
    } else {
      os << "Character(" << static_cast<int>(character.getType()) << ")";
    }
    return os;
  }

   inline char convertOperatorToChar(const Character op) {
     switch (op.getType()) {
     case CharacterType::Literal: return *op.getValue();
     case CharacterType::Wildcard: return '.';
     case CharacterType::OneOrMore: return '+';
     case CharacterType::ZeroOrMore: return '*';
     case CharacterType::ZeroOrOne: return '?';
     case CharacterType::Concatenation: return '&'; // Not explicitly represented; currently not distinguishable from real '&' character
     case CharacterType::Alternation: return '|';
     case CharacterType::GroupingStart: return '(';
     case CharacterType::GroupingEnd: return ')';
     default: return '\0'; // Not an operator
     }
   }

   inline std::string convertOperatorVectorToString(const VectorChar& operators) {
     std::string result;
     for (const auto& op : operators) {
       result += convertOperatorToChar(op);
     }
     return result;
   }

   inline Character convertCharToOperator(char ch) {
     switch (ch) {
     case '.': return Character{ CharacterType::Wildcard };
     case '+': return Character{ CharacterType::OneOrMore };
     case '*': return Character{ CharacterType::ZeroOrMore };
     case '?': return Character{ CharacterType::ZeroOrOne };
             // there is explicit representation of concatenation in the expression
     case '|': return Character{ CharacterType::Alternation };
     case '(': return Character{ CharacterType::GroupingStart };
     case ')': return Character{ CharacterType::GroupingEnd };
     default: return Character{ ch }; // Default to literal for any other character
     }
   }

   inline VectorChar convertStringToOperatorVector(std::string_view searchString) {
     VectorChar result;
     for (const auto& ch : searchString)
       result.push_back(convertCharToOperator(ch));
     return result;
   }


}
