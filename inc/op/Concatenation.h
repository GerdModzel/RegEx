#pragma once

#include "Operator.h"
#include "OperatorVisitor.h"

namespace regex::op {

  class Concatenation : public Operator {
  public:
    Concatenation() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<Concatenation>(*this);
    }

    char toChar() const override { return '&'; }
    bool isRepetition() const override { return false; }
    int nArgumentsRequired() const override { return 2; };
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

}
