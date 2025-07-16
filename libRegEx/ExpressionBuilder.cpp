#include "ExpressionBuilder.h"

namespace regex {

  ExpressionBuilder::ExpressionBuilder(VectorChar(*buildFunction)(std::string_view))
  : buildFunction(buildFunction) {
  }

  Expression ExpressionBuilder::build(std::string_view searchString) {
    VectorChar characters = buildFunction(searchString);
    return Expression(std::move(characters));
  }
}
