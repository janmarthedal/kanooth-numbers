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

#ifndef _KANOOTH_NUMBERS_DETAIL_RATIONAL_NUMBER_IMPL_HPP
#define _KANOOTH_NUMBERS_DETAIL_RATIONAL_NUMBER_IMPL_HPP

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/number_theory.hpp>
#include <kanooth/numbers/detail/rational_number/rat_num_abst.hpp>

namespace kanooth {
namespace numbers {

namespace detail {

template <typename NUM, typename DEN>
class numb<ratnum<NUM, DEN> > {
private:
  typedef typename kanooth::numbers::gcd_result<NUM, DEN>::type gcd_type;
  NUM numerator;
  DEN denominator;

  void set_this(const NUM& num, const DEN& den) {
    if (kanooth::numbers::is_zero(num)) {
      kanooth::numbers::set(numerator, 0u);
      kanooth::numbers::set(denominator, 1u);
    } else {
      gcd_type g = kanooth::numbers::gcd(num, den);
      kanooth::numbers::div(numerator, num, g);
      kanooth::numbers::div(denominator, den, g);
    }
  }

  void _add_sub(const NUM& a, const DEN& b, const NUM& c, const DEN& d, bool adding) {
    NUM num, tmp;
    DEN den;
    gcd_type g = kanooth::numbers::gcd(b, d);
    kanooth::numbers::div(den, d, g);
    kanooth::numbers::mul(num, a, den);
    kanooth::numbers::mul(tmp, c, kanooth::numbers::div(b, g));
    kanooth::numbers::mul(den, den, b);
    if (adding)
      kanooth::numbers::add(num, num, tmp);
    else
      kanooth::numbers::sub(num, num, tmp);
    set_this(num, den);
  }

  void _mul_num(const numb& v1, NUM v2) {
    gcd_type g = kanooth::numbers::gcd(v1.denominator, v2);
    kanooth::numbers::div(v2, v2, g);
    kanooth::numbers::mul(numerator, v1.numerator, v2);
    kanooth::numbers::div(denominator, v1.denominator, g);
  }
  
  void _div_num(const numb& v1, DEN v2) {
    gcd_type g = kanooth::numbers::gcd(v1.numerator, v2);
    kanooth::numbers::div(v2, v2, g);
    kanooth::numbers::div(numerator, v1.numerator, g);
    kanooth::numbers::mul(denominator, v1.denominator, v2);
  }

  template <typename T>
  void _div_num(const numb& v1, const T& v) {
    if (kanooth::numbers::is_negative(v))
      kanooth::numbers::negate(numerator, numerator);
    _div_num(v1, DEN(kanooth::numbers::abs(v)));
  }
  
public:
  numb() {
    set_this(0u, 1u);
  }
  template <typename V>
  numb(const V& v) {
    kanooth::numbers::set(*this, v);
  }
  template <typename V>
  numb& operator=(const V& v) {
    kanooth::numbers::set(*this, v);
    return *this;
  }
  inline bool is_zero() const { return kanooth::numbers::is_zero(numerator); }
  inline bool is_positive() const { return kanooth::numbers::is_zero(numerator); }
  inline operator bool() const { return !is_zero(); }

  inline void set(const numb& v) {
    kanooth::numbers::set(numerator, v.numerator);
    kanooth::numbers::set(denominator, v.denominator);
  }
  inline void set(const NUM& n) {
    set_this(n, DEN(1u));
  }
  template <typename T>
  inline void set(const T& v) {
    set_this(NUM(v), DEN(1u));
  }

  const NUM& get_numerator() const {
    return numerator;
  }

  const DEN& get_denominator() const {
    return denominator;
  }

  inline void mul(const numb& v1, const numb& v2) {
    _mul_num(v1, v2.numerator);
    _div_num(*this, v2.denominator);
  }

  template <typename T>
  inline void mul(const numb& v1, const T& v2) {
    _mul_num(v1, v2);
  }

  template <typename T>
  inline void mul(const T& v1, const numb& v2) {
    _mul_num(v2, v1);
  }
  
  inline void div(const numb& v1, const numb& v2) {
    _mul_num(v1, v2.denominator);
    _div_num(*this, v2.numerator);
  }

  template <typename T>
  inline void div(const numb& v1, const T& v2) {
    _div_num(v1, v2);
  }

  template <typename T>
  inline void div(const T& v1, const numb& v2) {
    div(numb(v1), v2);
  }
  
  template <typename T1, typename T2>
  inline void div(const T1& v1, const T2& v2) {
    div(numb(v1), v2);
  }

  inline void add(const numb& v1, const numb& v2) {
    _add_sub(v1.numerator, v1.denominator, v2.numerator, v2.denominator, true);
  }
  
  template <typename T>
  inline void add(const numb& v1, const T& v2) {
    _add_sub(v1.numerator, v1.denominator, NUM(v2), DEN(1u), true);
  }
  
  template <typename T>
  inline void add(const T& v1, const numb& v2) {
    _add_sub(NUM(v1), DEN(1u), v2.numerator, v2.denominator, true);
  }
  
  // Not used for native types
  template <typename T1, typename T2>
  inline void add(const T1& v1, const T2& v2) {
    NUM num;
    kanooth::numbers::add(num, v1, v2);
    set_this(num, DEN(1u));
  }
  
  inline void sub(const numb& v1, const numb& v2) {
    _add_sub(v1.numerator, v1.denominator, v2.numerator, v2.denominator, false);
  }
  
  template <typename T>
  inline void sub(const numb& v1, const T& v2) {
    _add_sub(v1.numerator, v1.denominator, NUM(v2), DEN(1u), false);
  }
  
  template <typename T>
  inline void sub(const T& v1, const numb& v2) {
    _add_sub(NUM(v1), DEN(1u), v2.numerator, v2.denominator, false);
  }
  
  // Not used for native types
  template <typename T1, typename T2>
  inline void sub(const T1& v1, const T2& v2) {
    NUM num;
    kanooth::numbers::sub(num, v1, v2);
    set_this(num, DEN(1u));
  }
  
};

/*
 * a   c   a*d + c*b   a*(d/g3) + c*(b/g3)
 * - + - = --------- = -------------------
 * b   d      b*d           b*(d/g3)
 * 
 * a   c   a*d + c*b   (a/g1)*d + (c/g2)*b   (a/g1)*(d/g3) + (c/g2)*(b/g3)
 * - + - = --------- = ------------------- = -----------------------------
 * b   d      b*d         (b/g2)*(d/g1)            (b/g2)*(d/g1)/g3
 * 
 * g1 = gcd(a, d)
 * g2 = gcd(b, c)
 * g3 = gcd(b/g2, d/g1)
 */

} // namespace detail
} // namespace kanooth
} // namespace numbers

#endif // _KANOOTH_NUMBERS_DETAIL_RATIONAL_NUMBER_IMPL_HPP
