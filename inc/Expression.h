#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <optional>
#include <iostream>

namespace regex {

  enum class CharacterType
  {
    Literal           // 'a', 'b', 'c', etc.: matches the literal character
  , Wildcard          // '.': matches any single character
  , OneOrMore         // '+': matches the preceding element one or more times
  , ZeroOrMore        // '*': matches the preceding element zero or more times
  , ZeroOrOne         // '?': matches the preceding element zero or one time
  , Concatenation     // is inferred between items, not explicitly represented: e.g., "ab" is treated as 'a' followed by 'b'
  , Alternation       // '|': matches either the left or right side of the alternation
  , GroupingStart     // '(': starts a group
  , GroupingEnd       // ')': ends a group
  };

  inline bool isOperation(const CharacterType type) {
    return type == CharacterType::Alternation || type == CharacterType::Concatenation || type == CharacterType::OneOrMore || type == CharacterType::ZeroOrMore || type == CharacterType::ZeroOrOne;
  }

  inline bool isRepition(const CharacterType type) {
    return type == CharacterType::OneOrMore || type == CharacterType::ZeroOrMore || type == CharacterType::ZeroOrOne;
  }

  inline bool isAlternation(const CharacterType type) {
    return type == CharacterType::Alternation;
  }

  inline bool isConcatenation(const CharacterType type) {
    return type == CharacterType::Concatenation;
  }

  inline bool isMatching(const CharacterType type) {
    return type == CharacterType::Literal || type == CharacterType::Wildcard;
  }

  inline bool isGroupingStart(const CharacterType type) {
    return type == CharacterType::GroupingStart;
  }

  inline bool isGroupingEnd(const CharacterType type) {
    return type == CharacterType::GroupingEnd;
  }



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

  inline char characterToChar(const Character& character) {
    switch (character.getType()) {
      case CharacterType::Literal: return *character.getValue();
      case CharacterType::Wildcard: return '.';
      case CharacterType::OneOrMore: return '+';
      case CharacterType::ZeroOrMore: return '*';
      case CharacterType::ZeroOrOne: return '?';
      case CharacterType::Concatenation: return '&'; // Not explicitly represented
      case CharacterType::Alternation: return '|';
      case CharacterType::GroupingStart: return '(';
      case CharacterType::GroupingEnd: return ')';
    }
    return '\0'; // Should not reach here
  }


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

   inline std::string toString(const std::vector<Character>& arg) {
    std::string result;
    for (const auto& ch : arg)
      result += characterToChar(ch);
    return result;
  }

  class Expression {
  public:
    Expression(std::string_view searchString);
    std::vector<regex::Character>::const_iterator cbegin() const {
      return characters.cbegin();
    }
     std::vector<regex::Character>::const_iterator cend() const {
      return characters.cend();
    }
    size_t size() const {
      return characters.size();
    }
    bool empty() const {
      return characters.empty();
    }
    const regex::Character& operator[](const size_t index) const {
      return characters[index];
    }
    std::string toString() const {
      return regex::toString(characters);
   }
  private:
    std::vector<regex::Character> characters;
  };

  using VectorChar = std::vector<Character>;
  using VectorExpr = std::vector<VectorChar>;

  std::vector<Character> replaceCharsWithOperators(std::string_view searchString);
  std::vector<Character> addConcatenationOperators(const std::vector<Character>& input);
  void orderExpression(VectorExpr::iterator begin, VectorExpr::iterator end);

}