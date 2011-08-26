/* 
 * File:   sputsoft/numbers/detail/pos_float_impl.hpp
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

#ifndef _SPUTSOFT_NUMBERS_DETAIL_POS_FLOAT_IMPL_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_POS_FLOAT_IMPL_HPP

#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/detail/pos_float_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

std::size_t ceil_multiple(std::size_t x, unsigned a)
{
  return ((x + a - 1)/a)*a;
}
  
template <typename NUM, typename EXP>
class numb<posfloatnum<NUM, EXP> > {
private:
  NUM num;
  EXP exponent;

  void normalize() {
    if (sputsoft::numbers::is_zero(num)) return;
    std::size_t bits_used = floor_log2(num) + 1;
    std::size_t to_use = ceil_multiple(bits_used, NUM::digit_bits);
    std::ptrdiff_t shift = to_use - bits_used;
    bit_shift_left(num, num, shift);
    exponent -= shift;
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
  numb() {}
  template <typename V>
  numb(const V& v) {
    sputsoft::numbers::set(*this, v);
  }
  template <typename V>
  numb& operator=(const V& v) {
    sputsoft::numbers::set(*this, v);
    return *this;
  }
  numb(const NUM& n) { set_num(n, 0); }
  inline bool is_zero() const { return !num; }
  inline bool is_positive() const { return num; }
  inline operator bool() const { return !is_zero(); }

  inline void set(const numb& v) { set_num(v.num, v.exponent); }
  inline void set(const NUM& n) { set_num(n, 0); }
  template <typename T>
  inline void set(T v) { set_int(v); }

  static void trunc(NUM& x, const numb& v) {
    sputsoft::numbers::bit_shift_left(x, v.num, v.exponent);
  }

  void mul(const numb& x, const numb& y) {
    sputsoft::numbers::mul(num, x.num, y.num);
    exponent = x.exponent + y.exponent;
    normalize();
  }
  
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_POS_FLOAT_IMPL_HPP
