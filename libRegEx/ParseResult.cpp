#include "ParseResult.h"

namespace regex {

  ParseResult::ParseResult(const int pos, const int size)
    : _position(pos)
    , _size(size) {
  }
}