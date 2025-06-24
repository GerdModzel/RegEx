#pragma once

#include "ParseResult.h"

#include <vector>

namespace regex {

  struct TextPartitionInfo {
    size_t textStart;
    size_t textEnd;
    int nHits;
  };

  std::vector<TextPartitionInfo> prepareTextPartitionByNumberOfHits(const std::vector<ParseResult>& parseResults, const size_t textLength);

}

