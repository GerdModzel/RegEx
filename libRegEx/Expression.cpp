#include "Expression.h"

#include <stack>
#include <cassert>
#include <algorithm>

namespace regex {

   using OpVector = std::vector<Operator>;
    using OpDoubleVector = std::vector<OpVector>;

  Expression::Expression(OpVector&& characters)
    : characters(std::move(characters)) {
 }

}
