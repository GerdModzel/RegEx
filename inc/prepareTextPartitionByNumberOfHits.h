#pragma once

#include "ParseResult.h"

#include <vector>
#include <ostream>

namespace regex {

  struct TextPartitionInfo {
    size_t partPosition;
    size_t partLength;
    int nHits;
  };

inline bool operator==(const TextPartitionInfo& lhs, const TextPartitionInfo& rhs) {
   return lhs.partPosition == rhs.partPosition
       && lhs.partLength == rhs.partLength
       && lhs.nHits == rhs.nHits;
}

inline  std::ostream& operator<<(std::ostream& os, const TextPartitionInfo& info) {
    os << "pos: " << info.partPosition << "| len: " << info.partLength << " | hits: " << info.nHits;
    return os;
  }

  std::vector<TextPartitionInfo> prepareTextPartitionByNumberOfHits(const std::vector<ParseResult>& parseResults, const size_t textLength);

}

