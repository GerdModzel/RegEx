#pragma once

#include <stddef.h>
#include <ostream>

namespace regex {

  class ParseResult {
  public:
    ParseResult(const size_t pos, const size_t size)
    : _position(pos)
    , _size(size) {
    }

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

  inline std::ostream& operator<<(std::ostream& os, const ParseResult& result) {
    os << "ParseResult(position: " << result.position() << ", size: " << result.size() << ")";
    return os;
  }

}
