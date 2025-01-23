#pragma once

#include "ParseResult.h"

#include <string_view>

namespace regex {

  ParseResult parse(std::string_view test, std::string_view expr);

}
