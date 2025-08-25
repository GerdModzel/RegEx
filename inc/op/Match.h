#pragma once

#include "Operator.h"
#include "OperatorVisitor.h"

namespace regex::op {


  class Match : public Operator {
  public:
    Match() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<Match>(*this);
    }

    char toChar() const override { return '#'; }
    bool isRepetition() const override { return false; }
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

}
