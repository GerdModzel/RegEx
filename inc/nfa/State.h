#pragma once

#include <optional>
#include <vector>

namespace regex::nfa {

  struct State
  {
    enum class Type {ch, split, match};

    State(Type type, std::optional<char> ch, std::vector<State*> nextStates);
    Type type;
    // std::nullopt: type != ch or character is wildcard character and matches any character
    std::optional<char> ch;
    std::vector<State*> nextStates;
    int lastList;
  };

}