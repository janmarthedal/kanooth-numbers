/*
 * File:   integer.hpp
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
#include <boost/cstdint.hpp>
#include <natural_number.hpp>


namespace sputsoft {
namespace multiprecision {

namespace detail {
template <typename unsigned_type> struct to_signed_type {};
template <> struct to_signed_type<boost::uint8_t> { typedef int8_t signed_type; };
template <> struct to_signed_type<boost::uint16_t> { typedef int16_t signed_type; };
template <> struct to_signed_type<boost::uint32_t> { typedef int32_t signed_type; };
#ifndef BOOST_NO_INT64_T
template <> struct to_signed_type<boost::uint64_t> { typedef int64_t signed_type; };
#endif
}


template <typename N>
class integer {
private:
  N number;
  bool positive;
  integer& add_this(const N& v, bool v_pos);
public:
  typedef typename detail::to_signed_type<typename N::digit_type>::signed_type register_type;
  integer();
  integer(const register_type v);
  integer(const N& n, const bool pos=true);
  integer operator-() const { return integer(number, !positive); }
  bool is_zero() const { return !number; }
  bool is_positive() const { return !is_zero() && positive; }
  bool is_negative() const { return !is_zero() && !positive; }
  static N abs(const integer<N>& u);
  static integer add(const integer<N>& u, const integer<N>& v);
  static integer subtract(const integer<N>& u, const integer<N>& v);
  static integer multiply(const integer<N>& u, const integer<N>& v);
  // Sets q to u/v rounded towards -inf, r = u - q*v
  static std::pair<integer,integer> divide_floor(const integer<N>& u, const integer<N>& v);
  // Sets q to u/v rounded towards inf, r = u - q*v
  static std::pair<integer,integer> divide_ceil(const integer<N>& u, const integer<N>& v);
  // Sets q to u/v rounded towards 0, r = u - q*v
  static std::pair<integer,integer> divide_truncate(const integer<N>& u, const integer<N>& v);
  static int compare(const integer<N>& u, const integer<N>& v);
  integer& operator+=(const integer<N>& v);
  integer& operator-=(const integer<N>& v);
  integer& operator*=(const integer<N>& v);
};

template <typename N>
integer<N>::integer() : number(), positive(true)
{}

template <typename N>
integer<N>::integer(const register_type v) : number(v < 0 ? -v : v), positive(v >= 0)
{}

template <typename N>
integer<N>::integer(const N& n, const bool pos) : number(n), positive(pos)
{}


template <typename N>
inline N integer<N>::abs(const integer<N>& u)
{
  return u.number;
}

template <typename N>
integer<N>& integer<N>::add_this(const N& v, bool v_pos)
{
  if (positive == v_pos)
    number += v;
  else if (number > v)
    number -= v;
  else {
    number = v - number;
    positive = !positive;
  }
  return *this;
}

template <typename N>
integer<N> add_help(const N& u, bool u_pos, const N& v, bool v_pos)
{
  return u_pos == v_pos ? integer<N>(u + v, u_pos)
       : u > v          ? integer<N>(u - v, u_pos)
       :                  integer<N>(v - u, !u_pos);
}


template <typename N>
integer<N> integer<N>::add(const integer<N>& u, const integer<N>& v)
{
  return add_help(abs(u), u.is_positive(), abs(v), v.is_positive());
}

template <typename N>
integer<N>& integer<N>::operator+=(const integer<N>& v)
{
  return integer<N>::add_this(abs(v), v.isPositive());
}


template <typename N>
integer<N> integer<N>::subtract(const integer<N>& u, const integer<N>& v)
{
  return add_help(abs(u), u.isPositive(), abs(v), !v.isPositive());
}

template <typename N>
integer<N>& integer<N>::operator-=(const integer<N>& v)
{
  return integer<N>::add_this(abs(v), !v.isPositive());
}


template <typename N>
integer<N> integer<N>::multiply(const integer<N>& x, const integer<N>& y)
{
  return integer<N>(x.number * y.number, x.positive == y.positive);
}

template <typename N>
integer<N>& integer<N>::operator*=(const integer<N>& v)
{
  number *= v.number;
  if (!v.positive) positive = !positive;
  return *this;
}


template <typename N>
std::pair<integer<N>, integer<N> > integer<N>::divide_floor(const integer<N>& u, const integer<N>& v)
{
  std::pair<N,N> qr = N::divide(u.number, v.number);
  return !qr.second ?
      std::make_pair(integer<N>(qr.first, u.positive == v.positive), integer<N>()) :
    u.positive == v.positive ?
      std::make_pair(integer<N>(qr.first), integer<N>(qr.second, v.positive)) :
      std::make_pair(integer<N>(qr.first += 1, false), integer<N>(v.number - qr.second, v.positive));
}

template <typename N>
std::pair<integer<N>, integer<N> > integer<N>::divide_ceil(const integer<N>& u, const integer<N>& v)
{
  std::pair<N,N> qr = N::divide(u.number, v.number);
  return !qr.second ?
      std::make_pair(integer<N>(qr.first, u.positive == v.positive), integer<N>()) :
    u.is_positive() != v.is_positive() ?
      std::make_pair(integer<N>(qr.first, false), integer<N>(qr.second, u.positive)) :
      std::make_pair(integer<N>(qr.first += 1), integer<N>(v.number - qr.second, !v.positive));
}

template <typename N>
std::pair<integer<N>, integer<N> > integer<N>::divide_truncate(const integer<N>& u, const integer<N>& v)
{
  std::pair<N,N> qr = N::divide(u.number, v.number);
  return std::make_pair(integer<N>(qr.first, u.positive == v.positive),
                        !qr.second ? integer<N>() : integer<N>(qr.second, u.positive));
}


template <typename N>
int integer<N>::compare(const integer<N>& u, const integer<N>& v)
{
  if (u.is_negative() == v.is_negative()) {
    int c = N::compare(u.number, v.number);
    return u.is_negative() ? -c : c;
  }
  return u.is_negative() ? -1 : 1;
}


/*
 * EXTERNAL FUNCTIONS
 */

template <typename N>
inline bool operator==(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::compare(u, v) == 0;
}

template <typename N>
inline bool operator!=(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::compare(u, v) != 0;
}

template <typename N>
inline bool operator<(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::compare(u, v) < 0;
}

template <typename N>
inline bool operator<=(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::compare(u, v) <= 0;
}

template <typename N>
inline bool operator>(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::compare(u, v) > 0;
}

template <typename N>
inline bool operator>=(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::compare(u, v) >= 0;
}

template <typename N>
inline integer<N> operator+(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::add(u, v);
}

template <typename N>
inline integer<N> operator-(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::subtract(u, v);
}

template <typename N>
inline integer<N> operator*(const integer<N>& u, const integer<N>& v)
{
  return integer<N>::multiply(u, v);
}


template <typename N>
std::ostream& operator<<(std::ostream& os, const integer<N>& n)
{
  if (n.is_negative()) os << "-";
  os << integer<N>::abs(n);
  return os;
}


} // multiprecision
} // sputsoft


#endif	/* _INTEGER_HPP */

