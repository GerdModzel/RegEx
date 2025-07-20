#pragma once

#include "ParseResult.h"

#include <vector>
#include <string_view>

 namespace regex {

  class NfaFragment;

  std::vector<ParseResult> executeSearch(std::string_view text, NfaFragment* fragment);

}

