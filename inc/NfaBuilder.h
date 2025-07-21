#pragma once

#include <string_view>
#include <stack>

#include "NfaState.h"
#include "Expression.h"
#include "NfaFragment.h"

namespace regex {

  using FragmentStack = std::stack<NfaFragment>;

  class NfaBuilder {
  public:
    static NfaFragment createNfaFragment(const regex::Expression& expr);
    static void addConcatenationFragmentTo(FragmentStack& fragmentStack);
    static void addAlternationFragmentTo(FragmentStack& fragmentStack);
    static void addWildcardFragmentTo(FragmentStack& fragmentStack);
    static void addLiteralFragmentTo(FragmentStack& fragmentStack);
    static void addZeroOrOneFragmentTo(FragmentStack& fragmentStack);
    static void addOneOrMoreFragmentTo(FragmentStack& fragmentStack);
    static void addZeroOrMoreFragmentTo(FragmentStack& fragmentStack);
    static void addSuccessStateTo(FragmentStack& fragmentStack);
  };

}



