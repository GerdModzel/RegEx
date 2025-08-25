#pragma once

#include "OperatorVisitor.h"

#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace regex::op {

  class Operator;
  using Vector = std::vector<std::unique_ptr<Operator>>;
  using DoubleVector = std::vector<Vector>;


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

  class Wildcard : public Operator {
  public:
    Wildcard() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<Wildcard>(*this);
    }

    char toChar() const override { return '.'; }
    bool isRepetition() const override { return false; }
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

  class Concatenation : public Operator {
  public:
    Concatenation() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<Concatenation>(*this);
    }

    char toChar() const override { return '&'; }
    bool isRepetition() const override { return false; }
    bool isBinaryOperation() const { return true; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

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
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    } private:
      char value;
  };

  class ZeroOrOne : public Operator {
  public:
    ZeroOrOne() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<ZeroOrOne>(*this);
    }

    char toChar() const override { return '?'; }
    bool isRepetition() const override { return true; }
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

  class OneOrMore : public Operator {
  public:
    OneOrMore() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<OneOrMore>(*this);
    }

    char toChar() const override { return '+'; }
    bool isRepetition() const override { return true; }
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

  class ZeroOrMore : public Operator {
  public:
    ZeroOrMore() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<ZeroOrMore>(*this);
    }

    char toChar() const override { return '*'; }
    bool isRepetition() const override { return true; }
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

  class GroupingStart : public Operator {
  public:
    GroupingStart() = default;
    std::unique_ptr<Operator> clone() const override {
      return std::make_unique<GroupingStart>(*this);
    }

    char toChar() const override { return '('; }
    bool isRepetition() const override { return false; }
    bool isBinaryOperation() const { return false; }
    virtual void accept(op::OperatorVisitor* visitor) override {
      visitor->visit(this);
    }
  };

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



  inline bool operator==(const Operator& lhs, const Operator& rhs) {
    auto plhs = dynamic_cast<const Literal*>(&lhs);
    auto prhs = dynamic_cast<const Literal*>(&rhs);
    if (plhs && prhs)
      return plhs->getValue() == prhs->getValue();
    return typeid(lhs) == typeid(rhs);
  }

  inline bool operator!=(const Operator& lhs, const Operator& rhs) {
    return !(lhs == rhs);
  }

  inline std::ostream& operator<<(std::ostream& os, const Operator& character) {
    os << character.toChar();
    return os;
  }


  inline std::string convertOpVectorToString(const op::Vector& operators) {
    std::string result;
    for (const auto& op : operators) {
      result += op->toChar();
    }
    return result;
  }


  inline std::unique_ptr<Operator> convertCharToOperator(char ch) {
    std::unique_ptr<Operator> result{ nullptr };
    switch (ch) {
    case '.': return std::make_unique<Wildcard>();
    case '+': return std::make_unique<OneOrMore>();
    case '*': return std::make_unique<ZeroOrMore>();
    case '?': return std::make_unique<ZeroOrOne>();
      // there is no explicit representation of concatenation in the expression
    case '|': return std::make_unique<Alternation>();
    case '(': return std::make_unique<GroupingStart>();
    case ')': return std::make_unique<GroupingEnd>();
    default: return std::make_unique<Literal>(ch); // Default to literal for any other character
    }
  }

  inline op::Vector convertStringToOpVector(std::string_view searchString) {
    op::Vector result;
    for (const auto& ch : searchString)
      result.push_back(convertCharToOperator(ch));
    return result;
  }

}
