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
    int nArgumentsRequired() const override { return 2; };
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

}
