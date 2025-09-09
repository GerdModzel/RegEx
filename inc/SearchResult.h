#pragma once

#include <stddef.h>
#include <ostream>

namespace regex {

  class SearchResult {
  public:
    SearchResult(const size_t pos, const size_t size)
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

  inline bool operator==(const SearchResult& lhs, const SearchResult& rhs) {
    return lhs.position() == rhs.position() && lhs.size() == rhs.size();
  }

  inline std::ostream& operator<<(std::ostream& os, const SearchResult& result) {
    os << "SearchResult(position: " << result.position() << ", size: " << result.size() << ")";
    return os;
  }

}
