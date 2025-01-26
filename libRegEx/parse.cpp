#include "parse.h"

#include "NfaState.h"

#include <iostream>
#include <span>

namespace regex {

  std::vector<ParseResult> parse(std::string_view text, std::string_view expr) {
    if (text.empty() || expr.empty())
      return {};

    const auto [nfa, stateManager] = buildNfaList(expr);
    std::vector<ParseResult> results;
    
    for (int i = 0; i < text.size(); ++i) {
      std::string_view subText = text.substr(i, text.size());
      const unsigned size = walkThroughNfa(nfa.startState, subText);
      if (size > 0)
        results.emplace_back(i, size);
    }
    return results;
  }

}
