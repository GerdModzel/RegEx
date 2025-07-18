#pragma once

#include "ParseResult.h"
#include "Expression.h"

#include <string_view>
#include <vector>

namespace regex {

  std::vector<ParseResult> find(std::string_view text, std::string_view searchString);
}
