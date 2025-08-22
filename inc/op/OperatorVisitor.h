#pragma once

#include <stdexcept>

namespace regex {

  namespace op {

  class Concatenation;
  class Alternation;
  class Wildcard;
  class Literal;
  class ZeroOrOne;
  class OneOrMore;
  class ZeroOrMore;
  class GroupingStart;
  class GroupingEnd;
  class Match;

   class OperatorVisitor {
    public:
      virtual void visit(Concatenation const * const op) { throw std::invalid_argument("concatenation operation is not implemented for this search algorithm"); }
      virtual void visit(Alternation const * const op) { throw std::invalid_argument("alternation operation is not implemented for this search algorithm"); }
      virtual void visit(Wildcard const * const op) { throw std::invalid_argument("wildcard operation is not implemented for this search algorithm"); }
      virtual void visit(Literal const * const op) { throw std::invalid_argument("literal operation is not implemented for this search algorithm"); }
      virtual void visit(ZeroOrOne const * const op) { throw std::invalid_argument("zero-or-one operation is not implemented for this search algorithm"); }
      virtual void visit(OneOrMore const * const op) { throw std::invalid_argument("one-or-more operation is not implemented for this search algorithm"); }
      virtual void visit(ZeroOrMore const * const op) { throw std::invalid_argument("zero-or-more operation is not implemented for this search algorithm"); }
      virtual void visit(GroupingStart const * const op) { throw std::invalid_argument("grouping-start operation is not implemented for this search algorithm"); }
      virtual void visit(GroupingEnd const * const op) { throw std::invalid_argument("grouping-end operation is not implemented for this search algorithm"); }
      virtual void visit(Match const* const op) = 0; // Match has to be implemented, as it is essential for the search algorithm
    };

  }

}

