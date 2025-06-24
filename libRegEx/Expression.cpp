#include "Expression.h"

namespace regex {

  Expression::Expression(std::string_view searchString) {
    bool firstCh = true;
    for (auto ch : searchString) {
      switch (ch) {
      default: {
        characters.emplace_back(ch);
        if (!firstCh) {
          characters.emplace_back(CharacterType::Concatenation);
        }
        firstCh = false;
      }
      }
    }
  }

}