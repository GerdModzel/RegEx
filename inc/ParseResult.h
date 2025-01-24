#pragma once

#include <stddef.h>

namespace regex {

  class ParseResult {
  public:
    ParseResult();

    inline size_t position() const {
      return pos;
    }

    inline size_t size() const {
      return length;
    }

  private:
    size_t pos;
    size_t length;
  };

}
