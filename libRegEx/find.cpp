#include "find.h"

#include "nfa/MainBuilder.h"
#include "ExpressionBuilder.h"
#include "ExpressionBuildFunctions.h"
#include "nfa/FragmentSearch.h"

#include <iostream>
#include <span>
#include <future>

namespace regex {

  std::vector<SearchResult> search(std::string_view text, std::string_view searchString) {
    if (text.empty() || searchString.empty())
      throw std::invalid_argument("text or search string cannot be empty");

    ExpressionBuilder exprBuilder(&buildExpressionArgumentsFirstOperatorLast);
    Expression expr = exprBuilder.build(searchString);

    nfa::MainBuilder nfaBuilder;
    nfa::Complete nfa = nfaBuilder.createNfaFragment(expr);

    return executeSearch(text, &nfa);
  }

}
