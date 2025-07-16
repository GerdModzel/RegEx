#pragma once

#include "Expression.h"

namespace regex {

  class ExpressionBuilder
  {
  public:
    ExpressionBuilder(OpVector(*buildFunction)(std::string_view));
    Expression build(std::string_view);

  private:
    OpVector(*buildFunction)(std::string_view);
  };

}
