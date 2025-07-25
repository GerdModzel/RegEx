#pragma once

#include "Operator.h"

#include <vector>
#include <string>

namespace regex {

 /// Represents a regular expression as a sequence of operators.
 class Expression {
  public:
    Expression(OpVector&& characters);
    std::vector<regex::Operator>::const_iterator cbegin() const {
      return characters.cbegin();
    }
     std::vector<regex::Operator>::const_iterator cend() const {
      return characters.cend();
    }
    size_t size() const {
      return characters.size();
    }
    bool empty() const {
      return characters.empty();
    }
    const regex::Operator& operator[](const size_t index) const {
      return characters[index];
    }
    /// Converts the expression to a string representation. Concatenation is represented as '&'.
    std::string toString() const {
      return regex::convertOpVectorToString(characters);
   }
  private:
    OpVector characters;
  };
}