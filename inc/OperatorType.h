#pragma once

namespace regex {

enum class OperatorType
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

  inline bool isOperation(const OperatorType type) {
    return type == OperatorType::Alternation || type == OperatorType::Concatenation || type == OperatorType::OneOrMore
        || type == OperatorType::ZeroOrMore || type == OperatorType::ZeroOrOne;
  }

  inline bool isRepition(const OperatorType type) {
    return type == OperatorType::OneOrMore || type == OperatorType::ZeroOrMore || type == OperatorType::ZeroOrOne;
  }

  inline bool isAlternation(const OperatorType type) {
    return type == OperatorType::Alternation;
  }

  inline bool isConcatenation(const OperatorType type) {
    return type == OperatorType::Concatenation;
  }

  inline bool isMatching(const OperatorType type) {
    return type == OperatorType::Literal || type == OperatorType::Wildcard;
  }

  inline bool isGroupingStart(const OperatorType type) {
    return type == OperatorType::GroupingStart;
  }

  inline bool isGroupingEnd(const OperatorType type) {
    return type == OperatorType::GroupingEnd;
  }

}

