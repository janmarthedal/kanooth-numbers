/* 
 * File:   sputsoft/numbers/detail/nat_num_shared.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created on 2010-04-30 15:18Z
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
class expr<natnum<boost::shared_ptr<wrap2<Con, LowLevel> > > > {
private:
  typedef expr<natnum<wrap2<Con, LowLevel> > > number_type;
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

  template <typename V2>
  void _sub(const number_type& v1, const V2& v2) {
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
  static T rem_int(const number_type& v1, const T& v2) {
    return number_type::rem(v1, v2);
  }

  template <typename V2>
  V2 _quotrem(const number_type& v1, const V2 v2) {
    if (!num.unique()) num.reset(new number_type);
    return num->quotrem(v1, v2);
  }

public:
  expr() : num(new number_type) {}
  expr(const expr& rhs) : num(rhs.num) {}
  expr& operator=(const expr& rhs) {
    num = rhs.num;
    return *this;
  }
  operator bool() const { return *num; }
  std::size_t floor_log2() const { return num->floor_log2(); }

  expr(unsigned short v) : num(new number_type(v)) {}
  inline void set(unsigned short v) { _set(v); }
  inline void add(const expr& x, unsigned short y) { _add(*x.num, y); }
  inline void add(unsigned short x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned short y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned short y) { _mul(*x.num, y); }
  inline void mul(unsigned short x, const expr& y) { _mul(*y.num, x); }
  inline void div(const expr& x, unsigned short y) { _div(*x.num, y); }
  static inline unsigned short rem(const expr& x, unsigned short y) { return rem_int(*x.num, y); }
  inline unsigned short quotrem(const expr& x, unsigned short y) { return _quotrem(*x.num, y); }
  inline int cmp(unsigned short v) const { return num->cmp(v); }

  expr(unsigned v) : num(new number_type(v)) {}
  inline void set(unsigned v) { _set(v); }
  inline void add(const expr& x, unsigned y) { _add(*x.num, y); }
  inline void add(unsigned x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned y) { _mul(*x.num, y); }
  inline void mul(unsigned x, const expr& y) { _mul(*y.num, x); }
  inline void div(const expr& x, unsigned y) { _div(*x.num, y); }
  static inline unsigned rem(const expr& x, unsigned y) { return rem_int(*x.num, y); }
  inline unsigned quotrem(const expr& x, unsigned y) { return _quotrem(*x.num, y); }
  inline int cmp(unsigned v) const { return num->cmp(v); }

  expr(unsigned long v) : num(new number_type(v)) {}
  inline void set(unsigned long v) { _set(v); }
  inline void add(const expr& x, unsigned long y) { _add(*x.num, y); }
  inline void add(unsigned long x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned long y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned long y) { _mul(*x.num, y); }
  inline void mul(unsigned long x, const expr& y) { _mul(*y.num, x); }
  inline void div(const expr& x, unsigned long y) { _div(*x.num, y); }
  static inline unsigned long rem(const expr& x, unsigned long y) { return rem_int(*x.num, y); }
  inline unsigned long quotrem(const expr& x, unsigned long y) { return _quotrem(*x.num, y); }
  inline int cmp(unsigned long v) const { return num->cmp(v); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  expr(unsigned long long v) : num(new number_type(v)) {}
  inline void set(unsigned long long v) { _set(v); }
  inline void add(const expr& x, unsigned long long y) { _add(*x.num, y); }
  inline void add(unsigned long long x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned long long y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned long long y) { _mul(*x.num, y); }
  inline void mul(unsigned long long x, const expr& y) { _mul(*y.num, x); }
  inline void div(const expr& x, unsigned long long y) { _div(*x.num, y); }
  static inline unsigned long long rem(const expr& x, unsigned long long y) { return rem_int(*x.num, y); }
  inline unsigned long long quotrem(const expr& x, unsigned long long y)
    { return _quotrem(*x.num, y); }
  inline int cmp(unsigned long long v) const { return num->cmp(v); }
#endif

  inline void set(const expr& x) { num = x.num; }
  inline void add(const expr& x, const expr& y) { _add(*x.num, *y.num); }
  inline void sub(const expr& x, const expr& y) { _sub(*x.num, *y.num); }
  inline void mul(const expr& x, const expr& y) { _mul(*x.num, *y.num); }
  void div(const expr& x, const expr& y) {
    if (!num.unique()) num.reset(new number_type);
    num->div(*x.num, *y.num);
  }
  void rem(const expr& x, const expr& y) {
    if (!num.unique()) num.reset(new number_type);
    num->rem(*x.num, *y.num);
  }
  static void quotrem(expr& q, expr& r, const expr& u, const expr& v) {
    if (!q.num.unique()) q.num.reset(new number_type);
    if (!r.num.unique()) r.num.reset(new number_type);
    number_type::quotrem(*q.num, *r.num, *u.num, *v.num);
  }
  inline int cmp(const expr& v) const { return num->cmp(*v.num); }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_SHARED_HPP */
