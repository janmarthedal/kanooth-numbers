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
class numb<posfloatnum<NUM, EXP, DEFPREC> >;

namespace {
  template <typename T>
  struct bit_width_eval {
    std::size_t operator()(const T& n) {
      return floor_log2(n) + 1;
    }
  };
  template <typename N, typename E, std::size_t D>
  struct bit_width_eval<numb<posfloatnum<N, E, D> > > {
    std::size_t operator()(const numb<posfloatnum<N, E, D> >& n) {
      return n.significand_bit_width();
    }    
  };
  template <typename T>
  std::size_t bit_width(const T& n) {
    return bit_width_eval<T>()(n);
  }
  template <typename T>
  struct exponent_eval {
    std::size_t operator()(const T& n) {
      return 0;
    }
  };
  template <typename N, typename E, std::size_t D>
  struct exponent_eval<numb<posfloatnum<N, E, D> > > {
    std::size_t operator()(const numb<posfloatnum<N, E, D> >& n) {
      return n.exponent;
    }    
  };
  template <typename T>
  std::size_t get_exponent(const T& n) {
    return exponent_eval<T>()(n);
  }
}

template <typename NUM, typename EXP, std::size_t DEFPREC>
class numb<posfloatnum<NUM, EXP, DEFPREC> > {
  friend struct exponent_eval<numb<posfloatnum<NUM, EXP, DEFPREC> > >;
public:
  enum round_mode {
    FLOOR, CEIL, ROUND
  };
private:
  NUM num;
  EXP exponent;
  std::size_t precision;
  round_mode rounding;

  /* Post normalize:
   *   If significand is zero, exponent must be zero
   *   If significand is non-zero, high-order digit has its top bit set
   */
  void normalize() {
    if (sputsoft::numbers::is_zero(num))
      exponent = 0;
    else {
      std::size_t cur_width = significand_bit_width();
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
  void add2(const T1& xval, EXP xexp, const T2& yval, EXP yexp) {
    NUM tmp;
    if (xexp >= yexp) {
      sputsoft::numbers::bit_shift_left(tmp, xval, xexp - yexp);
      sputsoft::numbers::add(num, tmp, yval);
      exponent = yexp;
    } else {
      sputsoft::numbers::bit_shift_left(tmp, yval, yexp - xexp);
      sputsoft::numbers::add(num, xval, tmp);
      exponent = xexp;
    }
    normalize();
  }

  template <typename T1, typename T2>
  void sub2(const T1& xval, EXP xexp, const T2& yval, EXP yexp) {
    NUM tmp;
    if (xexp >= yexp) {
      sputsoft::numbers::bit_shift_left(tmp, xval, xexp - yexp);
      sputsoft::numbers::sub(num, tmp, yval);
      exponent = yexp;
    } else {
      sputsoft::numbers::bit_shift_left(tmp, yval, yexp - xexp);
      sputsoft::numbers::sub(num, xval, tmp);
      exponent = xexp;
    }
    normalize();
  }

  template <typename T1, typename T2>
  void mul2(const T1& xval, EXP xexp, const T2& yval, EXP yexp) {
    sputsoft::numbers::mul(num, xval, yval);
    exponent = xexp + yexp;
    normalize();
  }

  template <typename T1, typename T2>
  void div2(const T1& xval, std::size_t xbits, EXP xexp,
            const T2& yval, std::size_t ybits, EXP yexp) {
    if (sputsoft::numbers::is_zero(xval))
      sputsoft::numbers::set(num, 0u);
    else if (sputsoft::numbers::is_zero(yval))
      sputsoft::numbers::set(num, 0u);  // TODO: What action to take?
    else {
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

  template <typename T1, typename T2>
  static int cmp3(const T1& xval, EXP xexp, const T2& yval, EXP yexp) {
    NUM tmp;
    if (xexp >= yexp) {
      sputsoft::numbers::bit_shift_left(tmp, xval, xexp - yexp);
      return sputsoft::numbers::compare(tmp, yval);
    } else {
      sputsoft::numbers::bit_shift_left(tmp, yval, yexp - xexp);
      return sputsoft::numbers::compare(xval, tmp);
    }
  }

  template <typename T1, typename T2>
  static int cmp2(const T1& xval, std::size_t xbits, EXP xexp,
            const T2& yval, std::size_t ybits, EXP yexp) {
    int c = sputsoft::numbers::compare(xexp + xbits, yexp + ybits);
    return c ? c : cmp3(xval, xexp, yval, yexp);
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

  std::size_t significand_bit_width() const {
    return floor_log2(num) + 1;
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

  inline void add(const numb& x, const numb& y) {
    add2(x.num, x.exponent, y.num, y.exponent);
  }

  template <typename T>
  inline void add(const numb& x, const T& y) {
    add2(x.num, x.exponent, y, 0);
  }

  template <typename T>
  inline void add(const T& x, const numb& y) {
    add2(x, 0, y.num, y.exponent);
  }

  inline void sub(const numb& x, const numb& y) {
    sub2(x.num, x.exponent, y.num, y.exponent);
  }

  template <typename T>
  inline void sub(const numb& x, const T& y) {
    sub2(x.num, x.exponent, y, 0);
  }

  template <typename T>
  inline void sub(const T& x, const numb& y) {
    sub2(x, 0, y.num, y.exponent);
  }

  inline void mul(const numb& x, const numb& y) {
    mul2(x.num, x.exponent, y.num, y.exponent);
  }

  template <typename T>
  inline void mul(const numb& x, const T& y) {
    mul2(x.num, x.exponent, y, 0);
  }

  template <typename T>
  inline void mul(const T& x, const numb& y) {
    mul2(x, 0, y.num, y.exponent);
  }

  inline void div(const numb& x, const numb& y) {
    div2(x.num, x.significand_bit_width(), x.exponent,
         y.num, y.significand_bit_width(), y.exponent);
  }

  template <typename T>
  inline void div(const numb& x, const T& y) {
    div2(x.num, x.significand_bit_width(), x.exponent,
         y, sputsoft::numbers::floor_log2(y) + 1, 0);
  }

  template <typename T>
  inline void div(const T& x, const numb& y) {
    div2(x, sputsoft::numbers::floor_log2(x) + 1, 0,
         y.num, x.significand_bit_width(), y.exponent);
  }

  template <typename T1, typename T2>
  inline void div(const T1& x, const T2& y) {
    div2(x, sputsoft::numbers::floor_log2(x) + 1, 0,
         y, sputsoft::numbers::floor_log2(y) + 1, 0);
  }

  inline int cmp(const numb& y) const {
    return cmp2(num, bit_width(*this), get_exponent(*this),
                y.num, bit_width(y), get_exponent(y));
  }

  template <typename T>
  inline int cmp(const T& y) const {
    return cmp2(num, bit_width(*this), get_exponent(*this),
                y, bit_width(y), get_exponent(y));
  }

  std::ostream& show_binary(std::ostream& os) const {
    NUM n;
    floor(n, *this);
    sputsoft::numbers::show_binary(os, num);
    return os << " (" << exponent << "," << significand_bit_width() << ") ~ " << n;
  }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_POSITIVE_FLOAT_IMPL_HPP
