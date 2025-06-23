#pragma once

#include <string_view>

#include "NfaState.h"
#include "Expression.h"


namespace regex {

class NfaBuilder {
	public:
		NfaBuilder(const regex::Expression& expr);
		NfaFragment build();
	private:
		Expression expr;
		std::vector<std::unique_ptr<NfaState>> stateManager;
	};

}