#pragma once

#include <memory>
#include <iosfwd>

namespace regex::op {

  class OperatorVisitor;

  class Operator {
  public:
    Operator() = default;
    virtual ~Operator() = default;
    virtual std::unique_ptr<Operator> clone() const = 0;

    virtual char toChar() const = 0;
    virtual bool isRepetition() const = 0;
    virtual bool isBinaryOperation() const = 0;

    virtual void accept(op::OperatorVisitor* visitor) = 0;
  };


  bool operator==(const Operator& lhs, const Operator& rhs);

  inline bool operator!=(const Operator& lhs, const Operator& rhs) {
    return !(lhs == rhs);
  }

  std::ostream& operator<<(std::ostream& os, const Operator& character);

}
