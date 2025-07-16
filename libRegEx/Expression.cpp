#include "Expression.h"

#include <stack>
#include <cassert>
#include <algorithm>

namespace regex {

   using VectorChar = std::vector<Character>;
    using VectorExpr = std::vector<VectorChar>;

  Expression::Expression(VectorChar&& characters)
    : characters(std::move(characters)) {
 }

}
