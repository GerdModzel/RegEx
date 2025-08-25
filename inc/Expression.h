#pragma once

#include "op/Operator.h"
#include "op/Types.h"
#include "op/Utils.h"

#include <vector>
#include <string>

namespace regex {

 /// Represents a regular expression as a sequence of operators.
 class Expression {
  public:
    Expression(op::Vector&& characters);
    Expression(const op::Vector& i_characters);

    op::Vector::const_iterator cbegin() const {
      return characters.cbegin();
    }
     op::Vector::const_iterator cend() const {
      return characters.cend();
    }
    size_t size() const {
      return characters.size();
    }
    bool empty() const {
      return characters.empty();
    }
    const std::unique_ptr<regex::op::Operator>& operator[](const size_t index) const {
      return characters[index];
    }
    /// Converts the expression to a string representation. Concatenation is represented as '&'.
    std::string toString() const {
      return regex::op::convertOpVectorToString(characters);
   }
  private:
    op::Vector characters;
  };
}