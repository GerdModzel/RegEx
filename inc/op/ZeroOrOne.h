#pragma once

#include "Operator.h"
#include "OperatorVisitor.h"

namespace regex::op {


  class ZeroOrOne : public Operator {
  public:
    ZeroOrOne() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<ZeroOrOne>(*this);
    }

    char toChar() const override { return '?'; }
    bool isRepetition() const override { return true; }
    int nArgumentsRequired() const override { return 1; };
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

}
