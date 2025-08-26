#pragma once

#include "Operator.h"
#include "OperatorVisitor.h"

namespace regex::op {


  class Literal : public Operator {
  public:
    explicit Literal(char value)
      : value(value) {
    }
    ~Literal() override = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<Literal>(*this);
    }

    char const getValue() const {
      return value;
    }
    char toChar() const override {
      return getValue();
    }
    bool isRepetition() const override { return false; }
    int nArgumentsRequired() const override { return 0; };
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    } private:
      char value;
  };

}
