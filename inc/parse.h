#pragma once

#include "Result.h"

#include <string_view>

namespace regex {

  ParseResult parse(std::string_view text);

}
