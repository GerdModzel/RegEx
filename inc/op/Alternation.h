#pragma once

#include "Operator.h"
#include "OperatorVisitor.h"

namespace regex::op {

  class Alternation : public Operator {
  public:
    Alternation() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<Alternation>(*this);
    }

    char toChar() const override { return '|'; }
    bool isRepetition() const override { return false; }
    bool isBinaryOperation() const { return true; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

}
