/*
 * File:   NNIutils.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-27 11:43Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _NNIUTILS_HPP
#define _NNIUTILS_HPP

/*
namespace com {
namespace sputsoft {
namespace multiprecision {
*/


template <typename T, typename V>
inline bool operator==(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) == 0;
}

template <typename T, typename V>
inline bool operator!=(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) != 0;
}

template <typename T, typename V>
inline bool operator<(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) < 0;
}

template <typename T, typename V>
inline bool operator<=(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) <= 0;
}

template <typename T, typename V>
inline bool operator>(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) > 0;
}

template <typename T, typename V>
inline bool operator>=(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::compare(u, v) >= 0;
}

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator+(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::add(u, v);
}

template <typename T, typename V>
NonNegativeInteger<T,V> operator-(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::subtract(u, v);
}

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator*(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::multiply(u, v);
}

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator/(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::divide(u, v).first;
}

template <typename T, typename V>
inline NonNegativeInteger<T,V> operator%(const NonNegativeInteger<T,V>& u, const NonNegativeInteger<T,V>& v)
{
  return NonNegativeInteger<T,V>::divide(u, v).second;
}


template <typename NONNEGNUM>
NONNEGNUM string_to_nni(const std::string& digits, unsigned short base=10)
{
  NONNEGNUM r = NONNEGNUM::zero;
  if (base >= 2 && base <= 36) {
    NONNEGNUM b(base);
    unsigned short v;
    for (std::string::const_iterator p=digits.begin(); p != digits.end(); ++p) {
      char c = *p;
      if (c >= '0' && c <= '9') v = c - '0';
      else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
      else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
      else break;
      r *= b;
      r += NONNEGNUM(v);
    }
  }
  return r;
}

template <typename NUM, typename Out>
void factorize(NUM n, Out out)
{
  NUM d(2);
  NUM one(1), two(2), four(4);
  std::pair<NUM,NUM> divrem;

  while (d <= n) {
    divrem = NUM::divide(n, d);
    if (divrem.second == NUM::zero) {
      *out++ = d;
      n = divrem.first;
    } else
      d += one;
  }
}

template <typename NUM>
NUM factorial(unsigned n)
{
  NUM r(1);
  for (unsigned k=2; k <= n; ++k)
    r *= k;
  return r;
}

/*
} // multiprecision
} // sputsoft
} // com
*/

#endif	/* _NNIUTILS_HPP */
