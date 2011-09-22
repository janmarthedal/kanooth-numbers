/* 
 * File:   kanooth/numbers/detail/integer/int_impl.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-04 20:12Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_INT_IMPL_HPP
#define _KANOOTH_NUMBERS_DETAIL_INT_IMPL_HPP

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/detail/integer/int_abst.hpp>

/*
 Integer division cases
 (trunc is default for C++ signed integers)
 a  b  trunc  floor   ceil
 7  3   2, 1   2, 1   3,-2
-7  3  -2,-1  -3, 2  -2,-1
 7 -3  -2, 1  -3,-2  -2, 1
-7 -3   2,-1   2,-1   3, 2
*/

namespace kanooth {
namespace numbers {
namespace detail {

template <typename NUM>
class numb<intnum<NUM> > {
private:
  NUM num;
  bool positive;

  inline void set_num(const NUM& n, bool p) {
    kanooth::numbers::set(num, n);
    positive = p;
  }

  template <typename T>
  void set_int(T v) {
    positive = v >= 0;
    kanooth::numbers::set(num, kanooth::to_unsigned(positive ? v : -v));
  }

  template <typename T1, typename T2>
  void _add(const T1& n1, bool p1, const T2& n2, bool p2) {
    if (p1 == p2) {
      kanooth::numbers::add(num, n1, n2);
      positive = p1;
    } else if (kanooth::numbers::is_greater_or_equal(n1, n2)) {
      kanooth::numbers::sub(num, n1, n2);
      positive = p1;
    } else {
      kanooth::numbers::sub(num, n2, n1);
      positive = p2;
    }
  }

  template <typename T1, typename T2>
  inline void _mul(const T1& n1, bool p1, const T2& n2, bool p2) {
    kanooth::numbers::mul(num, n1, n2);
    positive = p1 == p2;
  }

  /* quotrem, div, rem floor */

  static void quotrem_floor_num(numb& q, numb& r, const NUM& v1, bool p1, const NUM& v2, bool p2) {
    kanooth::numbers::divrem(q.num, r.num, v1, v2);
    if (p1 != p2 && r.num) {
      kanooth::numbers::add(q.num, q.num, 1u);
      kanooth::numbers::sub(r.num, v2, r.num);
    }
    q.positive = p1 == p2;
    r.positive = p2;
  }

  void div_floor_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    positive = p1 == p2;
    if (!positive) {
      NUM rnum;
      kanooth::numbers::divrem(num, rnum, v1, v2);
      if (rnum)
        kanooth::numbers::add(num, num, 1u);
    } else
      kanooth::numbers::div(num, v1, v2);
  }

  void rem_floor_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    kanooth::numbers::rem(num, v1, v2);
    positive = p2;
    if (p1 != p2 && num)
      kanooth::numbers::sub(num, v2, num);
  }

  template <typename T>
  T quotrem_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename kanooth::make_unsigned<T>::type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = kanooth::numbers::divrem(num, v1, v2u);
    positive = p1 == p2;
    if (!positive && ru) {
      kanooth::numbers::add(num, num, 1u);
      ru = v2u - ru;
    }
    return (T) (p2 ? ru : -ru);
  }

  template <typename T>
  void div_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename kanooth::make_unsigned<T>::type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    positive = p1 == p2;
    if (!positive) {
      unsigned_type ru = kanooth::numbers::divrem(num, v1, v2u);
      if (ru)
        kanooth::numbers::add(num, num, 1u);
    } else
      kanooth::numbers::div(num, v1, v2u);
  }

  template <typename T>
  static T rem_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename kanooth::make_unsigned<T>::type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = kanooth::numbers::rem(v1, v2u);
    if (p1 != p2 && ru) ru = v2u - ru;
    return (T) (p2 ? ru : -ru);
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

  template <typename T>
  static int cmp_int(const NUM& v1, bool p1, const T& v2) {
    bool p2 = v2 >= 0;
    return _cmp(v1, p1, kanooth::to_unsigned(p2 ? v2 : -v2), p2);
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
  numb(const NUM& n, bool pos=true) : num(n), positive(pos) {}
  inline void negate(const numb& v) { set_num(v.num, !v.positive); }
  inline bool is_zero() const { return !num; }
  inline bool is_positive() const { return num && positive; }
  inline bool is_negative() const { return num && !positive; }
  inline operator bool() const { return !is_zero(); }
  static inline void abs(NUM& r, const numb& v) {
    kanooth::numbers::set(r, v.num);
  }
  inline void abs(const numb& v) {
    kanooth::numbers::set(num, v.num);
    positive = false;
  }

  inline void set(const numb& v) { set_num(v.num, v.positive); }
  inline void set(const NUM& n) { set_num(n, true); }
  template <typename T>
  inline void set(T v) { set_int(v); }

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

  inline void div(const numb& u, const numb& v) {
    div_floor_num(u.num, u.positive, v.num, v.positive);
  }
  template <typename V>
  inline void div(const numb& u, V v) {
    div_floor_int(u.num, u.positive, v);
  }

  inline void rem(const numb& u, const numb& v) {
    rem_floor_num(u.num, u.positive, v.num, v.positive);
  }
  template <typename T>
  static inline T rem(const numb& v1, T v2) {
    return rem_floor_int(v1.num, v1.positive, v2);
  }

  static inline void divrem(numb& q, numb& r, const numb& u, const numb& v) {
    quotrem_floor_num(q, r, u.num, u.positive, v.num, v.positive);
  }
  template <typename T>
  static inline T divrem(numb& q, const numb& v1, T v2) {
    return q.quotrem_floor_int(v1.num, v1.positive, v2);
  }

  template <typename T>
  inline signed cmp(const T& v) const {
    return _cmp(num, positive, kanooth::numbers::abs(v), !kanooth::numbers::is_negative(v));
  }

  template <typename V1, typename V2>
  inline void bitwise_and_pos(const V1& v1, const V2& v2) {
    kanooth::numbers::bitwise_and(num, kanooth::numbers::abs(v1), kanooth::numbers::abs(v2));
    positive = true;
  }
  template <typename V1, typename V2>
  inline void bitwise_or_pos(const V1& v1, const V2& v2) {
    kanooth::numbers::bitwise_or(num, kanooth::numbers::abs(v1), kanooth::numbers::abs(v2));
    positive = true;
  }
  template <typename V1, typename V2>
  inline void bitwise_xor_pos(const V1& v1, const V2& v2) {
    kanooth::numbers::bitwise_xor(num, kanooth::numbers::abs(v1), kanooth::numbers::abs(v2));
    positive = true;
  }
  template <typename V1, typename V2>
  inline void bitwise_and_not_pos(const V1& v1, const V2& v2) {
    kanooth::numbers::bitwise_and_not(num, kanooth::numbers::abs(v1), kanooth::numbers::abs(v2));
    positive = true;
  }
};

} // namespace detail
} // namespace kanooth
} // namespace numbers

#endif // _KANOOTH_NUMBERS_DETAIL_INT_IMPL_HPP
