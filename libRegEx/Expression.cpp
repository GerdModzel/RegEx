#include "Expression.h"

#include <stack>
#include <cassert>
#include <algorithm>

namespace regex {

  Expression::Expression(op::Vector&& characters)
    : characters{ std::move(characters) } {
  }

  Expression::Expression(const op::Vector& i_characters)
    : characters{} {
    for (const auto& ch : i_characters)
      characters.push_back(ch->clone());
  }

}
