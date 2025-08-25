#pragma once

#include <vector>
#include <memory>

namespace regex::op {

  class Operator;
  using Vector = std::vector<std::unique_ptr<Operator>>;
  using DoubleVector = std::vector<Vector>;

}
