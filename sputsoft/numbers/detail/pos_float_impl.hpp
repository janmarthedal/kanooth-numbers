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

template <typename NUM, typename EXP>
class numb<posfloatnum<NUM, EXP> > {
private:
  NUM num;
  EXP exponent;

  inline void set_num(const NUM& n, EXP e) {
    sputsoft::numbers::set(num, n);
    exponent = e;
  }

  template <typename T>
  void set_int(T v) {
    sputsoft::numbers::set(num, v);
    exponent = 0;
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
  numb(const NUM& n) : num(n), exponent(0) {}
  inline bool is_zero() const { return !num; }
  inline bool is_positive() const { return num; }
  inline operator bool() const { return !is_zero(); }

  inline void set(const numb& v) { set_num(v.num, v.exponent); }
  inline void set(const NUM& n) { set_num(n, 0); }
  template <typename T>
  inline void set(T v) { set_int(v); }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_POS_FLOAT_IMPL_HPP
