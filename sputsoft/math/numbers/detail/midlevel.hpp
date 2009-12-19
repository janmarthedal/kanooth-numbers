/*
 * File:   math/numbers/detail/midlevel.hpp
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

#ifndef _SPUTSOFT_MATH_NUMBERS_DETAIL_MIDLEVEL_HPP_
#define _SPUTSOFT_MATH_NUMBERS_DETAIL_MIDLEVEL_HPP_

#include <sputsoft/types.hpp>
#include <boost/integer_traits.hpp>

template <typename T>
std::size_t log2_floor(T n) {
  std::size_t r = -1;
  while (n) {
    ++r;
    n >>= 1;
  }
  return r;
}


/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace detail {

int division_by_zero() {
  int y = 0;  // hide warning
  return 1/y;
}

/*
template <typename Con, typename LowLevel, typename D, typename B>
class midlevel_literals {

  static inline B remainder(const Con& u, B v) {
    return LowLevel::rem_1(u.get(), u.size(), v);
  }

};

template <typename Con, typename LowLevel>
class midlevel_literals<Con, LowLevel, uint16_t, uint32_t> {

  static inline void set_con(Con& r, uint32_t x) {
    r[0] = x & 0xFFFF;
    r[1] = x >> 16;
  }

  static inline uint32_t get_con(const Con& u) {
    return ((uint32_t) u[1] << 16) | ((uint32_t) u[0]);
  }

  static inline uint32_t remainder(const Con& u, uint32_t v) {
    Con r(2), vc(2);
    set_con(vc, v);
    midlevel<Con, LowLevel>::remainder(r, u, vc);
    return get_con(r);
  }

};
*/


template <typename Con, typename LowLevel>
class midlevel {

  typedef typename Con::digit_type digit_type;
  static const unsigned digit_bits = boost::integer_traits<digit_type>::digits;

  static void add3(Con& r, const Con& x, const Con& y) {
    std::size_t xs = x.size();
    digit_type carry = LowLevel::add(r.get(), x.get(), xs, y.get(), y.size());
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= y.size() >= 1
  static void add2(Con& r, const Con& x, const Con& y) {
    std::size_t xs = x.size();
    if (!r.request_size(xs + 1)) {
      Con t(xs + 1);
      add3(t, x, y);
      r.swap(t);
    } else 
      add3(r, x, y);
  }

  static void add3(Con& r, const Con& x, digit_type y) {
    std::size_t xs = x.size();
    digit_type carry = LowLevel::add_1(r.get(), x.get(), xs, y);
    if (carry) r[xs++] = carry;
    r.set_size(xs);
  }

  // x.size() >= 1, y != 0
  static void add2(Con& r, const Con& x, digit_type y) {
    std::size_t xs = x.size();
    if (!r.request_size(xs + 1)) {
      Con t(xs + 1);
      add3(t, x, y);
      r.swap(t);
    } else 
      add3(r, x, y);    
  }

  static void mul3(Con& r, const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    std::size_t n  = xn*yn;
    LowLevel::mul(r.get(), x.get(), xn, y.get(), yn);
    r.set_size(r[n-1] ? n : n-1);
  }

  // x.size() >= y.size() >= 1
  static void mul2(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size() * y.size();
    if (!r.request_size(n) || r.get() == x.get() || r.get() == y.get()) {
      Con t(n);
      mul3(t, x, y);
      r.swap(t);
    } else
      mul3(r, x, y);
  }

  static void mul3(Con& r, const Con& x, digit_type y) {
    std::size_t n = x.size();
    digit_type carry = LowLevel::mul_1(r.get(), x.get(), n, y);
    if (carry) r[n++] = carry;
    r.set_size(n);
  }

  // x.size() >= 1, y != 0
  static void mul2(Con& r, const Con& x, digit_type y) {
    std::size_t n = x.size();
    if (!r.request_size(n + 1)) {
      Con t(n + 1);
      mul3(t, x, y);
      r.swap(t);
    } else
      mul3(r, x, y);
  }

  // r.size() >= x.size(), !x.is_empty(), y != 0
  static void divide2(Con& r, const Con& x, digit_type y) {
    std::size_t n = x.size();
    LowLevel::quotrem_1(r.get(), x.get(), n, y);
    r.set_size(r[n-1] ? n : n-1);
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

public:

  typedef Con container_type;

  static void set(Con& r, digit_type x) {
    if (x) {
      if (!r.request_size(1))
        Con(1).swap(r);
      r[0] = x;
      r.set_size(1);
    } else
      r.set_size(0);
  }

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
    if (y.is_empty())             set(r, x);
    else if (x.is_empty())        set(r, y);
    else if (x.size() > y.size()) add2(r, x, y);
    else                          add2(r, y, x);
  }

  static void add(Con& r, const Con& x, digit_type y) {
    if (x.is_empty()) set(r, y);
    else if (!y)      set(r, x);
    else              add2(r, x, y);
  }

  static void multiply(Con& r, const Con& x, const Con& y) {
    if (y.is_empty())             set(r, digit_type(0));
    else if (x.is_empty())        set(r, digit_type(0));
    else if (x.size() > y.size()) mul2(r, x, y);
    else                          mul2(r, y, x);
  }

  static void multiply(Con& r, const Con& x, digit_type y) {
    if (x.is_empty() || !y) set(r, digit_type(0));
    else if (y == 1)        set(r, x);
    else                    mul2(r, x, y);
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

  static void divide(Con& r, const Con& u, digit_type v) {
    if (!v)
      division_by_zero();
    else if (u.is_empty())
      r.set_size(0);
    else {
      std::size_t n = u.size();
      if (!r.request_size(n)) {
        Con t(n);
        divide2(t, u, v);
        r.swap(t);
      } else 
        divide2(r, u, v);
    }
  }

  static inline digit_type divide(digit_type u, const Con& v) {
    if (v.is_empty())
      division_by_zero();
    else if (v.size() > 1)
      return 0;
    else
      return u / v[0];
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

  static inline digit_type remainder(const Con& u, digit_type v) {
    return LowLevel::rem_1(u.get(), u.size(), v);
  }

  static inline void remainder(Con& r, digit_type u, const Con& v) {
    if (v.is_empty())
      division_by_zero();
    else if (v.size() > 1 || v[0] > u)
      set(r, u);
    else
      set(r, u % v[0]);
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

  static inline digit_type quotrem(Con& r, const Con& u, digit_type v) {
    if (!v)
      division_by_zero();
    else if (u.is_empty()) {
      r.set_size(0);
      return v;
    } else {
      Con t(u.size());
      digit_type rem = divide2(t, u, v);
      r.swap(t);
      return rem;
    }
  }

  static inline digit_type quotrem(Con& r, digit_type u, const Con& v) {
    if (v.is_empty())
      division_by_zero();
    else if (v.size() > 1 || v[0] > u) {
      set(r, v);
      return 0;
    } else {
      std::pair<digit_type, digit_type> qr =
              sputsoft::math::number_theory::quotrem(u, v[0]);
      set(r, qr.second);
      return qr.first;
    }
  }

  static inline std::string to_string(const Con& x, unsigned base) {
    Con tmp(x);
    unsigned max_digits = tmp.size() * digit_bits / log2_floor(base) + 1;
    unsigned char st[max_digits];
    std::size_t used = LowLevel::to_chars(st, base, tmp.get(), tmp.size());
    for (unsigned i=0; i < used; ++i) st[i] += '0';
    st[used] = 0;
    return std::string((const char*)st, used);
  }

};

} // namespace detail
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_MIDLEVEL_HPP_

