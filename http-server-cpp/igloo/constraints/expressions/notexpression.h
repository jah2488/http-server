
//          Copyright Joakim Karlsson & Kim Gräsman 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IGLOO_NOTEXPRESSION_H
#define IGLOO_NOTEXPRESSION_H
  
#include <igloo/constraints/expressions/expression_fwd.h>

namespace igloo {

  template< typename ExpressionType >
  struct NotExpression : Expression< NotExpression<ExpressionType> >
  {
    NotExpression(const ExpressionType& expression)
      : m_expression(expression)
    {
    }

    template<typename ActualType>
    bool operator()(const ActualType& actual) const
    {
      return !m_expression(actual);
    }

    ExpressionType m_expression;
  };

  template< typename ExpressionType >
  struct Stringizer< NotExpression<ExpressionType> >
  {
    static std::string ToString(const NotExpression<ExpressionType>& expression)
    {
      std::ostringstream builder;
	  builder << "not " << igloo::Stringize(expression.m_expression);

      return builder.str();
    }
  };
}

#endif
