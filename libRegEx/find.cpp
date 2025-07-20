#include "find.h"

#include "NfaState.h"
#include "NfaBuilder.h"
#include "ExpressionBuilder.h"
#include "ExpressionBuildFunctions.h"
#include "NfaFragmentSearch.h"

#include <iostream>
#include <span>
#include <future>

namespace regex {

  std::vector<ParseResult> find(std::string_view text, std::string_view searchString) {
    if (text.empty() || searchString.empty())
      throw std::invalid_argument("text or search string cannot be empty");

    ExpressionBuilder builder(&buildExpressionArgumentsFirstOperatorLast);
    Expression expr = builder.build(searchString);

    NfaBuilder nfaBuilder(expr);
    NfaFragment nfa = nfaBuilder.build();
    std::vector<ParseResult> results;

    return executeSearch(text, &nfa);
  }

}
