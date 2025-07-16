#pragma once

#include "Expression.h"
#include "ExpressionBuilder.h"
#include "ExpressionBuildFunctions.h"

namespace regex {

  Expression buildNfaExpression(std::string_view searchString) {
    ExpressionBuilder builder(&buildExpressionArgumentsFirstOperatorLast);
    return builder.build(searchString);
  }


}
