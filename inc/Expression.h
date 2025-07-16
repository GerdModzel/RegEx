#pragma once

#include "Operator.h"

#include <vector>
#include <string>

namespace regex {


 class Expression {
  public:
    Expression(VectorChar&& characters);
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
      return regex::convertOperatorVectorToString(characters);
   }
  private:
    VectorChar characters;
  };
}