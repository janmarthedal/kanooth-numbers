/*
 * File:   kanooth/numbers/detail/positive_float/pos_float_impl.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-08-24 09:00Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP
#define _KANOOTH_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/common_functions.hpp>
#include <kanooth/numbers/detail/positive_float/pos_float_abst.hpp>

namespace kanooth {
namespace numbers {

namespace detail {

std::size_t ceil_multiple(std::size_t x, unsigned a)
{
  return kanooth::numbers::ceil_div(x, a) * a;
}

struct Adder {
  template <typename R, typename T1, typename T2>
  inline void operator()(R& r, const T1& v1, const T2& v2) {
    kanooth::numbers::add(r, v1, v2);
  }
};

struct Subtracter {
  template <typename R, typename T1, typename T2>
  inline void operator()(R& r, const T1& v1, const T2& v2) {
    kanooth::numbers::sub(r, v1, v2);
  }
};

template <typename NUM, typename EXP, std::size_t DEFPREC>
class numb<posfloatnum<NUM, EXP, DEFPREC> > {
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

  static inline const NUM& get_value(const numb& n) {
    return n.num;
  }

  template <typename T>
  static inline const T& get_value(const T& n) {
    return n;
  }

  inline void set_num(const NUM& n, EXP e) {
    kanooth::numbers::set(num, n);
    exponent = e;
    normalize();
  }

  template <typename T>
  void set_int(T v) {
    kanooth::numbers::set(num, v);
    exponent = 0;
    normalize();
  }

  /* Post normalize:
   *   If significand is zero, exponent must be zero
   *   If significand is non-zero, high-order digit has its top bit set
   */
  void normalize() {
    if (!kanooth::numbers::is_zero(num)) {
      std::size_t cur_width = top_exponent(*this) - bottom_exponent(*this);
      std::size_t new_width;
      bool increment = false;
      if (cur_width > precision) {
        new_width = ceil_multiple(precision, NUM::digit_bits);
        std::size_t testpos = cur_width - precision - 1;
        /*  |<- cur_width --------->|
         *  |<- new_width ->|
         *  |XX...XXXXXXXXXX|1XX..XX|
         *                   ^
         *                 testpos        */
        if (rounding == ROUND) {
          if (kanooth::numbers::test_bit(num, testpos)) {
            std::size_t rightmost = kanooth::numbers::ruler(num);
            // we always have testpos >= rightmost
            increment = kanooth::numbers::test_bit(num, testpos+1)
                        || rightmost != testpos;
          }
        } else if (rounding == CEIL) {
          std::size_t rightmost = kanooth::numbers::ruler(num);
          increment = testpos >= rightmost;
        } else {
          // never increment for TRUNC or FLOOR
        }
      } else
        new_width = ceil_multiple(cur_width, NUM::digit_bits);
      std::ptrdiff_t shift = new_width - cur_width;
      kanooth::numbers::bit_shift_left(num, num, shift);
      if (increment)
        kanooth::numbers::add(num, num, 1u);
      exponent -= shift;
    } else
      exponent = 0;
  }

  template <typename T1, typename T2, typename OP>
  void _add_sub(const T1& xval, const T2& yval, OP op) {
    if (kanooth::numbers::is_zero(xval))
      kanooth::numbers::set(*this, yval);
    else if (kanooth::numbers::is_zero(yval))
      kanooth::numbers::set(*this, xval);
    else {
      EXP xexp = bottom_exponent(xval);
      EXP yexp = bottom_exponent(yval);
      EXP shift = xexp - yexp;
      if (shift == 0) {
        op(num, get_value(xval), get_value(yval));
        exponent = xexp;
      } else {
        NUM tmp;
        if (shift > 0) {
          kanooth::numbers::bit_shift_left(tmp, get_value(xval), shift);
          op(num, tmp, get_value(yval));
          exponent = yexp;
        } else {
          kanooth::numbers::bit_shift_left(tmp, get_value(yval), -shift);
          op(num, get_value(xval), tmp);
          exponent = xexp;
        }
      }
    }
    normalize();
  }

  template <typename T1, typename T2>
  static int _cmp(const T1& xval, const T2& yval) {
    if (kanooth::numbers::is_zero(xval))
      return kanooth::numbers::is_zero(yval) ? 0 : -1;
    if (kanooth::numbers::is_zero(yval))
      return 1;
    int c = kanooth::numbers::compare(top_exponent(xval), top_exponent(yval));
    if (c) return c;
    EXP shift = bottom_exponent(xval) - bottom_exponent(yval);
    if (shift == 0)
      return kanooth::numbers::compare(get_value(xval), get_value(yval));
    NUM tmp;
    if (shift > 0) {
      kanooth::numbers::bit_shift_left(tmp, get_value(xval), shift);
      return kanooth::numbers::compare(tmp, get_value(yval));
    } else {
      kanooth::numbers::bit_shift_left(tmp, get_value(yval), -shift);
      return kanooth::numbers::compare(get_value(xval), tmp);
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
    kanooth::numbers::set(*this, v);
  }
  //numb(const NUM& n) : precision(DEFPREC) { set_num(n, 0); }
  template <typename V>
  numb& operator=(const V& v) {
    kanooth::numbers::set(*this, v);
    return *this;
  }
  inline bool is_zero() const {
    return !num;
  }
  inline bool is_positive() const {
    return !is_zero();
  }
  inline operator bool() const {
    return !is_zero();
  }

  inline void set(const numb& v) {
    set_num(v.num, v.exponent);
  }
  inline void set(const NUM& n) {
    set_num(n, 0);
  }
  template <typename T>
  inline void set(T v) {
    set_int(v);
  }

  void set_precision(std::size_t prec) {
    precision = prec;  // ceil_multiple(prec, NUM::digit_bits);
    normalize();
  }

  void set_rounding_mode(round_mode mode) {
    rounding = mode;
  }

  void floor_to(NUM& x) const {
    kanooth::numbers::bit_shift_left(x, num, exponent);
  }

  static void round(NUM& x, const numb& v) {
    bool increment = v.exponent < 0 && kanooth::numbers::test_bit(v.num, -v.exponent-1);
    kanooth::numbers::bit_shift_left(x, v.num, v.exponent);
    if (increment)
      kanooth::numbers::add(x, x, 1u);
  }

  template <typename T1, typename T2>
  inline void add(const T1& xval, const T2& yval) {
    _add_sub(xval, yval, Adder());
  }

  template <typename T1, typename T2>
  inline void sub(const T1& xval, const T2& yval) {
    _add_sub(xval, yval, Subtracter());
  }

  template <typename T1, typename T2>
  inline void mul(const T1& x, const T2& y) {
    EXP xexp = bottom_exponent(x);
    EXP yexp = bottom_exponent(y);
    kanooth::numbers::mul(num, get_value(x), get_value(y));
    exponent = xexp + yexp;
    normalize();
  }

  template <typename T1, typename T2>
  inline void div(const T1& xval, const T2& yval) {
    if (kanooth::numbers::is_zero(xval))
      kanooth::numbers::set(num, 0u);
    else if (kanooth::numbers::is_zero(yval))
      kanooth::numbers::set(num, 0u);  // TODO: Set NaN, +inf, -inf
    else {
      EXP xexp = bottom_exponent(xval);
      EXP yexp = bottom_exponent(yval);
      std::size_t xbits = top_exponent(xval) - xexp;
      std::size_t ybits = top_exponent(yval) - yexp;
      std::size_t destbits = ceil_multiple(precision ? precision
              : std::max(xbits, ybits), NUM::digit_bits) + NUM::digit_bits;
      std::size_t numeratorbits = ybits + destbits;
      std::ptrdiff_t numeratorshift = numeratorbits - xbits;
      NUM tmp;
      kanooth::numbers::bit_shift_left(tmp, get_value(xval), numeratorshift);
      kanooth::numbers::div(num, tmp, get_value(yval));
      exponent = xexp - numeratorshift - yexp;
    }
    normalize();
  }

  template <typename T>
  inline int cmp(const T& rhs) const {
    return _cmp(*this, rhs);
  }

  template <typename T>
  void sqrt(const T& rhs) {
    EXP exp = bottom_exponent(rhs);
    std::size_t rhs_bits = top_exponent(rhs) - exp;
    std::size_t dst_bits = ceil_multiple(precision ? precision
            : rhs_bits/2, NUM::digit_bits) + NUM::digit_bits;
    std::size_t src_bits = 2*dst_bits;
    EXP shift = src_bits > rhs_bits ? src_bits - rhs_bits : 0;
    if ((exp - shift) % 2 != 0)
      shift--;
    NUM tmp;
    kanooth::numbers::bit_shift_left(tmp, get_value(rhs), shift);
    kanooth::numbers::set(num, kanooth::numbers::integer_sqrt(tmp));
    exponent = (exp - shift)/2;
  }

  std::ostream& show_binary(std::ostream& os) const {
    NUM n;
    floor(n, *this);
    kanooth::numbers::show_binary(os, num);
    return os << " (" << exponent << ","
              << (top_exponent(*this) - bottom_exponent(*this)) << ") ~ " << n;
  }

};

} // namespace detail
} // namespace kanooth
} // namespace numbers

#endif // _KANOOTH_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP
