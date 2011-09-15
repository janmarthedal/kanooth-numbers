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
public:
  enum round_mode {
    FLOOR, CEIL, ROUND
  };
private:
  NUM num;
  EXP exponent;
  std::size_t precision;
  round_mode rounding;

  static inline EXP bottom_exponent(const numb& n) {
    return n.exponent;
  }

  template <typename T>
  static inline EXP bottom_exponent(const T& n) {
    return 0;
  }

  static inline EXP top_exponent(const numb& n) {
    return n.exponent + floor_log2(n.num) + 1;
  }

  template <typename T>
  static inline EXP top_exponent(const T& n) {
    return floor_log2(n) + 1;
  }

  static inline const NUM& get_num(const numb& n) {
    return n.num;
  }

  template <typename T>
  static inline const T& get_num(const T& n) {
    return n;
  }

  /* Post normalize:
   *   If significand is zero, exponent must be zero
   *   If significand is non-zero, high-order digit has its top bit set
   */
  void normalize() {
    if (sputsoft::numbers::is_zero(num))
      exponent = 0;
    else {
      std::size_t cur_width = top_exponent(*this) - bottom_exponent(*this);
      if (cur_width > precision) {
        std::size_t new_width = ceil_multiple(precision, NUM::digit_bits);
        std::ptrdiff_t shift = new_width - cur_width;
        bool increment = false;
        if (rounding == ROUND) {
          std::size_t testpos = cur_width - precision - 1;
          if (sputsoft::numbers::test_bit(num, testpos)) {
            /*  |<- precision ->|
             *  |??...??????????|1??????
             *                   ^
             *                 testpos
             */
            std::size_t rightmost = sputsoft::numbers::ruler(num);
            // we always have rightmost <= testpos
            if (rightmost != testpos || sputsoft::numbers::test_bit(num, testpos+1))
              increment = true;
          }
        }
        sputsoft::numbers::bit_shift_left(num, num, shift);
        if (increment)
          sputsoft::numbers::add(num, num, 1u);
        exponent -= shift;
      } else {
        std::size_t new_width = ceil_multiple(cur_width, NUM::digit_bits);
        std::ptrdiff_t shift = new_width - cur_width;
        sputsoft::numbers::bit_shift_left(num, num, shift);
        exponent -= shift;
      }
    }
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

  template <typename T1, typename T2>
  static int cmp2(const T1& xval, const T2& yval) {
    if (sputsoft::numbers::is_zero(xval))
      return sputsoft::numbers::is_zero(yval) ? 0 : -1;
    if (sputsoft::numbers::is_zero(yval))
      return 1;
    int c = sputsoft::numbers::compare(top_exponent(xval), top_exponent(yval));
    if (c) return c;
    EXP shift = bottom_exponent(xval) - bottom_exponent(yval);
    if (shift == 0)
      return sputsoft::numbers::compare(get_num(xval), get_num(yval));
    NUM tmp;
    if (shift > 0) {
      sputsoft::numbers::bit_shift_left(tmp, get_num(xval), shift);
      return sputsoft::numbers::compare(tmp, get_num(yval));
    } else {
      sputsoft::numbers::bit_shift_left(tmp, get_num(yval), -shift);
      return sputsoft::numbers::compare(get_num(xval), tmp);
    }
  }

public:
  numb() {
    precision = DEFPREC;
    rounding = ROUND;
  }
  template <typename V>
  numb(const V& v) {
    precision = DEFPREC;
    rounding = ROUND;
    sputsoft::numbers::set(*this, v);
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
    precision = prec;  // ceil_multiple(prec, NUM::digit_bits);
    normalize();
  }

  void set_rounding_mode(round_mode mode) {
    rounding = mode;
  }

  static void floor(NUM& x, const numb& v) {
    sputsoft::numbers::bit_shift_left(x, v.num, v.exponent);
  }

  static void round(NUM& x, const numb& v) {
    bool increment = v.exponent < 0 && sputsoft::numbers::test_bit(v.num, -v.exponent-1);
    sputsoft::numbers::bit_shift_left(x, v.num, v.exponent);
    if (increment)
      sputsoft::numbers::add(x, x, 1u);
  }

  template <typename T1, typename T2>
  inline void add(const T1& xval, const T2& yval) {
    if (sputsoft::numbers::is_zero(xval))
      sputsoft::numbers::set(*this, yval);
    else if (sputsoft::numbers::is_zero(yval))
      sputsoft::numbers::set(*this, xval);
    else {
      EXP xexp = bottom_exponent(xval);
      EXP yexp = bottom_exponent(yval);
      EXP shift = xexp - yexp;
      if (shift == 0) {
        sputsoft::numbers::add(num, get_num(xval), get_num(yval));
        exponent = xexp;
      } else {
        NUM tmp;
        if (shift > 0) {
          sputsoft::numbers::bit_shift_left(tmp, get_num(xval), shift);
          sputsoft::numbers::add(num, tmp, get_num(yval));
          exponent = yexp;
        } else {
          sputsoft::numbers::bit_shift_left(tmp, get_num(yval), -shift);
          sputsoft::numbers::add(num, get_num(xval), tmp);
          exponent = xexp;
        }
      }
    }
    normalize();
  }

  template <typename T1, typename T2>
  inline void sub(const T1& xval, const T2& yval) {
    if (sputsoft::numbers::is_zero(xval))
      sputsoft::numbers::set(*this, yval);
    else if (sputsoft::numbers::is_zero(yval))
      sputsoft::numbers::set(*this, xval);
    else {
      EXP xexp = bottom_exponent(xval);
      EXP yexp = bottom_exponent(yval);
      EXP shift = xexp - yexp;
      if (shift == 0) {
        sputsoft::numbers::sub(num, get_num(xval), get_num(yval));
        exponent = xexp;
      } else {
        NUM tmp;
        if (shift > 0) {
          sputsoft::numbers::bit_shift_left(tmp, get_num(xval), shift);
          sputsoft::numbers::sub(num, tmp, get_num(yval));
          exponent = yexp;
        } else {
          sputsoft::numbers::bit_shift_left(tmp, get_num(yval), -shift);
          sputsoft::numbers::sub(num, get_num(xval), tmp);
          exponent = xexp;
        }
      }
    }
    normalize();
  }

  template <typename T1, typename T2>
  inline void mul(const T1& x, const T2& y) {
    EXP xexp = bottom_exponent(x);
    EXP yexp = bottom_exponent(y);
    sputsoft::numbers::mul(num, get_num(x), get_num(y));
    exponent = xexp + yexp;
    normalize();
  }

  template <typename T1, typename T2>
  inline void div(const T1& xval, const T2& yval) {
    if (sputsoft::numbers::is_zero(xval))
      sputsoft::numbers::set(num, 0u);
    else if (sputsoft::numbers::is_zero(yval))
      sputsoft::numbers::set(num, 0u);  // TODO: What action to take?
    else {
      EXP xexp = bottom_exponent(xval);
      EXP yexp = bottom_exponent(yval);
      std::size_t xbits = top_exponent(xval) - xexp;
      std::size_t ybits = top_exponent(yval) - yexp;
      std::size_t destbits = precision;
      if (!destbits)
        destbits = std::max(xbits, ybits);
      destbits += NUM::digit_bits - 1;  // guard digits
      std::size_t numeratorbits = ybits + destbits;
      std::ptrdiff_t numeratorshift = numeratorbits - xbits;
      NUM tmp;
      sputsoft::numbers::bit_shift_left(tmp, xval, numeratorshift);
      sputsoft::numbers::div(num, tmp, yval);
      exponent = xexp - numeratorshift - yexp;
    }
    normalize();
  }

  template <typename T>
  inline int cmp(const T& rhs) const {
    return cmp2(*this, rhs);
  }

  std::ostream& show_binary(std::ostream& os) const {
    NUM n;
    floor(n, *this);
    sputsoft::numbers::show_binary(os, num);
    return os << " (" << exponent << "," << (top_exponent(*this) - bottom_exponent(*this)) << ") ~ " << n;
  }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP
