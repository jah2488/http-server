
//          Copyright Joakim Karlsson & Kim Gräsman 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IGLOO_EQUALSCONSTRAINT_H
#define IGLOO_EQUALSCONSTRAINT_H

#include <igloo/constraints/expressions/expression.h>

namespace igloo {

  template< typename ExpectedType >
  struct EqualsConstraint : Expression< EqualsConstraint<ExpectedType> >
  {
    EqualsConstraint(const ExpectedType& expected)
      : m_expected(expected)
    {
    }

    template<typename ActualType>
    bool operator()(const ActualType& actual) const
    {
      return (m_expected == actual);
    }

    ExpectedType m_expected;
  };

  template< typename ExpectedType >
  inline EqualsConstraint<ExpectedType> Equals(const ExpectedType& expected)
  {
    return EqualsConstraint<ExpectedType>(expected);
  }

  inline EqualsConstraint<std::string> Equals(const char* expected)
  {
    return EqualsConstraint<std::string>(expected);
  }

  inline EqualsConstraint<bool> IsFalse()
  {
    return EqualsConstraint<bool>(false);
  }

  inline EqualsConstraint<bool> IsTrue()
  {
    return EqualsConstraint<bool>(true);
  }

  template <>
  struct Stringizer< EqualsConstraint< bool > >
  {
    static std::string ToString(const EqualsConstraint<bool>& constraint)
    {
      return constraint.m_expected ? "true" : "false";
    }
  };

  template< typename ExpectedType >
  struct Stringizer< EqualsConstraint< ExpectedType > >
  {
    static std::string ToString(const EqualsConstraint<ExpectedType>& constraint)
    {
      std::ostringstream builder;
	  builder << "equal to " << igloo::Stringize(constraint.m_expected);

      return builder.str();
    }
  };
}

#endif
