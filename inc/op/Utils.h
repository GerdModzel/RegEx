#pragma once

#include "op/Types.h"

#include <string>
#include <memory>

namespace regex::op {

    std::string convertOpVectorToString(const op::Vector& operators);
    std::unique_ptr<Operator> convertCharToOperator(char ch);
    op::Vector convertStringToOpVector(std::string_view searchString);
}

