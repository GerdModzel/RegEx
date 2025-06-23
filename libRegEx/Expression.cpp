#include "Expression.h"

namespace regex {

  Expression::Expression(std::string_view searchString) {
    bool firstCh = true;
    for (auto ch : searchString) {
      if (ch == '.') {
        ;
        // Handle special characters if needed
      }
      else {
        characters.emplace_back(ch);
        if (!firstCh) {
          characters.emplace_back(CharacterType::Concatenation);
        }
        firstCh = false;
      }
    }
  }

}