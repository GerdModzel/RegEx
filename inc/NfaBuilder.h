#pragma once

#include <string_view>
#include <stack>

#include "NfaState.h"
#include "Expression.h"
#include "NfaFragment.h"

namespace regex {

  NfaFragment createNfaFragment(const regex::Expression& expr);

  using FragmentStack = std::stack<NfaFragment>;

}



