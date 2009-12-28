/*
 * File:   numbers/detail/midlevel.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_MIDLEVEL_HPP_
#define _SPUTSOFT_NUMBERS_DETAIL_MIDLEVEL_HPP_

#include <sputsoft/types.hpp>
#include <sputsoft/number_theory/common.hpp>
#include <boost/integer_traits.hpp>


namespace sputsoft {
namespace numbers {
namespace detail {

int division_by_zero() {
  int y = 0;  // hide warning
  return 1/y;
}


template <typename Con, typename LowLevel> class midlevel;

template <typename Con, typename LowLevel, typename D>
class midlevel_1_noconv {
private:

  static void add3(Con& r, const Con& x, const D y) {
    std::size_t xs = x.size();
    D carry = LowLevel::add_1(r.get(), x.get(), xs, y);
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= 1, y != 0
  static void add2(Con& r, const Con& x, const D y) {
    std::size_t xs = x.size();
    if (!r.request_size(xs + 1)) {
      Con t(xs + 1);
      add3(t, x, y);
      r.swap(t);
    } else
      add3(r, x, y);
  }

  static void sub3(Con& r, const Con& x, const D y) {
    std::size_t n = x.size();
    LowLevel::sub_1(r.get(), x.get(), n, y);
    while (n && !r[n-1]) --n;
    r.set_size(n);
  }

  // x.size() >= 1, y != 0
  static void sub2(Con& r, const Con& x, const D y) {
    std::size_t n = x.size();
    if (!r.request_size(n)) {
      Con t(n);
      sub3(t, x, y);
      r.swap(t);
    } else
      sub3(r, x, y);
  }

  static void mul3(Con& r, const Con& x, const D y) {
    std::size_t n = x.size();
    D carry = LowLevel::mul_1(r.get(), x.get(), n, y);
    if (carry) r[n++] = carry;
    r.set_size(n);
  }

  // x.size() >= 1, y != 0
  static void mul2(Con& r, const Con& x, const D y) {
    std::size_t n = x.size();
    if (!r.request_size(n + 1)) {
      Con t(n + 1);
      mul3(t, x, y);
      r.swap(t);
    } else
      mul3(r, x, y);
  }

  // q.size() >= u.size(), !u.is_empty(), v != 0
  static D div2(Con& q, const Con& u, const D v) {
    std::size_t n = u.size();
    D r = LowLevel::quotrem_1(q.get(), u.get(), n, v);
    q.set_size(q[n-1] ? n : n-1);
    return r;
  }

public:

  static void set(Con& r, const D x) {
    if (x) {
      if (!r.request_size(1))
        Con(1).swap(r);
      r[0] = x;
      r.set_size(1);
    } else
      r.set_size(0);
  }

  static void add(Con& r, const Con& x, const D y) {
    if (x.is_empty()) set(r, y);
    else if (!y)      midlevel<Con, LowLevel>::set(r, x);
    else              add2(r, x, y);
  }

  static void subtract(Con& r, const Con& x, const D y) {
    if (x.is_empty()) set(r, D(0));                        // misuse if y != 0
    else if (!y)      midlevel<Con, LowLevel>::set(r, x);
    else              sub2(r, x, y);
  }

  static D subtract(const D x, const Con& y) {
    return y.is_empty() ? x : y.size() > 1 ? 0 : x - y[0];
  }

  static void multiply(Con& r, const Con& x, const D y) {
    if (x.is_empty() || !y) set(r, D(0));
    else if (y == 1)        midlevel<Con, LowLevel>::set(r, x);
    else                    mul2(r, x, y);
  }

  static void divide(Con& r, const Con& u, const D v) {
    if (!v)
      division_by_zero();
    else if (u.is_empty())
      r.set_size(0);
    else {
      std::size_t n = u.size();
      if (!r.request_size(n)) {
        Con t(n);
        div2(t, u, v);
        r.swap(t);
      } else
        div2(r, u, v);
    }
  }

  static inline D divide(const D u, const Con& v) {
    if (v.is_empty())
      division_by_zero();
    else if (v.size() > 1)
      return 0;
    else
      return u / v[0];
  }

  static inline D remainder(const Con& u, const D v) {
    return LowLevel::rem_1(u.get(), u.size(), v);
  }

  static void remainder(Con& r, const D u, const Con& v) {
    if (v.is_empty())
      division_by_zero();
    else if (v.size() > 1 || v[0] > u)
      set(r, u);
    else
      set(r, u % v[0]);
  }

  static D quotrem(Con& q, const Con& u, const D v) {
    if (!v)
      division_by_zero();
    else if (u.is_empty()) {
      q.set_size(0);
      return v;
    } else {
      Con qt(u.size());
      D r = div2(qt, u, v);
      q.swap(qt);
      return r;
    }
  }

  static D quotrem(Con& r, const D u, const Con& v) {
    if (v.is_empty())
      division_by_zero();
    else if (v.size() > 1 || v[0] > u) {
      set(r, v);
      return 0;
    } else {
      std::pair<D, D> qr = number_theory::quotrem(u, v[0]);
      set(r, qr.second);
      return qr.first;
    }
  }

};

template <typename Con, typename D, typename B>
class conwrap {
private:
  static const unsigned d_bits = boost::integer_traits<D>::digits;
  static const unsigned b_bits = boost::integer_traits<B>::digits;
  static const unsigned con_size = b_bits / d_bits;
  static const B mask = (B(1) << d_bits) - 1;
  Con con;
public:
  conwrap() : con(con_size) {}
  conwrap(B v) : con(con_size) {
    unsigned n = 0;
    while (v) {
      con[n++] = v & mask;
      v >>= d_bits;
    }
    con.set_size(n);
  }
  const Con& get() const { return con; }
  Con& get() { return con; }
  B value() const {
    B r = 0;
    unsigned n=con.size();
    while (n)
      r = (r << d_bits) | con[--n];
    return r;
  }
};

template <typename Con, typename LowLevel, typename D, typename B, bool Cnv>
class midlevel_1;

/* Argument type equal to or smaller than digit type: */

template <typename Con, typename LowLevel, typename D, typename B>
class midlevel_1<Con, LowLevel, D, B, false>
 : public midlevel_1_noconv<Con, LowLevel, D> {};

/* Argument type larger than digit type: */

template <typename Con, typename LowLevel, typename D, typename B>
class midlevel_1<Con, LowLevel, D, B, true> {
private:

  typedef conwrap<Con, D, B> con_type;
  typedef midlevel<Con, LowLevel> midlevel_;

public:

  static inline void set(Con& r, const B u) {
    midlevel_::set(r, con_type(u).get());
  }

  static inline void add(Con& r, const Con& u, const B v) {
    midlevel_::add(r, u, con_type(v).get());
  }

  static inline void subtract(Con& r, const Con& u, const B v) {
    midlevel_::subtract(r, u, con_type(v).get());
  }

  static inline B subtract(const B u, const Con& v) {
    con_type cr;
    midlevel_::subtract(cr.get(), con_type(u).get(), v);
    return cr.value();
  }

  static inline void multiply(Con& r, const Con& u, const B v) {
    midlevel_::multiply(r, u, con_type(v).get());
  }

  static inline void divide(Con& r, const Con& u, const B v) {
    midlevel_::multiply(r, u, con_type(v).get());
  }

  static inline B divide(const B u, const Con& v) {
    con_type cq;
    midlevel_::divide(cq.get(), con_type(u).get(), v);
    return cq.value();
  }

  static inline B remainder(const Con& u, const B v) {
    con_type cr;
    con_type cv(v);
    midlevel_::remainder(cr.get(), u, cv.get());
    return cr.value();
  }

  static inline void remainder(Con& r, const B u, const Con& v) {
    midlevel_::remainder(r, con_type(u).get(), v);
  }

  static inline B quotrem(Con& q, const Con& u, const B v) {
    con_type cr;
    midlevel_::quotrem(q, cr.get(), u, con_type(v).get());
    return cr.value();
  }

  static inline B quotrem(Con& r, const B u, const Con& v) {
    con_type cq;
    midlevel_::quotrem(cq.get(), r, con_type(u).get(), v);
    return cq.value();
  }

};


template <typename Con, typename LowLevel>
class midlevel {

  typedef typename Con::digit_type digit_type;
  static const unsigned digit_bits = boost::integer_traits<digit_type>::digits;
  static const unsigned short_bits = boost::integer_traits<unsigned short>::digits;
  static const unsigned int_bits = boost::integer_traits<unsigned>::digits;
  static const unsigned long_bits = boost::integer_traits<unsigned long>::digits;
  typedef midlevel_1<Con, LowLevel, digit_type, unsigned short, digit_bits < short_bits> midlevel_1_short;
  typedef midlevel_1<Con, LowLevel, digit_type, unsigned,       digit_bits < int_bits>   midlevel_1_int;
  typedef midlevel_1<Con, LowLevel, digit_type, unsigned long,  digit_bits < long_bits>  midlevel_1_long;

  static void add3(Con& r, const Con& x, const Con& y) {
    std::size_t xs = x.size();
    digit_type carry = LowLevel::add(r.get(), x.get(), xs, y.get(), y.size());
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= y.size() >= 1
  static void add2(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    if (!r.request_size(n + 1)) {
      Con t(n + 1);
      add3(t, x, y);
      r.swap(t);
    } else 
      add3(r, x, y);
  }

  static void sub3(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    LowLevel::sub(r.get(), x.get(), n, y.get(), y.size());
    while (n && !r[n-1]) --n;
    r.set_size(n);
  }

  // x.size() >= y.size() >= 1
  static void sub2(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    if (!r.request_size(n)) {
      Con t(n);
      sub3(t, x, y);
      r.swap(t);
    } else
      sub3(r, x, y);
  }

  static void mul3(Con& r, const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    std::size_t n  = xn + yn;
    LowLevel::mul(r.get(), x.get(), xn, y.get(), yn);
    r.set_size(r[n-1] ? n : n-1);
  }

  // x.size() >= y.size() >= 1
  static void mul2(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size() + y.size();
    if (!r.request_size(n) || r.get() == x.get() || r.get() == y.get()) {
      Con t(n);
      mul3(t, x, y);
      r.swap(t);
    } else
      mul3(r, x, y);
  }

  static void quotrem2(Con& q, Con& r, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    std::size_t n = un - vn + 1;
    LowLevel::quotrem(q.get(), r.get(), u.get(), un, v.get(), vn);
    q.set_size(q[n-1] ? n : n-1);
    while (vn && !r[vn-1]) --vn;
    r.set_size(vn);
  }

  static void lshift2(Con& z, const Con& u, std::size_t count) {
    unsigned whole = count / digit_bits;
    count %= digit_bits;
    std::size_t zn = u.size() + whole;
    if (count) {
      digit_type s = LowLevel::lshift(z.get() + whole, u.get(), u.size(), count);
      if (s) z[zn++] = s;
    } else
      std::copy_backward(u.get(), u.get() + u.size(), z.get() + zn);
    std::fill_n(z.get(), whole, 0);
    z.set_size(zn);
  }

  static void rshift2(Con& z, const Con& u, std::size_t count) {
    unsigned whole = count / digit_bits;
    count %= digit_bits;
    std::size_t zn = u.size() - whole;
    if (count) {
      LowLevel::rshift(z.get(), u.get() + whole, zn, count);
      if (!z[zn-1]) --zn;
    } else
      std::copy(u.get() + whole, u.get() + u.size(), z.get());
    z.set_size(zn);
  }

public:

  typedef Con container_type;

  static void set(Con& r, const Con& x) {
    std::size_t n = x.size();
    if (!r.request_size(n)) {
      Con t(n);
      LowLevel::copy(t.get(), x.get(), n);
      r.swap(t);
    } else
      LowLevel::copy(r.get(), x.get(), x.size());
    r.set_size(n);
  }

  static void add(Con& r, const Con& x, const Con& y) {
    if (y.is_empty())              set(r, x);
    else if (x.is_empty())         set(r, y);
    else if (x.size() >= y.size()) add2(r, x, y);
    else                           add2(r, y, x);
  }

  static void subtract(Con& r, const Con& x, const Con& y) {
    if (y.is_empty())             set(r, x);
    else if (y.size() > x.size()) set(r, digit_type(0));  // misuse !!!
    else                          sub2(r, x, y);
  }

  static void multiply(Con& r, const Con& x, const Con& y) {
    if (y.is_empty() || x.is_empty()) set(r, digit_type(0));
    else if (x.size() >= y.size())    mul2(r, x, y);
    else                              mul2(r, y, x);
  }

  static void divide(Con& q, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    if (!vn)
      division_by_zero();
    else if (un < vn)
      set(q, digit_type(0));
    else if (u.get() == v.get())
      set(q, digit_type(1));
    else {
      Con r(vn);
      std::size_t n = un-vn+1;
      if (q.get() == u.get() || q.get() == v.get() || !q.request_size(n)) {
        Con t(n);
        quotrem2(t, r, u, v);
        q.swap(t);
      } else
        quotrem2(q, r, u, v);
    }
  }

  static void remainder(Con& r, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    if (!vn)
      division_by_zero();
    else if (un < vn)
      set(r, un);
    else if (u.get() == v.get())
      set(r, digit_type(0));
    else {
      Con q(un-vn+1);
      if (r.get() == u.get() || r.get() == v.get() || !r.request_size(vn)) {
        Con t(vn);
        quotrem2(q, t, u, v);
        r.swap(t);
      } else
        quotrem2(q, r, u, v);
    }
  }

  static void quotrem(Con& q, Con& r, const Con& u, const Con& v) {
    std::size_t un = u.size();
    std::size_t vn = v.size();
    if (!vn)
      division_by_zero();
    else if (un < vn) {
      set(q, digit_type(0));
      set(r, un);
    } else if (u.get() == v.get()) {
      set(q, digit_type(1));
      set(r, digit_type(0));
    } else {
      Con qt(un-vn+1);
      Con rt(vn);
      quotrem2(qt, rt, u, v);
      r.swap(rt);
      q.swap(qt);
    }
  }

  static void left_shift(Con& z, const Con& u, std::size_t count) {
    if (u.is_empty())
      set(z, digit_type(0));
    else if (!count)
      set(z, u);
    else {
      std::size_t n = u.size() + (count+digit_bits-1)/digit_bits;
      if (!z.request_size(n)) {
        Con t(n);
        lshift2(t, u, count);
        z.swap(t);
      } else
        lshift2(z, u, count);
    }
  }

  static void right_shift(Con& z, const Con& u, std::size_t count) {
    if (u.is_empty() || count >= u.size()*digit_bits)
      set(z, digit_type(0));
    else if (!count)
      set(z, u);
    else {
      std::size_t n = u.size() - count/digit_bits;
      if (!z.request_size(n)) {
        Con t(n);
        rshift2(t, u, count);
        z.swap(t);
      } else
        rshift2(z, u, count);
    }
  }

  static inline void set(Con& r, unsigned short u) {
    midlevel_1_short::set(r, u);
  }
  static inline void add(Con& r, const Con& u, unsigned short v) {
    midlevel_1_short::add(r, u, v);
  }
  static inline void subtract(Con& r, const Con& u, unsigned short v) {
    midlevel_1_short::subtract(r, u, v);
  }
  static inline unsigned short subtract(unsigned short u, const Con& v) {
    return midlevel_1_short::subtract(u, v);
  }
  static inline void multiply(Con& r, const Con& u, unsigned short v) {
    midlevel_1_short::multiply(r, u, v);
  }
  static inline void divide(Con& r, const Con& u, unsigned short v) {
    midlevel_1_short::divide(r, u, v);
  }
  static inline unsigned short divide(unsigned short u, const Con& v) {
    midlevel_1_short::divide(u, v);
  }
  static inline unsigned short remainder(const Con& u, unsigned short v) {
    midlevel_1_short::remainder(u, v);
  }
  static inline void remainder(Con& r, unsigned short u, const Con& v) {
    midlevel_1_short::remainder(r, u, v);
  }
  static inline digit_type quotrem(Con& r, const Con& u, unsigned short v) {
    midlevel_1_short::quotrem(r, u, v);
  }
  static inline digit_type quotrem(Con& r, unsigned short u, const Con& v) {
    midlevel_1_short::quotrem(r, u, v);
  }

  static inline void set(Con& r, unsigned u) {
    midlevel_1_int::set(r, u);
  }
  static inline void add(Con& r, const Con& u, unsigned v) {
    midlevel_1_int::add(r, u, v);
  }
  static inline void subtract(Con& r, const Con& u, unsigned v) {
    midlevel_1_int::subtract(r, u, v);
  }
  static inline unsigned subtract(unsigned u, const Con& v) {
    return midlevel_1_int::subtract(u, v);
  }
  static inline void multiply(Con& r, const Con& u, unsigned v) {
    midlevel_1_int::multiply(r, u, v);
  }
  static inline void divide(Con& r, const Con& u, unsigned v) {
    midlevel_1_int::divide(r, u, v);
  }
  static inline unsigned divide(unsigned u, const Con& v) {
    midlevel_1_int::divide(u, v);
  }
  static inline unsigned remainder(const Con& u, unsigned v) {
    midlevel_1_int::remainder(u, v);
  }
  static inline void remainder(Con& r, unsigned u, const Con& v) {
    midlevel_1_int::remainder(r, u, v);
  }
  static inline digit_type quotrem(Con& r, const Con& u, unsigned v) {
    midlevel_1_int::quotrem(r, u, v);
  }
  static inline digit_type quotrem(Con& r, unsigned u, const Con& v) {
    midlevel_1_int::quotrem(r, u, v);
  }

  static inline void set(Con& r, unsigned long u) {
    midlevel_1_long::set(r, u);
  }
  static inline void add(Con& r, const Con& u, unsigned long v) {
    midlevel_1_long::add(r, u, v);
  }
  static inline void subtract(Con& r, const Con& u, unsigned long v) {
    midlevel_1_long::subtract(r, u, v);
  }
  static inline unsigned long subtract(unsigned long u, const Con& v) {
    return midlevel_1_long::subtract(u, v);
  }
  static inline void multiply(Con& r, const Con& u, unsigned long v) {
    midlevel_1_long::multiply(r, u, v);
  }
  static inline void divide(Con& r, const Con& u, unsigned long v) {
    midlevel_1_long::divide(r, u, v);
  }
  static inline unsigned long divide(unsigned long u, const Con& v) {
    midlevel_1_long::divide(u, v);
  }
  static inline unsigned long remainder(const Con& u, unsigned long v) {
    midlevel_1_long::remainder(u, v);
  }
  static inline void remainder(Con& r, unsigned long u, const Con& v) {
    midlevel_1_long::remainder(r, u, v);
  }
  static inline digit_type quotrem(Con& r, const Con& u, unsigned long v) {
    midlevel_1_long::quotrem(r, u, v);
  }
  static inline digit_type quotrem(Con& r, unsigned long u, const Con& v) {
    midlevel_1_long::quotrem(r, u, v);
  }

};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_MIDLEVEL_HPP_
