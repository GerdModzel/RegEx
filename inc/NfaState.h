#pragma once

#include <string_view>
#include <stack>
#include <optional>
#include <memory>
#include <vector>
#include <cassert>
#include <algorithm>

#include <ParseResult.h>

namespace regex {

  struct NfaState
  {
    enum class Type {ch, split, match};

    Type type;
    // std::nullopt: type != ch or character is wildcard character and matches any character
    std::optional<char> ch;
    std::vector<NfaState*> nextStates;
    int lastList;
  };

  struct NfaFragment {
     NfaState* startState;
    std::vector<NfaState**> nextStates;
  };

}