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
  };

  struct Character {
  public:
    explicit Character(char i_value)
    : type(CharacterType::Literal)
    , value(i_value)
    {}
    explicit Character(CharacterType i_type)
    : type(i_type)
    , value(std::nullopt)
    {}
    CharacterType const type;
    std::optional<char> const value;
  };

  inline bool operator==(const Character& lhs, const Character& rhs) {
    return lhs.type == rhs.type && (!lhs.value.has_value() && !rhs.value.has_value() || lhs.value == rhs.value);
  }

  inline std::ostream& operator<<(std::ostream& os, const Character& character) {
    if (character.value.has_value()) {
      os << "Character(" << static_cast<int>(character.type) << ", '" << character.value.value() << "')";
    } else {
      os << "Character(" << static_cast<int>(character.type) << ")";
    }
    return os;
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
  private:
    std::vector<regex::Character> characters;
  };
}