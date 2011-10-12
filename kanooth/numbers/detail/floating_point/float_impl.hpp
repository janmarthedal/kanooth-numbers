/*
 * File:   kanooth/numbers/detail/floating_point/float_impl.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-10-12 11:57Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_FLOAT_IMPL_HPP
#define _KANOOTH_NUMBERS_DETAIL_FLOAT_IMPL_HPP

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/detail/floating_point/float_abst.hpp>

namespace kanooth {
namespace numbers {
namespace detail {

template <typename NUM>
class numb<floatnum<NUM> > {
private:
  NUM num;
  bool positive;

  template <typename T>
  void set_num(const T& v) {
    positive = !kanooth::numbers::is_negative(v);
    kanooth::numbers::set(num, kanooth::numbers::abs(v));
  }

  template <typename T1, typename T2>
  void _add(const T1& n1, bool p1, const T2& n2, bool p2) {
    if (p1 == p2) {
      positive = p1;
      kanooth::numbers::add(num, n1, n2);
    } else if (kanooth::numbers::is_greater_or_equal(n1, n2)) {
      positive = p1;
      kanooth::numbers::sub(num, n1, n2);
    } else {
      positive = p2;
      kanooth::numbers::sub(num, n2, n1);
    }
  }

  template <typename T1, typename T2>
  inline void _mul(const T1& n1, bool p1, const T2& n2, bool p2) {
    positive = p1 == p2;
    kanooth::numbers::mul(num, n1, n2);
  }

  /* comparisons */

  template <typename T>
  static int _cmp(const NUM& v1, bool p1, const T& v2, bool p2) {
    if (!v1)
      return !v2 ? 0 : p2 ? -1 : 1;
    if (!v2 || p1 != p2)
      return p1 ? 1 : -1;
    int c = kanooth::numbers::compare(v1, v2);
    return p1 ? c : -c;
  }

public:
  numb() {}

  template <typename V>
  numb(const V& v) {
    kanooth::numbers::set(*this, v);
  }

  template <typename V>
  numb& operator=(const V& v) {
    kanooth::numbers::set(*this, v);
    return *this;
  }

  numb(const NUM& n, bool pos=true) : NUM(n), positive(pos) {}

  inline void set(const numb& v) {
    kanooth::numbers::set(num, v.num);
    positive = v.positive;
  }
  inline void set(const NUM& n) {
    kanooth::numbers::set(num, n);
    positive = true;
  }
  template <typename T>
  inline void set(T v) {
    kanooth::numbers::set(num, kanooth::numbers::abs(v));
    positive = !kanooth::numbers::is_negative(v);
  }

  inline void negate(const numb& v) {
    kanooth::numbers::set(num, v.num);
    positive = !v.positive;
  }
  template <typename T>
  inline void negate(const T& v) {
    kanooth::numbers::set(num, kanooth::numbers::abs(v));
    positive = kanooth::numbers::is_negative(v);
  }
  inline bool is_zero() const {
    return !num;
  }
  inline bool is_positive() const {
    return num && positive;
  }
  inline bool is_negative() const {
    return num && !positive;
  }
  inline operator bool() const {
    return !is_zero();
  }
  static inline void abs(NUM& r, const numb& v) {
    kanooth::numbers::set(r, v.num);
  }
  inline void abs(const numb& v) {
    set_num(v.num);
  }

  template <typename V1, typename V2>
  inline void add(const V1& v1, const V2& v2) {
    _add(kanooth::numbers::abs(v1), !kanooth::numbers::is_negative(v1),
         kanooth::numbers::abs(v2), !kanooth::numbers::is_negative(v2));
  }

  template <typename V1, typename V2>
  inline void sub(const V1& v1, const V2& v2) {
    _add(kanooth::numbers::abs(v1), !kanooth::numbers::is_negative(v1),
         kanooth::numbers::abs(v2), kanooth::numbers::is_negative(v2));
  }

  template <typename V1, typename V2>
  inline void mul(const V1& v1, const V2& v2) {
    _mul(kanooth::numbers::abs(v1), !kanooth::numbers::is_negative(v1),
         kanooth::numbers::abs(v2), !kanooth::numbers::is_negative(v2));
  }

  template <typename T>
  inline signed cmp(const T& v) const {
    return _cmp(num, positive, kanooth::numbers::abs(v), !kanooth::numbers::is_negative(v));
  }

  template <typename V1, typename V2>
  inline void div(const V1& v1, const V2& v2) {
  }

};

} // namespace detail
} // namespace kanooth
} // namespace numbers

#endif // _KANOOTH_NUMBERS_DETAIL_FLOAT_IMPL_HPP
