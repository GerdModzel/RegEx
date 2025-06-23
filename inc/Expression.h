#pragma once

#include <string_view>
#include <vector>
#include <optional>

namespace regex {

  enum class CharacterType
  { Literal
  , Wildcard
  , OneOrMore
  , ZeroOrMore
  , ZeroOrOne
  , AnyCharacter
  , Concatenation
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