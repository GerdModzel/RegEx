#pragma once

#include "ParseResult.h"

#include <string_view>
#include <vector>

namespace regex {

  std::vector<ParseResult> parse(std::string_view test, std::string_view expr);
}
