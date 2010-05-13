/* 
 * File:   sputsoft/numbers/detail/nat_num_impl.hpp
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

/*
 * constructor: u, n
 * set: u->n, n->n
 * addition: n+n->n, u+n->n, n+u->n
 * subtraction: n-n->n, u-n->n, n-u->n
 *
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_IMPL_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_IMPL_HPP

#include <boost/integer_traits.hpp>
#include <sputsoft/detail/types.hpp>
#include <sputsoft/numbers/detail/nat_num_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Con, typename LowLevel>
class wrap2;

template <typename Con, typename LowLevel>
class expr<natnum<wrap2<Con, LowLevel> > > {
private:
  Con con;
  typedef typename Con::digit_type digit_type;
  static const unsigned digit_bits = boost::integer_traits<digit_type>::digits;

  static int division_by_zero() {
    int y = 0;  // hide warning
    return 1/y;
  }

  static void ensure_size(Con& c, unsigned size) {
    if (!c.request_size(size))
      Con(size).swap(c);
  }

  template <typename T>
  static T to_int_type(const Con& c) {
    if (c.is_empty())
      return (T) 0;
    else if (sizeof(T) <= sizeof(digit_type))
      return (T) c[0];
    else {
      T v = 0;
      unsigned n=c.size();
      while (n)
        v = (v << digit_bits) | c[--n];
      return v;
    }
  }

  /* Set to number */

  static void set_num(Con& r, const Con& x) {
    std::size_t n = x.size();
    ensure_size(r, n);
    LowLevel::copy(r.get(), x.get(), n);
    r.set_size(n);
  }

  /* Set to integer */

  template <typename T>
  static void set_int(Con& r, T v) {
    if (!v)
      r.set_size(0);
    else if (sizeof(T) <= sizeof(digit_type)) {
      ensure_size(r, 1);
      r[0] = v;
      r.set_size(1);
    } else {
      static const T mask = (T(1) << digit_bits) - 1;
      unsigned n = 0;
      ensure_size(r, sizeof(T) / sizeof(digit_type));
      while (v) {
        r[n++] = v & mask;
        v >>= digit_bits;
      }
      r.set_size(n);
    }
  }

  /* Add two numbers */

  static void add_num0(Con& r, const Con& x, const Con& y) {
    std::size_t xs = x.size();
    digit_type carry = LowLevel::add(r.get(), x.get(), xs, y.get(), y.size());
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= y.size() >= 1
  static void add_num1(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    if (!r.request_size(n + 1)) {
      Con t(n + 1);
      add_num0(t, x, y);
      r.swap(t);
    } else
      add_num0(r, x, y);
  }

  static void add_num(Con& r, const Con& x, const Con& y) {
    if (y.is_empty())              set_num(r, x);
    else if (x.is_empty())         set_num(r, y);
    else if (x.size() >= y.size()) add_num1(r, x, y);
    else                           add_num1(r, y, x);
  }

  /* Add number and integer */

  static void add_int0(Con& r, const Con& x, const digit_type y) {
    std::size_t xs = x.size();
    digit_type carry = LowLevel::add_1(r.get(), x.get(), xs, y);
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= 1, y != 0
  static void add_int1(Con& r, const Con& x, const digit_type y) {
    std::size_t xs = x.size();
    if (!r.request_size(xs + 1)) {
      Con t(xs + 1);
      add_int0(t, x, y);
      r.swap(t);
    } else
      add_int0(r, x, y);
  }

  template <typename T>
  static void add_int(Con& r, const Con& x, const T y) {
    if (x.is_empty())
      set_int(r, y);
    else if (!y)
      set_num(r, x);
    else if (sizeof(T) <= sizeof(digit_type))
      add_int1(r, x, y);
    else
      add_num(r, x, expr(y).con);
  }

  /* Subtract two numbers */

  static void sub_num0(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    LowLevel::sub(r.get(), x.get(), n, y.get(), y.size());
    while (n && !r[n-1]) --n;
    r.set_size(n);
  }

  // x.size() >= y.size() >= 1
  static void sub_num1(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    if (!r.request_size(n)) {
      Con t(n);
      sub_num0(t, x, y);
      r.swap(t);
    } else
      sub_num0(r, x, y);
  }

  static void sub_num(Con& r, const Con& x, const Con& y) {
    if (y.is_empty())             set_num(r, x);
    else if (y.size() > x.size()) r.set_size(0);  // misuse !!!
    else                          sub_num1(r, x, y);
  }

  /* Subtract number and integer */

  static void sub_int0(Con& r, const Con& x, const digit_type y) {
    std::size_t n = x.size();
    LowLevel::sub_1(r.get(), x.get(), n, y);
    while (n && !r[n-1]) --n;
    r.set_size(n);
  }

  // x.size() >= 1, y != 0
  static void sub_int1(Con& r, const Con& x, const digit_type y) {
    std::size_t n = x.size();
    if (!r.request_size(n)) {
      Con t(n);
      sub_int0(t, x, y);
      r.swap(t);
    } else
      sub_int0(r, x, y);
  }

  template <typename T>
  static void sub_num_int(Con& r, const Con& x, const T y) {
    if (x.is_empty())
      r.set_size(0);
    else if (!y)
      set_num(r, x);
    else if (sizeof(T) <= sizeof(digit_type))
      sub_int1(r, x, y);
    else
      sub_num(r, x, expr(y).con);
  }

  template <typename T>
  static void sub_int_num(Con& r, const T x, const Con& y) {
    if (!x)
      r.set_size(0);
    else if (y.is_empty())
      set_int(r, x);
    else if (sizeof(T) <= sizeof(digit_type))
      sub_int(r, x - y[0]);
    else
      sub_num(r, expr(x).con, y);
  }

  /* Multiply two numbers */

  static void mul_num0(Con& r, const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    std::size_t n  = xn + yn;
    LowLevel::mul(r.get(), x.get(), xn, y.get(), yn);
    r.set_size(r[n-1] ? n : n-1);
  }

  // x.size() >= y.size() >= 1
  static void mul_num1(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size() + y.size();
    if (!r.request_size(n) || r.get() == x.get() || r.get() == y.get()) {
      Con t(n);
      mul_num0(t, x, y);
      r.swap(t);
    } else
      mul_num0(r, x, y);
  }

  static void mul_num(Con& r, const Con& x, const Con& y) {
    if (y.is_empty() || x.is_empty()) r.set_size(0);
    else if (x.size() >= y.size())    mul_num1(r, x, y);
    else                              mul_num1(r, y, x);
  }

  /* Multiply number and integer */

  static void mul_int0(Con& r, const Con& x, const digit_type y) {
    std::size_t n = x.size();
    digit_type carry = LowLevel::mul_1(r.get(), x.get(), n, y);
    if (carry) r[n++] = carry;
    r.set_size(n);
  }

  // x.size() >= 1, y != 0
  static void mul_int1(Con& r, const Con& x, const digit_type y) {
    std::size_t n = x.size();
    if (!r.request_size(n + 1)) {
      Con t(n + 1);
      mul_int0(t, x, y);
      r.swap(t);
    } else
      mul_int0(r, x, y);
  }

  template <typename T>
  static void mul_int(Con& z, const Con& x, const T y) {
    if (x.is_empty() || !y)
      z.set_size(0);
    else if (y == T(1))
      set_int(z, T(0));
    else if (sizeof(T) <= sizeof(digit_type))
      mul_int1(z, x, y);
    else
      mul_num(z, x, expr(y).con);
  }

  /* Quotient and remainder from two numbers */

  static void quotrem_num0(Con& q, Con& r, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    std::size_t n = un - vn + 1;
    LowLevel::quotrem(q.get(), r.get(), u.get(), un, v.get(), vn);
    q.set_size(q[n-1] ? n : n-1);
    while (vn && !r[vn-1]) --vn;
    r.set_size(vn);
  }

  static void quot_num(Con& q, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    if (!vn)
      division_by_zero();
    else if (un < vn)
      set_int(q, digit_type(0));
    else if (u.get() == v.get())
      set_int(q, digit_type(1));
    else {
      Con r(vn);  // work space only
      std::size_t qn = un-vn+1;
      if (q.get() == u.get() || q.get() == v.get() || !q.request_size(qn)) {
        Con qt(qn);
        quotrem_num0(qt, r, u, v);
        q.swap(qt);
      } else
        quotrem_num0(q, r, u, v);
    }
  }

  static void rem_num(Con& r, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    if (!vn)
      division_by_zero();
    else if (un < vn)
      set_num(r, u);
    else if (u.get() == v.get())
      set_int(r, digit_type(0));
    else {
      Con q(un-vn+1);  // work space only
      if (r.get() == u.get() || r.get() == v.get() || !r.request_size(vn)) {
        Con rt(vn);
        quotrem_num0(q, rt, u, v);
        r.swap(rt);
      } else
        quotrem_num0(q, r, u, v);
    }
  }

  static void quotrem_num(Con& q, Con& r, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    if (!vn)
      division_by_zero();
    else if (un < vn) {
      set_int(q, digit_type(0));
      set_num(r, u);
    } else if (u.get() == v.get()) {
      set_int(q, digit_type(1));
      set_int(r, digit_type(0));
    } else {
      std::size_t qn = un-vn+1;
      if (q.get() == u.get() || q.get() == v.get() || !q.request_size(qn)) {
        Con qt(qn);
        if (r.get() == u.get() || r.get() == v.get() || !r.request_size(vn)) {
          Con rt(vn);
          quotrem_num0(qt, rt, u, v);
          r.swap(rt);
        } else
          quotrem_num0(qt, r, u, v);
        q.swap(qt);
      } else if (r.get() == u.get() || r.get() == v.get() || !r.request_size(vn)) {
        Con rt(vn);
        quotrem_num0(q, rt, u, v);
        r.swap(rt);
      } else
        quotrem_num0(q, r, u, v);
    }
  }

  /* Quotient and remainder from a number and an integer */

  // q.size() >= u.size(), !u.is_empty(), v != 0
  static digit_type quotrem_int0(Con& q, const Con& u, const digit_type v) {
    std::size_t n = u.size();
    digit_type r = LowLevel::quotrem_1(q.get(), u.get(), n, v);
    q.set_size(q[n-1] ? n : n-1);
    return r;
  }

  template <typename T>
  static void quot_int(Con& q, const Con& u, const T v) {
    if (!v)
      division_by_zero();
    else if (u.is_empty())
      q.set_size(0);
    else if (sizeof(T) <= sizeof(digit_type)) {
      ensure_size(q, u.size());
      quotrem_int0(q, u, v);
    } else {
      Con r(sizeof(T) / sizeof(digit_type));  // work space only
      quotrem_num(q, r, u, expr(v).con);
    }
  }

  template <typename T>
  static T rem_int(const Con& u, const T v) {
    if (!v)
      division_by_zero();
    else if (u.is_empty())
      return 0;
    else {
      Con q(u.size());  // work space only
      if (sizeof(T) <= sizeof(digit_type))
        return quotrem_int0(q, u, v);
      else {
        Con r(sizeof(T) / sizeof(digit_type));
        quotrem_num(q, r, u, expr(v).con);
        return to_int_type<T>(r);
      }
    }
  }

  template <typename T>
  static T quotrem_int(Con& q, const Con& u, const T v) {
    if (!v)
      division_by_zero();
    else if (u.is_empty()) {
      q.set_size(0);
      return 0;
    } else if (sizeof(T) <= sizeof(digit_type)) {
      // q.get() == u.get() is ok, because q is then never resized
      ensure_size(q, u.size());
      return quotrem_int0(q, u, v);
    } else {
      Con r(sizeof(T) / sizeof(digit_type));
      quotrem_num(q, r, u, expr(v).con);
      return to_int_type<T>(r);
    }
  }

  /* Compare two numbers */

  template <typename T>
  static inline int compare_ints(const T a, const T b) {
    return a < b ? -1 : a > b ? 1 : 0;
  }

  static int comp_num(const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    if (xn != yn) return compare_ints(xn, yn);
    if (!xn) return 0;
    LowLevel::comp(x.get(), y.get(), xn);
  }

  /* Compare numbers to integer */

  template <typename T>
  static int comp_int(const Con& x, const T v) {
    if (!v)
      return x.is_empty() ? 0 : 1;
    else if (x.is_empty())
      return -1;
    else if (sizeof(T) <= sizeof(digit_type))
      return x.size() > 1 ? 1 : compare_ints(x[0], (digit_type) v);
    else
      return comp_num(x, expr(v).con);
  }

public:
  expr() : con() {}
  expr(const expr& rhs) : con() {
    set_num(con, rhs.con);
  }
  expr& operator=(const expr& rhs) {
    set_num(con, rhs.con);
    return *this;
  }
  operator bool() const {
    return !con.is_empty();
  }
  std::size_t log2_floor() const {
    std::size_t n = con.size();
    if (!n) return (std::size_t) -1;
    return sputsoft::numbers::log2_floor(con[n - 1]) + (n - 1)*digit_bits;
  }

  expr(unsigned short v) : con() { set_int(con, v); }
  inline void set(unsigned short v) { set_int(con, v); }
  inline void add(const expr& x, unsigned short y) { add_int(con, x.con, y); }
  inline void add(unsigned short x, const expr& y) { add_int(con, y.con, x); }
  inline void sub(const expr& x, unsigned short y) { sub_num_int(con, x.con, y); }
  inline void sub(unsigned short x, const expr& y) { sub_int_num(con, x, y.con); }
  inline void mul(const expr& x, unsigned short y) { mul_int(con, x.con, y); }
  inline void mul(unsigned short x, const expr& y) { mul_int(con, y.con, x); }
  inline void div(const expr& x, unsigned short y) { quot_int(con, x.con, y); }
  static inline unsigned short rem(const expr& x, unsigned short y) { return rem_int(x.con, y); }
  inline unsigned short quotrem(const expr& x, unsigned short y)
    { return quotrem_int(con, x.con, y); }
  inline int cmp(unsigned short v) const { return comp_int(con, v); }

  expr(unsigned v) : con() { set_int(con, v); }
  inline void set(unsigned v) { set_int(con, v); }
  inline void add(const expr& x, unsigned y) { add_int(con, x.con, y); }
  inline void add(unsigned x, const expr& y) { add_int(con, y.con, x); }
  inline void sub(const expr& x, unsigned y) { sub_num_int(con, x.con, y); }
  inline void sub(unsigned x, const expr& y) { sub_int_num(con, x, y.con); }
  inline void mul(const expr& x, unsigned y) { mul_int(con, x.con, y); }
  inline void mul(unsigned x, const expr& y) { mul_int(con, y.con, x); }
  inline void div(const expr& x, unsigned y) { quot_int(con, x.con, y); }
  static inline unsigned rem(const expr& x, unsigned y) { return rem_int(x.con, y); }
  inline unsigned quotrem(const expr& x, unsigned y) { return quotrem_int(con, x.con, y); }
  inline int cmp(unsigned v) const { return comp_int(con, v); }

  expr(unsigned long v) : con() { set_int(con, v); }
  inline void set(unsigned long v) { set_int(con, v); }
  inline void add(const expr& x, unsigned long y) { add_int(con, x.con, y); }
  inline void add(unsigned long x, const expr& y) { add_int(con, y.con, x); }
  inline void sub(const expr& x, unsigned long y) { sub_num_int(con, x.con, y); }
  inline void sub(unsigned long x, const expr& y) { sub_int_num(con, x, y.con); }
  inline void mul(const expr& x, unsigned long y) { mul_int(con, x.con, y); }
  inline void mul(unsigned long x, const expr& y) { mul_int(con, y.con, x); }
  inline void div(const expr& x, unsigned long y) { quot_int(con, x.con, y); }
  static inline unsigned long rem(const expr& x, unsigned long y) { return rem_int(x.con, y); }
  inline unsigned long quotrem(const expr& x, unsigned long y)
    { return quotrem_int(con, x.con, y); }
  inline int cmp(unsigned long v) const { return comp_int(con, v); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  expr(unsigned long long v) : con() { set_int(con, v); }
  inline void set(unsigned long long v) { set_int(con, v); }
  inline void add(const expr& x, unsigned long long y) { add_int(con, x.con, y); }
  inline void add(unsigned long long x, const expr& y) { add_int(con, y.con, x); }
  inline void sub(const expr& x, unsigned long long y) { sub_num_int(con, x.con, y); }
  inline void sub(unsigned long long x, const expr& y) { sub_int_num(con, x, y.con); }
  inline void mul(const expr& x, unsigned long long y) { mul_int(con, x.con, y); }
  inline void mul(unsigned long long x, const expr& y) { mul_int(con, y.con, x); }
  inline void div(const expr& x, unsigned long long y) { quot_int(con, x.con, y); }
  static inline unsigned long long rem(const expr& x, unsigned long long y) { return rem_int(x.con, y); }
  inline unsigned long long quotrem(const expr& x, unsigned long long y)
    { return quotrem_int(con, x.con, y); }
  inline int cmp(unsigned long long v) const { return comp_int(con, v); }
#endif

  inline void set(const expr& x) { set_num(con, x.con); }
  inline void add(const expr& x, const expr& y) { add_num(con, x.con, y.con); }
  inline void sub(const expr& x, const expr& y) { sub_num(con, x.con, y.con); }
  inline void mul(const expr& x, const expr& y) { mul_num(con, x.con, y.con); }
  inline void div(const expr& x, const expr& y) { quot_num(con, x.con, y.con); }
  inline void rem(const expr& x, const expr& y) { rem_num(con, x.con, y.con); }
  static inline void quotrem(expr& q, expr& r, const expr& x, const expr& y)
    { quotrem_num(q.con, r.con, x.con, y.con); }
  inline int cmp(const expr& v) const { return comp_num(con, v.con); }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_IMPL_HPP */
