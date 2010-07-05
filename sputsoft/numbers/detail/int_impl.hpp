/* 
 * File:   sputsoft/numbers/detail/int_impl.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-04 20:12Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_INT_IMPL_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_INT_IMPL_HPP

#include <sputsoft/detail/types.hpp>
#include <sputsoft/numbers/detail/int_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename NUM>
class numb<intnum<NUM> > {
  template <typename NUM2>
  friend std::ostream& operator<<(std::ostream& os, const numb<intnum<NUM2> >& n);
private:
  NUM num;
  bool positive;

  inline void set_num(const NUM& n, bool p) {
    sputsoft::numbers::set(num, n);
    positive = p;
  }

  template <typename T>
  void set_int(T v) {
    positive = v >= 0;
    sputsoft::numbers::set(num, sputsoft::to_unsigned(positive ? v : -v));
  }

  template <typename T>
  void _add(const NUM& n1, bool p1, const T& n2, bool p2) {
    if (p1 == p2) {
      sputsoft::numbers::add(num, n1, n2);
      positive = p1;
    } else if (sputsoft::numbers::compare(n1, n2) >= 0) {
      sputsoft::numbers::sub(num, n1, n2);
      positive = p1;
    } else {
      sputsoft::numbers::sub(num, n2, n1);
      positive = p2;
    }
  }

  template <typename T>
  inline void add_signed_int(const NUM& n1, bool p1, T v2) {
    bool p2 = v2 >= 0;
    _add(n1, p1, sputsoft::to_unsigned(p2 ? v2 : -v2), p2);
  }

  template <typename T>
  inline void _mul(const NUM& n1, bool p1, const T& n2, bool p2) {
    sputsoft::numbers::mul(num, n1, n2);
    positive = p1 == p2;
  }

  template <typename T>
  inline void mul_signed_int(const NUM& n1, bool p1, T v2) {
    bool p2 = v2 >= 0;
    _mul(n1, p1, sputsoft::to_unsigned(p2 ? v2 : -v2), p2);
  }

  /* quotrem, div, rem floor */

  static void quotrem_floor_num(numb& q, numb& r, const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::quotrem(q.num, r.num, v1, v2);
    if (p1 != p2 && r.num) {
      sputsoft::numbers::add(q.num, q.num, 1u);
      sputsoft::numbers::sub(r.num, v2, r.num);
    }
    q.positive = p1 == p2;
    r.positive = p2;
  }

  void div_floor_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    positive = p1 == p2;
    if (!positive) {
      NUM rnum;
      sputsoft::numbers::quotrem(num, rnum, v1, v2);
      if (rnum)
        sputsoft::numbers::add(num, num, 1u);
    } else
      sputsoft::numbers::div(num, v1, v2);
  }

  void rem_floor_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::rem(num, v1, v2);
    positive = p2;
    if (p1 != p2 && num)
      sputsoft::numbers::sub(num, v2, num);
  }

  template <typename T>
  T quotrem_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::quotrem(num, v1, v2u);
    positive = p1 == p2;
    if (!positive && ru) {
      sputsoft::numbers::add(num, num, 1u);
      ru = v2u - ru;
    }
    return (T) (p2 ? ru : -ru);
  }

  template <typename T>
  void div_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    positive = p1 == p2;
    if (!positive) {
      unsigned_type ru = sputsoft::numbers::quotrem(num, v1, v2u);
      if (ru)
        sputsoft::numbers::add(num, num, 1u);
    } else
      sputsoft::numbers::div(num, v1, v2u);
  }

  template <typename T>
  static T rem_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::rem(v1, v2u);
    if (p1 != p2 && ru) ru = v2u - ru;
    return (T) (p2 ? ru : -ru);
  }

  /* quotrem, div, rem ceil */

  static void quotrem_ceil_num(numb& q, numb& r, const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::quotrem(q.num, r.num, v1, v2);
    if (p1 == p2 && r.num) {
      sputsoft::numbers::add(q.num, q.num, 1u);
      sputsoft::numbers::sub(r.num, v2, r.num);
    }
    q.positive = p1 == p2;
    r.positive = !p2;
  }

  void div_ceil_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    positive = p1 == p2;
    if (positive) {
      NUM rnum;
      sputsoft::numbers::quotrem(num, rnum, v1, v2);
      if (rnum)
        sputsoft::numbers::add(num, num, 1u);
    } else
      sputsoft::numbers::div(num, v1, v2);
  }

  void rem_ceil_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::rem(num, v1, v2);
    positive = !p2;
    if (p1 == p2 && num)
      sputsoft::numbers::sub(num, v2, num);
  }

  template <typename T>
  T quotrem_ceil_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::quotrem(num, v1, v2u);
    positive = p1 == p2;
    if (positive && ru) {
      sputsoft::numbers::add(num, num, 1u);
      ru = v2u - ru;
    }
    return (T) (p2 ? -ru : ru);
  }

  template <typename T>
  void div_ceil_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type r = sputsoft::numbers::quotrem(num, v1, (unsigned_type) (p2 ? v2 : -v2));
    positive = p1 == p2;
    if (positive && r)
      sputsoft::numbers::add(num, num, 1u);
  }

  template <typename T>
  static T rem_ceil_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::rem(v1, v2u);
    if (p1 == p2 && ru)
      ru = v2u - ru;
    return (T) (p2 ? -ru : ru);
  }

  /* quotrem, div, rem trunc */

  static void quotrem_trunc_num(numb& q, numb& r, const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::quotrem(q.num, r.num, v1, v2);
    q.positive = p1 == p2;
    r.positive = p1;
  }

  void div_trunc_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::div(num, v1, v2);
    positive = p1 == p2;
  }

  void rem_trunc_num(const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::rem(num, v1, v2);
    positive = p1;
  }

  template <typename T>
  T quotrem_trunc_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type ru = sputsoft::numbers::quotrem(num, v1, (unsigned_type) (p2 ? v2 : -v2));
    positive = p1 == p2;
    return (T) (p1 ? ru : -ru);
  }

  template <typename T>
  void div_trunc_int(const NUM& v1, bool p1, T v2) {
    bool p2 = v2 >= 0;
    sputsoft::numbers::div(num, v1, sputsoft::to_unsigned(p2 ? v2 : -v2));
    positive = p1 == p2;
  }

  template <typename T>
  static T rem_trunc_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type ru = sputsoft::numbers::rem(v1, (unsigned_type) (p2 ? v2 : -v2));
    return (T) (p1 ? ru : -ru);
  }

  /* comparisons */

  template <typename T>
  static int _cmp(const NUM& v1, bool p1, const T& v2, bool p2) {
    if (!v1)
      return !v2 ? 0 : p2 ? -1 : 1;
    if (!v2 || p1 != p2)
      return p1 ? 1 : -1;
    int c = sputsoft::numbers::compare(v1, v2);
    return p1 ? c : -c;
  }

  template <typename T>
  static int cmp_int(const NUM& v1, bool p1, const T& v2) {
    bool p2 = v2 >= 0;
    return _cmp(v1, p1, sputsoft::to_unsigned(p2 ? v2 : -v2), p2);
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
  numb(const NUM& n, bool pos=true) : num(n), positive(pos) {}
  inline void set(const numb& v) { set_num(v.num, v.positive); }
  inline void set(const NUM& n) { set_num(n, true); }
  inline void negate(const numb& v) { set_num(v.num, !v.positive); }
  inline bool is_zero() const { return !num; }
  inline bool is_positive() const { return num && positive; }
  inline bool is_negative() const { return num && !positive; }
  inline operator bool() const { return !is_zero(); }

  inline void set(unsigned short v) { set_int(v); }
  inline void add(const numb& v1, unsigned short v2) { _add(v1.num, v1.positive, v2, true); }
  inline void add(unsigned short v1, const numb& v2) { _add(v2.num, v2.positive, v1, true); }
  inline void sub(const numb& v1, unsigned short v2) { _add(v1.num, v1.positive, v2, false); }
  inline void sub(unsigned short v1, const numb& v2) { _add(v2.num, v2.positive, v1, false); }
  inline void mul(const numb& v1, unsigned short v2) { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(unsigned short v1, const numb& v2) { _mul(v2.num, v2.positive, v1, true); }
  inline void div_floor(const numb& v1, unsigned short v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(unsigned short& r, const numb& v1, unsigned short v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, unsigned short& r, const numb& v1, unsigned short v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, unsigned short v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(unsigned short& r, const numb& v1, unsigned short v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, unsigned short& r, const numb& v1, unsigned short v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, unsigned short v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(unsigned short& r, const numb& v1, unsigned short v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, unsigned short& r, const numb& v1, unsigned short v2)
    { r = quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(unsigned short v) const { return cmp_int(num, positive, v); }

  inline void set(signed short v) { set_int(v); }
  inline void add(const numb& v1, signed short v2) { add_signed_int(v1.num, v1.positive, v2); }
  inline void add(signed short v1, const numb& v2) { add_signed_int(v2.num, v2.positive, v1); }
  inline void sub(const numb& v1, signed short v2) { add_signed_int(v1.num, v1.positive, -v2); }
  inline void sub(signed short v1, const numb& v2) { add_signed_int(v2.num, !v2.positive, v1); }
  inline void mul(const numb& v1, signed short v2) { mul_signed_int(v1.num, v1.positive, v2); }
  inline void mul(signed short v1, const numb& v2) { mul_signed_int(v2.num, v2.positive, v1); }
  inline void div_floor(const numb& v1, signed short v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(signed short& r, const numb& v1, signed short v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, signed short& r, const numb& v1, signed short v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, signed short v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(signed short& r, const numb& v1, signed short v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, signed short& r, const numb& v1, signed short v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, signed short v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(signed short& r, const numb& v1, signed short v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, signed short& r, const numb& v1, signed short v2)
    { r = q.quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(signed short v) const { return cmp_int(num, positive, v); }

  inline void set(unsigned v) { set_int(v); }
  inline void add(const numb& v1, unsigned v2) { _add(v1.num, v1.positive, v2, true); }
  inline void add(unsigned v1, const numb& v2) { _add(v2.num, v2.positive, v1, true); }
  inline void sub(const numb& v1, unsigned v2) { _add(v1.num, v1.positive, v2, false); }
  inline void sub(unsigned v1, const numb& v2) { _add(v2.num, v2.positive, v1, false); }
  inline void mul(const numb& v1, unsigned v2) { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(unsigned v1, const numb& v2) { _mul(v2.num, v2.positive, v1, true); }
  inline void div_floor(const numb& v1, unsigned v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(unsigned& r, const numb& v1, unsigned v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, unsigned& r, const numb& v1, unsigned v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, unsigned v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(unsigned& r, const numb& v1, unsigned v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, unsigned& r, const numb& v1, unsigned v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, unsigned v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(unsigned& r, const numb& v1, unsigned v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, unsigned& r, const numb& v1, unsigned v2)
    { r = quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(unsigned v) const { return cmp_int(num, positive, v); }

  inline void set(signed v) { set_int(v); }
  inline void add(const numb& v1, signed v2) { add_signed_int(v1.num, v1.positive, v2); }
  inline void add(signed v1, const numb& v2) { add_signed_int(v2.num, v2.positive, v1); }
  inline void sub(const numb& v1, signed v2) { add_signed_int(v1.num, v1.positive, -v2); }
  inline void sub(signed v1, const numb& v2) { add_signed_int(v2.num, !v2.positive, v1); }
  inline void mul(const numb& v1, signed v2) { mul_signed_int(v1.num, v1.positive, v2); }
  inline void mul(signed v1, const numb& v2) { mul_signed_int(v2.num, v2.positive, v1); }
  inline void div_floor(const numb& v1, signed v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(signed& r, const numb& v1, signed v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, signed& r, const numb& v1, signed v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, signed v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(signed& r, const numb& v1, signed v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, signed& r, const numb& v1, signed v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, signed v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(signed& r, const numb& v1, signed v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, signed& r, const numb& v1, signed v2)
    { r = q.quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(signed v) const { return cmp_int(num, positive, v); }

  inline void set(unsigned long v) { set_int(v); }
  inline void add(const numb& v1, unsigned long v2) { _add(v1.num, v1.positive, v2, true); }
  inline void add(unsigned long v1, const numb& v2) { _add(v2.num, v2.positive, v1, true); }
  inline void sub(const numb& v1, unsigned long v2) { _add(v1.num, v1.positive, v2, false); }
  inline void sub(unsigned long v1, const numb& v2) { _add(v2.num, v2.positive, v1, false); }
  inline void mul(const numb& v1, unsigned long v2) { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(unsigned long v1, const numb& v2) { _mul(v2.num, v2.positive, v1, true); }
  inline void div_floor(const numb& v1, unsigned long v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(unsigned long& r, const numb& v1, unsigned long v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, unsigned long& r, const numb& v1, unsigned long v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, unsigned long v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(unsigned long& r, const numb& v1, unsigned long v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, unsigned long& r, const numb& v1, unsigned long v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, unsigned long v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(unsigned long& r, const numb& v1, unsigned long v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, unsigned long& r, const numb& v1, unsigned long v2)
    { r = quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(unsigned long v) const { return cmp_int(num, positive, v); }

  inline void set(signed long v) { set_int(v); }
  inline void add(const numb& v1, signed long v2) { add_signed_int(v1.num, v1.positive, v2); }
  inline void add(signed long v1, const numb& v2) { add_signed_int(v2.num, v2.positive, v1); }
  inline void sub(const numb& v1, signed long v2) { add_signed_int(v1.num, v1.positive, -v2); }
  inline void sub(signed long v1, const numb& v2) { add_signed_int(v2.num, !v2.positive, v1); }
  inline void mul(const numb& v1, signed long v2) { mul_signed_int(v1.num, v1.positive, v2); }
  inline void mul(signed long v1, const numb& v2) { mul_signed_int(v2.num, v2.positive, v1); }
  inline void div_floor(const numb& v1, signed long v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(signed long& r, const numb& v1, signed long v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, signed long& r, const numb& v1, signed long v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, signed long v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(signed long& r, const numb& v1, signed long v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, signed long& r, const numb& v1, signed long v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, signed long v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(signed long& r, const numb& v1, signed long v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, signed long& r, const numb& v1, signed long v2)
    { r = q.quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(signed long v) const { return cmp_int(num, positive, v); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  inline void set(unsigned long long v) { set_int(v); }
  inline void add(const numb& v1, unsigned long long v2) { _add(v1.num, v1.positive, v2, true); }
  inline void add(unsigned long long v1, const numb& v2) { _add(v2.num, v2.positive, v1, true); }
  inline void sub(const numb& v1, unsigned long long v2) { _add(v1.num, v1.positive, v2, false); }
  inline void sub(unsigned long long v1, const numb& v2) { _add(v2.num, v2.positive, v1, false); }
  inline void mul(const numb& v1, unsigned long long v2) { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(unsigned long long v1, const numb& v2) { _mul(v2.num, v2.positive, v1, true); }
  inline void div_floor(const numb& v1, unsigned long long v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(unsigned long long& r, const numb& v1, unsigned long long v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, unsigned long long& r, const numb& v1, unsigned long long v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, unsigned long long v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(unsigned long long& r, const numb& v1, unsigned long long v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, unsigned long long& r, const numb& v1, unsigned long long v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, unsigned long long v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(unsigned long long& r, const numb& v1, unsigned long long v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, unsigned long long& r, const numb& v1, unsigned long long v2)
    { r = quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(unsigned long long v) const { return cmp_int(num, positive, v); }

  inline void set(signed long long v) { set_int(v); }
  inline void add(const numb& v1, signed long long v2) { add_signed_int(v1.num, v1.positive, v2); }
  inline void add(signed long long v1, const numb& v2) { add_signed_int(v2.num, v2.positive, v1); }
  inline void sub(const numb& v1, signed long long v2) { add_signed_int(v1.num, v1.positive, -v2); }
  inline void sub(signed long long v1, const numb& v2) { add_signed_int(v2.num, !v2.positive, v1); }
  inline void mul(const numb& v1, signed long long v2) { mul_signed_int(v1.num, v1.positive, v2); }
  inline void mul(signed long long v1, const numb& v2) { mul_signed_int(v2.num, v2.positive, v1); }
  inline void div_floor(const numb& v1, signed long long v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline void rem_floor(signed long long& r, const numb& v1, signed long long v2)
    { r = rem_floor_int(v1.num, v1.positive, v2); }
  static inline void quotrem_floor(numb& q, signed long long& r, const numb& v1, signed long long v2)
    { r = q.quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const numb& v1, signed long long v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline void rem_ceil(signed long long& r, const numb& v1, signed long long v2)
    { r = rem_ceil_int(v1.num, v1.positive, v2); }
  static inline void quotrem_ceil(numb& q, signed long long& r, const numb& v1, signed long long v2)
    { r = q.quotrem_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const numb& v1, signed long long v2) { div_trunc_int(v1.num, v1.positive, v2); }
  static inline void rem_trunc(signed long long& r, const numb& v1, signed long long v2)
    { r = rem_trunc_int(v1.num, v1.positive, v2); }
  static inline void quotrem_trunc(numb& q, signed long long& r, const numb& v1, signed long long v2)
    { r = q.quotrem_trunc_int(v1.num, v1.positive, v2); }
  inline signed cmp(signed long long v) const { return cmp_int(num, positive, v); }
#endif

  inline void add(const numb& v1, const numb& v2) { _add(v1.num, v1.positive, v2.num, v2.positive); }
  inline void add(const numb& v1, const NUM& v2)  { _add(v1.num, v1.positive, v2, true); }
  inline void add(const NUM& v1,  const numb& v2) { _add(v1, true, v2.num, v2.positive); }
  inline void add(const NUM& v1,  const NUM& v2)  { _add(v1, true, v2, true); }
  inline void sub(const numb& v1, const numb& v2) { _add(v1.num, v1.positive, v2.num, !v2.positive); }
  inline void sub(const numb& v1, const NUM& v2)  { _add(v1.num, v1.positive, v2, false); }
  inline void sub(const NUM& v1,  const numb& v2) { _add(v1, true, v2.num, !v2.positive); }
  inline void sub(const NUM& v1,  const NUM& v2)  { _add(v1, true, v2, false); }
  inline void mul(const numb& v1, const numb& v2) { _mul(v1.num, v1.positive, v2.num, v2.positive); }
  inline void mul(const numb& v1, const NUM& v2)  { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(const NUM& v1,  const numb& v2) { _mul(v1, true, v2.num, v2.positive); }
  inline void mul(const NUM& v1,  const NUM& v2)  { _mul(v1, true, v2, true); }
  inline void div_floor(const numb& u, const numb& v)
    { div_floor_num(u.num, u.positive, v.num, v.positive); }
  static inline void rem_floor(numb& r, const numb& u, const numb& v)
    { r.rem_floor_num(u.num, u.positive, v.num, v.positive); }
  static inline void quotrem_floor(numb& q, numb& r, const numb& u, const numb& v)
    { quotrem_floor_num(q, r, u.num, u.positive, v.num, v.positive); }
  inline void div_ceil(const numb& u, const numb& v)
    { div_ceil_num(u.num, u.positive, v.num, v.positive); }
  static inline void rem_ceil(numb& r, const numb& u, const numb& v)
    { r.rem_ceil_num(u.num, u.positive, v.num, v.positive); }
  static inline void quotrem_ceil(numb& q, numb& r, const numb& u, const numb& v)
    { quotrem_ceil_num(q, r, u.num, u.positive, v.num, v.positive); }
  inline void div_trunc(const numb& u, const numb& v)
    { div_trunc_num(u.num, u.positive, v.num, v.positive); }
  static inline void rem_trunc(numb& r, const numb& u, const numb& v)
    { r.rem_trunc_num(u.num, u.positive, v.num, v.positive); }
  static inline void quotrem_trunc(numb& q, numb& r, const numb& u, const numb& v)
    { quotrem_trunc_num(q, r, u.num, u.positive, v.num, v.positive); }
  inline signed cmp(const numb& v) const { return _cmp(num, positive, v.num, v.positive); }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_INT_IMPL_HPP