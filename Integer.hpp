/* 
 * File:   Integer.hpp
 * Author: jmr
 *
 * Created on 5. maj 2009, 15:52
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _INTEGER_HPP
#define	_INTEGER_HPP

#include <iostream>
#include "NonNegativeInteger.hpp"

namespace com {
namespace sputsoft {
namespace multiprecision {

template <typename T>
class Integer {
  template <typename S>
  friend std::ostream& operator<<(std::ostream& os, const Integer<S>& n);
private:
  bool positive;
  NonNegativeInteger<T> number;
public:
  Integer(const NonNegativeInteger<T>& n, const bool pos=true) : positive(pos), number(n) {}
  Integer operator-() const { return Integer(number, !positive); }
  bool isZero() const { return number.isZero(); }
  bool isPositive() const { return !isZero() && positive; }
  bool isNegative() const { return !isZero() && !positive; }
  static Integer add(const Integer<T>& x, const Integer<T>& y);
  static Integer subtract(const Integer<T>& x, const Integer<T>& y);
};

template <typename T>
Integer<T> Integer<T>::add(const Integer<T>& x, const Integer<T>& y)
{
  if (x.isZero()) return y;
  if (y.isZero()) return x;
  if (x.isPositive() == y.isPositive())
    return Integer<T>(x.number + y.number, x.isPositive());
  bool xLarger = x.number > y.number;
  return Integer<T>(xLarger ? x.number - y.number : y.number - x.number, xLarger == x.isPositive());
}

template <typename T>
Integer<T> Integer<T>::subtract(const Integer<T>& x, const Integer<T>& y)
{
  if (x.isZero()) return -y;
  if (y.isZero()) return x;
  if (x.isPositive() != y.isPositive())
    return Integer<T>(x.number + y.number, x.isPositive());
  bool xLarger = x.number > y.number;
  return Integer<T>(xLarger ? x.number - y.number : y.number - x.number, xLarger == x.isPositive());
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Integer<T>& n)
{
  if (n.isNegative()) os << "-";
  os << n.number;
  return os;
}

} // multiprecision
} // sputsoft
} // com

#endif	/* _INTEGER_HPP */

