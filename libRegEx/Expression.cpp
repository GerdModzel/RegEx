#include "Expression.h"

namespace regex {

  Expression::Expression(std::string_view searchString) {
    bool firstCh = true;
    for (auto ch : searchString) {
      switch (ch) {
        case '.': {
        characters.emplace_back(CharacterType::Wildcard);
        if (!firstCh)
          characters.emplace_back(CharacterType::Concatenation);
        firstCh = false;
        break;
        }
        case '+': {
        if (!firstCh)
          characters.emplace_back(CharacterType::OneOrMore);
        break;
        }
      default: {
        characters.emplace_back(ch);
        if (!firstCh)
          characters.emplace_back(CharacterType::Concatenation);
        firstCh = false;
      }
      }
    }
  }

}