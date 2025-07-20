#pragma once

#include <optional>
#include <vector>

namespace regex {

  struct NfaState
  {
    enum class Type {ch, split, match};

    NfaState(Type type, std::optional<char> ch, std::vector<NfaState*> nextStates);
    Type type;
    // std::nullopt: type != ch or character is wildcard character and matches any character
    std::optional<char> ch;
    std::vector<NfaState*> nextStates;
    int lastList;
  };

}