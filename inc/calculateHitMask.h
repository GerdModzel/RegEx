#pragma once

#include "ParseResult.h"

#include <vector>
#include <ostream>

namespace regex {

  using HitMask = std::vector<unsigned char>;

  HitMask calculateHitMask(const std::vector<SearchResult>& parseResults, const size_t textLength);

}

