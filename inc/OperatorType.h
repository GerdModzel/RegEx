#pragma once

namespace regex {

enum class OperatorType
  {
    Literal           // 'a', 'b', 'c', etc.: matches the literal character
  , Wildcard          // '.': matches any single character
  , OneOrMore         // '+': matches the preceding element one or more times
  , ZeroOrMore        // '*': matches the preceding element zero or more times
  , ZeroOrOne         // '?': matches the preceding element zero or one time
  , Concatenation     // '&': is inferred between items, not explicitly represented: e.g., "ab" is treated as 'a' followed by 'b'
  , Alternation       // '|': matches either the left or right side of the alternation
  , GroupingStart     // '(': starts a group
  , GroupingEnd       // ')': ends a group
  , Match             // '#': indicates a match operation; not a regex operator
  };

}

