#pragma once

#include "Operator.h"

#include <vector>
#include <string>

namespace regex {


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
    std::string toString() const {
      return regex::convertOpVectorToString(characters);
   }
  private:
    OpVector characters;
  };
}