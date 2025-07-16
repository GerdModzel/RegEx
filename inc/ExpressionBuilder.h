#pragma once

#include "Expression.h"

namespace regex {

  class ExpressionBuilder
  {
  public:
    ExpressionBuilder(VectorChar(*buildFunction)(std::string_view));
    Expression build(std::string_view);

  private:
    VectorChar(*buildFunction)(std::string_view);
  };

}
