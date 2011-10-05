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
    gcd_type g1 = kanooth::numbers::gcd(v1.numerator, v2.denominator);
    gcd_type g2 = kanooth::numbers::gcd(v2.numerator, v1.denominator);
    kanooth::numbers::div(numerator, v1.numerator, g1);
    kanooth::numbers::div(denominator, v1.denominator, g2);
    kanooth::numbers::mul(numerator, numerator, kanooth::numbers::div(v2.numerator, g2));
    kanooth::numbers::mul(denominator, denominator, kanooth::numbers::div(v2.denominator, g1));
  }

  inline void div(const numb& v1, const numb& v2) {
    gcd_type g1 = kanooth::numbers::gcd(v1.numerator, v2.numerator);
    gcd_type g2 = kanooth::numbers::gcd(v2.denominator, v1.denominator);
    kanooth::numbers::div(numerator, v1.numerator, g1);
    kanooth::numbers::div(denominator, v1.denominator, g2);
    if (kanooth::numbers::is_negative(v2.numerator))
      kanooth::numbers::negate(numerator, numerator);
    kanooth::numbers::mul(numerator, numerator, kanooth::numbers::div(v2.denominator, g2));
    kanooth::numbers::mul(denominator, denominator, kanooth::numbers::div(kanooth::numbers::abs(v2.numerator), g1));
  }

};

} // namespace detail
} // namespace kanooth
} // namespace numbers

#endif // _KANOOTH_NUMBERS_DETAIL_RATIONAL_NUMBER_IMPL_HPP
