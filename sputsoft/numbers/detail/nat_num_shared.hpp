/* 
 * File:   sputsoft/numbers/detail/nat_num_shared.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-04-30 15:18Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_SHARED_HPP
#define	_SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_SHARED_HPP

#include <sputsoft/numbers/detail/nat_num_impl.hpp>
#include <boost/shared_ptr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Con, typename LowLevel>
class numb<natnum<boost::shared_ptr<wrap2<Con, LowLevel> > > > {
private:
  typedef numb<natnum<wrap2<Con, LowLevel> > > number_type;
  boost::shared_ptr<number_type> num;

  template <typename T>
  void _set(T v) {
    if (!num.unique()) num.reset(new number_type);
    num->set(v);
  }

  template <typename V2>
  void _add(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->add(v1, v2);
  }

  template <typename V1, typename V2>
  void _sub(const V1& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->sub(v1, v2);
  }

  template <typename V2>
  void _mul(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->mul(v1, v2);
  }

  template <typename V2>
  void _div(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->div(v1, v2);
  }

  template <typename T>
  static void rem_int(T& r, const number_type& v1, const T& v2) {
    number_type::rem(r, v1, v2);
  }

  template <typename T>
  static void quotrem_int(numb& q, T& r, const number_type& v1, const T v2) {
    if (!q.num.unique()) q.num.reset(new number_type);
    number_type::quotrem(*q.num, r, v1, v2);
  }

  void rem_num(const number_type& x, const number_type& y) {
    if (!num.unique()) num.reset(new number_type);
    number_type::rem(*num, x, y);
  }

  static void quotrem_num(numb& q, numb& r, const number_type& u, const number_type& v) {
    if (!q.num.unique()) q.num.reset(new number_type);
    if (!r.num.unique()) r.num.reset(new number_type);
    number_type::quotrem(*q.num, *r.num, u, v);
  }

  void bit_and_num(const number_type& x, const number_type& y) {
    if (!num.unique()) num.reset(new number_type);
    number_type::bitwise_and(*num, x, y);
  }

  void left_shift_num(const number_type& x, std::ptrdiff_t count) {
    if (!num.unique()) num.reset(new number_type);
    num->left_shift(x, count);
  }

  void right_shift_num(const number_type& x, std::ptrdiff_t count) {
    if (!num.unique()) num.reset(new number_type);
    num->right_shift(x, count);
  }

  template <typename V2>
  void bit_and(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->bitwise_and(v1, v2);
  }

  template <typename V2>
  void bit_or(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->bitwise_or(v1, v2);
  }

  template <typename V2>
  void bit_xor(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->bitwise_xor(v1, v2);
  }

  template <typename V2>
  void bit_and_not(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->bitwise_and_not(v1, v2);
  }

public:
  numb() : num(new number_type) {}
  template <typename V>
  numb(const V& v) : num(new number_type) {
    sputsoft::numbers::set(*this, v);
  }
  template <typename V>
  numb& operator=(const V& v) {
    sputsoft::numbers::set(*this, v);
    return *this;
  }
  inline bool is_zero() const { return num->is_zero(); }
  inline operator bool() const { return !is_zero(); }
  std::size_t log2_floor() const { return num->log2_floor(); }

  inline void set(unsigned short v) { _set(v); }
  inline void add(const numb& x, unsigned short y) { _add(*x.num, y); }
  inline void add(unsigned short x, const numb& y) { _add(*y.num, x); }
  inline void sub(const numb& x, unsigned short y) { _sub(*x.num, y); }
  inline void sub(unsigned short x, const numb& y) { _sub(x, *y.num); }
  static inline unsigned short subi(unsigned short x, const numb& y)
    { return number_type::subi(x, *y.num); }
  inline void mul(const numb& x, unsigned short y) { _mul(*x.num, y); }
  inline void mul(unsigned short x, const numb& y) { _mul(*y.num, x); }
  inline void div(const numb& x, unsigned short y) { _div(*x.num, y); }
  static inline void rem(unsigned short& r, const numb& x, unsigned short y)
    { rem_int(r, *x.num, y); }
  static inline void quotrem(numb& q, unsigned short& r, const numb& x, unsigned short y)
    { quotrem_int(q, r, *x.num, y); }
  inline int cmp(unsigned short v) const { return num->cmp(v); }

  inline void set(unsigned v) { _set(v); }
  inline void add(const numb& x, unsigned y) { _add(*x.num, y); }
  inline void add(unsigned x, const numb& y) { _add(*y.num, x); }
  inline void sub(const numb& x, unsigned y) { _sub(*x.num, y); }
  inline void sub(unsigned x, const numb& y) { _sub(x, *y.num); }
  static inline unsigned subi(unsigned x, const numb& y) { return number_type::subi(x, *y.num); }
  inline void mul(const numb& x, unsigned y) { _mul(*x.num, y); }
  inline void mul(unsigned x, const numb& y) { _mul(*y.num, x); }
  inline void div(const numb& x, unsigned y) { _div(*x.num, y); }
  static inline void rem(unsigned& r, const numb& x, unsigned y) { rem_int(r, *x.num, y); }
  static inline void quotrem(numb& q, unsigned& r, const numb& x, unsigned y)
    { quotrem_int(q, r, *x.num, y); }
  static inline unsigned bitwise_andi(const numb& x, unsigned y)
    { return number_type::bitwise_andi(*x.num, y); }
  inline void bitwise_and(const numb& x, unsigned y) { bit_and(*x.num, y); }
  inline void bitwise_and(unsigned x, const numb& y) { bit_and(*y.num, x); }
  inline void bitwise_or(const numb& x, unsigned y) { bit_or(*x.num, y); }
  inline void bitwise_or(unsigned x, const numb& y) { bit_or(*y.num, x); }
  inline void bitwise_xor(const numb& x, unsigned y) { bit_xor(*x.num, y); }
  inline void bitwise_xor(unsigned x, const numb& y) { bit_xor(*y.num, x); }
  static inline unsigned bitwise_and_noti(unsigned x, const numb& y)
    { return number_type::bitwise_and_noti(x, *y.num); }
  inline void bitwise_and_not(const numb& x, unsigned y) { bit_and_not(*x.num, y); }
  inline int cmp(unsigned v) const { return num->cmp(v); }

  inline void set(unsigned long v) { _set(v); }
  inline void add(const numb& x, unsigned long y) { _add(*x.num, y); }
  inline void add(unsigned long x, const numb& y) { _add(*y.num, x); }
  inline void sub(const numb& x, unsigned long y) { _sub(*x.num, y); }
  inline void sub(unsigned long x, const numb& y) { _sub(x, *y.num); }
  static inline unsigned long subi(unsigned long x, const numb& y)
    { return number_type::subi(x, *y.num); }
  inline void mul(const numb& x, unsigned long y) { _mul(*x.num, y); }
  inline void mul(unsigned long x, const numb& y) { _mul(*y.num, x); }
  inline void div(const numb& x, unsigned long y) { _div(*x.num, y); }
  static inline void rem(unsigned long& r, const numb& x, unsigned long y)
    { rem_int(r, *x.num, y); }
  static inline void quotrem(numb& q, unsigned long& r, const numb& x, unsigned long y)
    { quotrem_int(q, r, *x.num, y); }
  inline int cmp(unsigned long v) const { return num->cmp(v); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  inline void set(unsigned long long v) { _set(v); }
  inline void add(const numb& x, unsigned long long y) { _add(*x.num, y); }
  inline void add(unsigned long long x, const numb& y) { _add(*y.num, x); }
  inline void sub(const numb& x, unsigned long long y) { _sub(*x.num, y); }
  inline void sub(unsigned long long x, const numb& y) { _sub(x, *y.num); }
  static inline unsigned long long subi(unsigned long long x, const numb& y)
    { return number_type::subi(x, *y.num); }
  inline void mul(const numb& x, unsigned long long y) { _mul(*x.num, y); }
  inline void mul(unsigned long long x, const numb& y) { _mul(*y.num, x); }
  inline void div(const numb& x, unsigned long long y) { _div(*x.num, y); }
  static inline void rem(unsigned long long& r, const numb& x, unsigned long long y)
    { rem_int(r, *x.num, y); }
  static inline void quotrem(numb& q, unsigned long long& r, const numb& x, unsigned long long y)
    { quotrem_int(q, r, *x.num, y); }
  inline int cmp(unsigned long long v) const { return num->cmp(v); }
#endif

  inline void set(const numb& x) { num = x.num; }
  inline void add(const numb& x, const numb& y) { _add(*x.num, *y.num); }
  inline void sub(const numb& x, const numb& y) { _sub(*x.num, *y.num); }
  inline void mul(const numb& x, const numb& y) { _mul(*x.num, *y.num); }
  inline void div(const numb& x, const numb& y) { _div(*x.num, *y.num); }
  static inline void rem(numb& r, const numb& x, const numb& y) { r.rem_num(*x.num, *y.num); }
  static inline void quotrem(numb& q, numb& r, const numb& u, const numb& v)
    { quotrem_num(q, r, *u.num, *v.num); }
  inline void bitwise_and(const numb& x, const numb& y) { bit_and(*x.num, *y.num); }
  inline void bitwise_or(const numb& x, const numb& y) { bit_or(*x.num, *y.num); }
  inline void bitwise_xor(const numb& x, const numb& y) { bit_xor(*x.num, *y.num); }
  inline void bitwise_and_not(const numb& x, const numb& y) { bit_and_not(*x.num, *y.num); }
  inline int cmp(const numb& v) const { return num->cmp(*v.num); }

  inline void left_shift(const numb& x, std::ptrdiff_t count) { left_shift_num(*x.num, count); }
  inline void right_shift(const numb& x, std::ptrdiff_t count) { right_shift_num(*x.num, count); }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_SHARED_HPP */
