
//          Copyright Joakim Karlsson & Kim Gräsman 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IGLOO_ENDSWITHCONSTRAINT_H
#define IGLOO_ENDSWITHCONSTRAINT_H

#include <igloo/constraints/expressions/expression.h>

namespace igloo {

  template <typename ExpectedType>
  struct EndsWithConstraint : Expression< EndsWithConstraint<ExpectedType> >
  {
    EndsWithConstraint(const ExpectedType& expected) 
      : m_expected(expected) {}
      
    bool operator()(const std::string& actual) const
    {
      size_t expectedPos = actual.length() - m_expected.length();
      return actual.find(m_expected) == expectedPos;
    } 
    
    ExpectedType m_expected;
  };              

  template< typename ExpectedType >
  inline EndsWithConstraint<ExpectedType> EndsWith(const ExpectedType& expected)
  {
    return EndsWithConstraint<ExpectedType>(expected);
  }
  
  inline EndsWithConstraint<std::string> EndsWith(const char* expected)
  {
    return EndsWithConstraint<std::string>(expected);
  }

  template< typename ExpectedType >
  struct Stringizer< EndsWithConstraint< ExpectedType > >
  {
    static std::string ToString(const EndsWithConstraint<ExpectedType>& constraint)
    {
      std::ostringstream builder;
	  builder << "ends with " << igloo::Stringize(constraint.m_expected);

      return builder.str();
    }
  };
}

#endif
