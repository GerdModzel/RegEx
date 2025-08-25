#include "op/Operator.h"

#include "op/Literal.h"

#include <typeinfo>
#include <ostream>

namespace regex::op {

  bool operator==(const Operator& lhs, const Operator& rhs) {
    auto plhs = dynamic_cast<const Literal*>(&lhs);
    auto prhs = dynamic_cast<const Literal*>(&rhs);
    if (plhs && prhs)
      return plhs->getValue() == prhs->getValue();
    return typeid(lhs) == typeid(rhs);
  }

  std::ostream& operator<<(std::ostream& os, const Operator& character) {
    os << character.toChar();
    return os;
  }

}
