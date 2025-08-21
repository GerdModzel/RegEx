#include "Expression.h"

#include <stack>
#include <cassert>
#include <algorithm>

namespace regex {

  Expression::Expression(OpVector&& characters)
    : characters{ std::move(characters) } {
  }

  Expression::Expression(const OpVector& i_characters)
    : characters{} {
    for (const auto& ch : i_characters)
      characters.push_back(std::make_unique<Operator>(*ch));
  }

}
