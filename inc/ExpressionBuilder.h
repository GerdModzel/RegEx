#pragma once

#include "Expression.h"

namespace regex {

  /**
   * \brief Creates an Expression from a string using buildFunction. The Expression is designed to be further processed, e.g. into an NFA.
   */
  class ExpressionBuilder
  {
  public:
    ExpressionBuilder(OpVector(*buildFunction)(std::string_view));
    Expression build(std::string_view);

  private:
    OpVector(*buildFunction)(std::string_view);
  };

}
