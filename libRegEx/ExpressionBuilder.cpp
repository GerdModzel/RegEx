#include "ExpressionBuilder.h"

namespace regex {

  ExpressionBuilder::ExpressionBuilder(OpVector(*buildFunction)(std::string_view))
  : buildFunction(buildFunction) {
  }

  Expression ExpressionBuilder::build(std::string_view searchString) {
    OpVector characters = buildFunction(searchString);
    return Expression(std::move(characters));
  }
}
