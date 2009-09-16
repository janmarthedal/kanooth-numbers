/*
 * File:   Integer.hpp
 * Author: Jan Marthedal Rasmussen
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
//#include "NonNegativeInteger.hpp"

namespace sputsoft {
namespace multiprecision {

template <typename N>
class Integer {
  template <typename S>
  friend std::ostream& operator<<(std::ostream& os, const Integer<S>& n);
private:
  bool positive;
  N number;
public:
  Integer(const N& n, const bool pos=true) : positive(pos), number(n) {}
  Integer operator-() const { return Integer(number, !positive); }
  bool isZero() const { return number.isZero(); }
  bool isPositive() const { return !isZero() && positive; }
  bool isNegative() const { return !isZero() && !positive; }
  static Integer add(const Integer<N>& x, const Integer<N>& y);
  static Integer subtract(const Integer<N>& x, const Integer<N>& y);
  static Integer multiply(const Integer<N>& x, const Integer<N>& y);
};

template <typename N>
Integer<N> Integer<N>::add(const Integer<N>& x, const Integer<N>& y)
{
  if (x.isZero()) return y;
  if (y.isZero()) return x;
  if (x.isPositive() == y.isPositive())
    return Integer<N>(x.number + y.number, x.isPositive());
  bool xLarger = x.number > y.number;
  return Integer<N>(xLarger ? x.number - y.number : y.number - x.number, xLarger == x.isPositive());
}

template <typename N>
Integer<N> Integer<N>::subtract(const Integer<N>& x, const Integer<N>& y)
{
  if (x.isZero()) return -y;
  if (y.isZero()) return x;
  if (x.isPositive() != y.isPositive())
    return Integer<N>(x.number + y.number, x.isPositive());
  bool xLarger = x.number > y.number;
  return Integer<N>(xLarger ? x.number - y.number : y.number - x.number, xLarger == x.isPositive());
}

template <typename N>
Integer<N> Integer<N>::multiply(const Integer<N>& x, const Integer<N>& y)
{
  return Integer<N>(x.number * y.number, x.positive == y.positive);
}

template <typename N>
inline Integer<N> operator*(const Integer<N>& u, const Integer<N>& v)
{
  return Integer<N>::multiply(u, v);
}

template <typename N>
std::ostream& operator<<(std::ostream& os, const Integer<N>& n)
{
  if (n.isNegative()) os << "-";
  os << n.number;
  return os;
}

} // multiprecision
} // sputsoft

#endif	/* _INTEGER_HPP */

