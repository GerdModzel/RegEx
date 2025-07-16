#pragma once

namespace regex {

enum class CharacterType
  {
    Literal           // 'a', 'b', 'c', etc.: matches the literal character
  , Wildcard          // '.': matches any single character
  , OneOrMore         // '+': matches the preceding element one or more times
  , ZeroOrMore        // '*': matches the preceding element zero or more times
  , ZeroOrOne         // '?': matches the preceding element zero or one time
  , Concatenation     // is inferred between items, not explicitly represented: e.g., "ab" is treated as 'a' followed by 'b'
  , Alternation       // '|': matches either the left or right side of the alternation
  , GroupingStart     // '(': starts a group
  , GroupingEnd       // ')': ends a group
  };

  inline bool isOperation(const CharacterType type) {
    return type == CharacterType::Alternation || type == CharacterType::Concatenation || type == CharacterType::OneOrMore
        || type == CharacterType::ZeroOrMore || type == CharacterType::ZeroOrOne;
  }

  inline bool isRepition(const CharacterType type) {
    return type == CharacterType::OneOrMore || type == CharacterType::ZeroOrMore || type == CharacterType::ZeroOrOne;
  }

  inline bool isAlternation(const CharacterType type) {
    return type == CharacterType::Alternation;
  }

  inline bool isConcatenation(const CharacterType type) {
    return type == CharacterType::Concatenation;
  }

  inline bool isMatching(const CharacterType type) {
    return type == CharacterType::Literal || type == CharacterType::Wildcard;
  }

  inline bool isGroupingStart(const CharacterType type) {
    return type == CharacterType::GroupingStart;
  }

  inline bool isGroupingEnd(const CharacterType type) {
    return type == CharacterType::GroupingEnd;
  }

}

