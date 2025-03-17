#pragma once

#include "ParseResult.h"
#include "Expression.h"

#include <string_view>
#include <vector>

namespace regex {

  std::vector<ParseResult> parse(std::string_view text, const regex::Expression& expr);
}
