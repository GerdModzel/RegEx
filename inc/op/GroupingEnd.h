#pragma once

#include "Operator.h"
#include "OperatorVisitor.h"

namespace regex::op {


  class GroupingEnd : public Operator {
  public:
    GroupingEnd() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<GroupingEnd>(*this);
    }

    char toChar() const override { return ')'; }
    bool isRepetition() const override { return false; }
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

}
