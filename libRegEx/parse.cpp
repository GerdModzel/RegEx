#include "parse.h"

#include <iostream>

#include "Result.h"

namespace regex {

  ParseResult parse(std::string_view text) {
    std::cout << text << std::endl;
    return ParseResult{};
  }

}
