#include "calculateHitMask.h"

#include <algorithm>
#include <vector>

namespace regex {

  HitMask calculateHitMask(const std::vector<ParseResult>& parseResults, const size_t textLength) {
    HitMask hitMask(textLength, 0);
    for (const auto& current : parseResults) {

      if (current.position() + current.size() > textLength)
        throw std::out_of_range("ParseResult position or length are out of bounds");

      auto hitStart = hitMask.begin() + current.position();
      auto hitEnd = hitStart + current.size();
      std::transform(hitStart, hitEnd, hitStart, [](const unsigned char el) { return el + 1; });
    }
    return hitMask;
  }

}
