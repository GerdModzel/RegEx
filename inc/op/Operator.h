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
    
    /// Returns a character representation of the operator.
    virtual char toChar() const = 0;
    /// Returns true if the operator is a repetition operator (like '*', '+', '?').
    virtual bool isRepetition() const = 0;
    ///  Returns the number of arguments the operator needs (Wildcard needs 0, Concatenation needs 2, ZeroOrMore needs 1).
    virtual int nArgumentsRequired() const = 0;
  
    /// Accepts a visitor that can perform operations based on the specific operator type (Visitor Pattern).
    virtual void accept(op::OperatorVisitor* visitor) = 0;
  };


  bool operator==(const Operator& lhs, const Operator& rhs);

  inline bool operator!=(const Operator& lhs, const Operator& rhs) {
    return !(lhs == rhs);
  }

  std::ostream& operator<<(std::ostream& os, const Operator& character);

}
