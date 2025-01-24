#include "parse.h"

#include <iostream>
#include <span>

namespace regex {

  std::vector<ParseResult> parse(std::string_view text, std::string_view expr) {
    std::cout << expr << std::endl;
    return {};
  }

}
