#include "ExpressionBuilder.h"

namespace regex {

  ExpressionBuilder::ExpressionBuilder(op::Vector(*buildFunction)(std::string_view))
  : buildFunction(buildFunction) {
  }

  Expression ExpressionBuilder::build(std::string_view searchString) {
    op::Vector characters = buildFunction(searchString);
    return Expression(std::move(characters));
  }
}
