#pragma once

#include <stddef.h>

namespace regex {

  class ParseResult {
  public:
    ParseResult(const int pos, const int length);

    inline size_t position() const {
      return _position;
    }

    inline size_t size() const {
      return _size;
    }

  private:
    size_t _position;
    size_t _size;
  };

  inline bool operator==(const ParseResult& lhs, const ParseResult& rhs) {
    return lhs.position() == rhs.position() && lhs.size() == rhs.size();
  }

}
