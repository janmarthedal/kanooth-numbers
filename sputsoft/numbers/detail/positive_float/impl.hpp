/*
 * File:   sputsoft/numbers/detail/positive_float/impl.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-08-24 09:00Z
 *
 * (C) Copyright SputSoft 2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP

#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/detail/positive_float/abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

std::size_t ceil_multiple(std::size_t x, unsigned a)
{
  return ((x + a - 1)/a)*a;
}

template <typename NUM, typename EXP, std::size_t DEFPREC>
class numb<posfloatnum<NUM, EXP, DEFPREC> > {
private:
  NUM num;
  EXP exponent;
  std::size_t precision;

  void normalize() {
    if (sputsoft::numbers::is_zero(num)) return;
    std::size_t bits_used = floor_log2(num) + 1;
    std::size_t to_use = ceil_multiple(bits_used, NUM::digit_bits);
    std::ptrdiff_t shift = to_use - bits_used;
    bit_shift_left(num, num, shift);
    exponent -= shift;
  }

  void round_and_normalize() {

  }

  inline void set_num(const NUM& n, EXP e) {
    sputsoft::numbers::set(num, n);
    exponent = e;
    normalize();
  }

  template <typename T>
  void set_int(T v) {
    sputsoft::numbers::set(num, v);
    exponent = 0;
    normalize();
  }

public:
  numb() {
    set_precision(DEFPREC);
  }
  template <typename V>
  numb(const V& v) {
    sputsoft::numbers::set(*this, v);
    set_precision(DEFPREC);
  }
  //numb(const NUM& n) : precision(DEFPREC) { set_num(n, 0); }
  template <typename V>
  numb& operator=(const V& v) {
    sputsoft::numbers::set(*this, v);
    return *this;
  }
  inline bool is_zero() const { return !num; }
  inline bool is_positive() const { return num; }
  inline operator bool() const { return !is_zero(); }

  inline void set(const numb& v) { set_num(v.num, v.exponent); }
  inline void set(const NUM& n) { set_num(n, 0); }
  template <typename T>
  inline void set(T v) { set_int(v); }

  void set_precision(std::size_t prec) {
    precision = ceil_multiple(prec, NUM::digit_bits);
  }

  static void trunc(NUM& x, const numb& v) {
    sputsoft::numbers::bit_shift_left(x, v.num, v.exponent);
  }

  void add(const numb& x, const numb& y) {
    NUM tmp;
    if (x.exponent >= y.exponent) {
      sputsoft::numbers::bit_shift_left(tmp, x.num, x.exponent - y.exponent);
      sputsoft::numbers::add(num, tmp, y.num);
      exponent = y.exponent;
    } else {
      sputsoft::numbers::bit_shift_left(tmp, y.num, y.exponent - x.exponent);
      sputsoft::numbers::add(num, x.num, tmp);
      exponent = x.exponent;
    }
    normalize();
  }

  void sub(const numb& x, const numb& y) {
    NUM tmp;
    if (x.exponent >= y.exponent) {
      sputsoft::numbers::bit_shift_left(tmp, x.num, x.exponent - y.exponent);
      sputsoft::numbers::sub(num, tmp, y.num);
      exponent = y.exponent;
    } else {
      sputsoft::numbers::bit_shift_left(tmp, y.num, y.exponent - x.exponent);
      sputsoft::numbers::sub(num, x.num, tmp);
      exponent = x.exponent;
    }
    normalize();
  }

  void mul(const numb& x, const numb& y) {
    sputsoft::numbers::mul(num, x.num, y.num);
    exponent = x.exponent + y.exponent;
    normalize();
  }

  void div(const numb& x, const numb& y) {
    if (sputsoft::numbers::is_zero(x.num))
      sputsoft::numbers::set(num, 0u);
    else if (sputsoft::numbers::is_zero(y.num))
      sputsoft::numbers::set(num, 0u);  // TODO: What action to take?
    else {
      std::size_t xbits = sputsoft::numbers::floor_log2(x.num) + 1;
      std::size_t ybits = sputsoft::numbers::floor_log2(y.num) + 1;
      std::size_t destbits = precision;
      if (!destbits)
        destbits = std::max(xbits, ybits);
      destbits += NUM::digit_bits - 1;  // guard digits
      std::size_t numeratorbits = ybits + destbits;
      std::ptrdiff_t numeratorshift = numeratorbits - xbits;
      NUM tmp;
      sputsoft::numbers::bit_shift_left(tmp, x.num, numeratorshift);
      sputsoft::numbers::div(num, tmp, y.num);
      exponent = x.exponent - numeratorshift - y.exponent;
      round_and_normalize();
    }
  }

  EXP get_exponent() const {
    return exponent;
  }

  NUM get_num() const {
    return num;
  }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP
