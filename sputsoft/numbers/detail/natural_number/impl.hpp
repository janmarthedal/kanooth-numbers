/* 
 * File:   sputsoft/numbers/detail/nat_num_impl.hpp
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

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_IMPL_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_IMPL_HPP

#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/detail/nat_num_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Con, typename LowLevel>
class wrap2;

template <typename Con, typename LowLevel>
class numb<natnum<wrap2<Con, LowLevel> > > {
public:
  typedef typename Con::digit_type digit_type;
  static const unsigned digit_bits = sputsoft::number_bits<digit_type>::value;
private:
  Con con;

  static int division_by_zero() {
    int y = 0;  // hide warning
    return 1/y;
  }

  static void ensure_size(Con& c, unsigned size) {
    if (!c.request_size(size))
      Con(size).swap(c);
  }

  template <typename T, bool fits> struct to_int_type_spec;
  template <typename T> struct to_int_type_spec<T, true> {
    static inline T to_int_type(const Con& c) {
      return (T) c[0];
    }
  };
  template <typename T> struct to_int_type_spec<T, false> {
    static inline T to_int_type(const Con& c) {
      T v = 0;
      unsigned n = c.size();
      while (n)
        v = (v << digit_bits) | c[--n];
      return v;
    }
  };

  template <typename T>
  static T to_int_type(const Con& c) {
    if (c.is_empty()) return (T) 0;
    return to_int_type_spec<T, sizeof(T) <= sizeof(digit_type)>::to_int_type(c);
  }

  /* Set to number */

  static void set_num(Con& r, const Con& x) {
    if (r.get() != x.get()) {
      std::size_t n = x.size();
      ensure_size(r, n);
      LowLevel::copy_forward(r.get(), x.get(), n);
      r.set_size(n);
    }
  }

  /* Set to integer */

  template <typename T, bool fits> struct set_int_spec;
  template <typename T> struct set_int_spec<T, true> {
    static inline void set_int(Con& r, T v) {
      ensure_size(r, 1);
      r[0] = v;
      r.set_size(1);
    }
  };
  template <typename T> struct set_int_spec<T, false> {
    static inline void set_int(Con& r, T v) {
      static const T mask = (T(1) << digit_bits) - 1;
      unsigned n = 0;
      ensure_size(r, sizeof(T) / sizeof(digit_type));
      while (v) {
        r[n++] = v & mask;
        v >>= digit_bits;
      }
      r.set_size(n);
    }
  };

  template <typename T>
  static void set_int(Con& r, T v) {
    if (!v)
      r.set_size(0);
    else
      set_int_spec<T, sizeof(T) <= sizeof(digit_type)>::set_int(r, v);
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
      add_num(r, x, numb(y).con);
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
      sub_num(r, x, numb(y).con);
  }

  template <typename T>
  static T sub_int_num(const T x, const Con& y) {
    return sputsoft::numbers::sub(x, to_int_type<T>(y));
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
      set_num(z, x);
    else if (sizeof(T) <= sizeof(digit_type))
      mul_int1(z, x, y);
    else
      mul_num(z, x, numb(y).con);
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

  static void div_num(Con& q, const Con& u, const Con& v) {
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
      quotrem_num(q, r, u, numb(v).con);
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
        quotrem_num(q, r, u, numb(v).con);
        return to_int_type<T>(r);
      }
    }
  }

  template <typename T>
  static T quotrem_i(Con& q, const Con& u, const T v) {
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
      quotrem_num(q, r, u, numb(v).con);
      return to_int_type<T>(r);
    }
  }

  /* Bitwise and two numbers */

  // x.size() >= y.size() >= 1
  static void bit_and_num0(Con& r, const Con& x, const Con& y) {
    std::size_t n = y.size();
    LowLevel::bitwise_and(r.get(), x.get(), y.get(), n);
    while (n && !r[n-1]) --n;
    r.set_size(n);
  }

  // x.size() >= y.size() >= 1
  static void bit_and_num1(Con& r, const Con& x, const Con& y) {
    std::size_t n = y.size();
    if (!r.request_size(n)) {
      Con t(n);
      bit_and_num0(t, x, y);
      r.swap(t);
    } else
      bit_and_num0(r, x, y);
  }

  static void bit_and_num(Con& r, const Con& x, const Con& y) {
    if (y.is_empty() || x.is_empty()) r.set_size(0);
    else if (x.size() >= y.size())    bit_and_num1(r, x, y);
    else                              bit_and_num1(r, y, x);
  }

  /* Bitwise and number and integer */

  template <typename T>
  static T bit_and_int(const Con& x, const T y) {
    if (x.is_empty())
      return 0;
    else if (sizeof(T) <= sizeof(digit_type))
      return sputsoft::numbers::bitwise_and((T) x[0], y);
    else {
      Con r(sizeof(T) / sizeof(digit_type));
      bit_and_num(r, x, numb(y).con);
      return to_int_type<T>(r);
    }
  }

  // Bitwise or two numbers

  // x.size() >= y.size() >= 1
  static void bit_or_num0(Con& r, const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    LowLevel::bitwise_or(r.get(), x.get(), y.get(), yn);
    LowLevel::copy_forward(r.get() + yn, x.get() + yn, xn - yn);
    r.set_size(xn);
  }

  // x.size() >= y.size() >= 1
  static void bit_or_num1(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    if (!r.request_size(n)) {
      Con t(n);
      bit_or_num0(t, x, y);
      r.swap(t);
    } else
      bit_or_num0(r, x, y);
  }

  static void bit_or_num(Con& r, const Con& x, const Con& y) {
    if (x.is_empty())              set_num(r, y);
    else if (y.is_empty())         set_num(r, x);
    else if (x.size() >= y.size()) bit_or_num1(r, x, y);
    else                           bit_or_num1(r, y, x);
  }

  // Bitwise or number and integer

  template <typename T>
  static void bit_or_int(Con& r, const Con& x, const T y) {
    if (x.is_empty())
      set_int(r, y);
    else if (sizeof(T) <= sizeof(digit_type)) {
      set_num(r, x);
      sputsoft::numbers::bitwise_or(r[0], r[0], y);
    } else
      bit_or_num(r, x, numb(y).con);
  }

  // Bitwise xor two numbers

  // x.size() >= y.size() >= 1
  static void bit_xor_num0(Con& r, const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    LowLevel::bitwise_xor(r.get(), x.get(), y.get(), yn);
    if (xn == yn)
      while (xn && !r[xn-1]) --xn;
    else
      LowLevel::copy_forward(r.get() + yn, x.get() + yn, xn - yn);
    r.set_size(xn);
  }

  // x.size() >= y.size() >= 1
  static void bit_xor_num1(Con& r, const Con& x, const Con& y) {
    std::size_t n = x.size();
    if (!r.request_size(n)) {
      Con t(n);
      bit_xor_num0(t, x, y);
      r.swap(t);
    } else
      bit_xor_num0(r, x, y);
  }

  static void bit_xor_num(Con& r, const Con& x, const Con& y) {
    if (x.is_empty())              set_num(r, y);
    else if (y.is_empty())         set_num(r, x);
    else if (x.size() >= y.size()) bit_xor_num1(r, x, y);
    else                           bit_xor_num1(r, y, x);
  }

  // Bitwise or number and integer

  template <typename T>
  static void bit_xor_int(Con& r, const Con& x, const T y) {
    if (x.is_empty())
      set_int(r, y);
    else if (sizeof(T) <= sizeof(digit_type)) {
      set_num(r, x);
      sputsoft::numbers::bitwise_xor(r[0], r[0], y);
    } else
      bit_xor_num(r, x, numb(y).con);
  }

  // Bitwise and not two numbers

  static void bit_and_not_num0(Con& r, const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    if (yn < xn) {
      LowLevel::bitwise_and_not(r.get(), x.get(), y.get(), yn);
      LowLevel::copy_forward(r.get() + yn, x.get() + yn, xn - yn);
    } else {
      LowLevel::bitwise_and_not(r.get(), x.get(), y.get(), xn);
      while (xn && !r[xn-1]) --xn;
    }
    r.set_size(xn);
  }

  static void bit_and_not_num(Con& r, const Con& x, const Con& y) {
    std::size_t xn = x.size();
    if (!xn)
      r.set_size(0);
    else if (y.is_empty())
      set_num(r, x);
    else if (!r.request_size(xn)) {
      Con t(xn);
      bit_and_not_num0(t, x, y);
      r.swap(t);
    } else
      bit_and_not_num0(r, x, y);
  }

  // Bitwise and number and integer

  template <typename T>
  static T bit_and_not_int_num(const T x, const Con& y) {
    if (!x)
      return 0;
    else if (y.is_empty())
      return x;
    else if (sizeof(T) <= sizeof(digit_type))
      return sputsoft::numbers::bitwise_and_not(x, (T) y[0]);
    else {
      Con r(sizeof(T) / sizeof(digit_type));
      bit_and_not_num(r, numb(x).con, y);
      return to_int_type<T>(r);
    }
  }

  template <typename T>
  static void bit_and_not_num_int(Con& r, const Con& x, const T y) {
    if (x.is_empty())
      r.set_size(0);
    else if (!y)
      set_num(r, x);
    else if (sizeof(T) <= sizeof(digit_type)) {
      set_num(r, x);
      sputsoft::numbers::bitwise_and_not(r[0], r[0], (digit_type) y);
    } else
      bit_and_not_num(r, x, numb(y).con);
  }

  // Compare two numbers

  template <typename T>
  static inline int compare_ints(const T a, const T b) {
    return a < b ? -1 : a > b ? 1 : 0;
  }

  static int comp_num(const Con& x, const Con& y) {
    std::size_t xn = x.size();
    std::size_t yn = y.size();
    if (xn != yn) return compare_ints(xn, yn);
    if (!xn) return 0;
    return LowLevel::comp(x.get(), y.get(), xn);
  }

  // Compare numbers to integer

  template <typename T>
  static int comp_int(const Con& x, const T v) {
    if (!v)
      return x.is_empty() ? 0 : 1;
    else if (x.is_empty())
      return -1;
    else if (sizeof(T) <= sizeof(digit_type))
      return x.size() > 1 ? 1 : compare_ints(x[0], (digit_type) v);
    else
      return comp_num(x, numb(v).con);
  }

  // Binary shift left

  static void left_shift_num0(Con& z, const Con& u, std::size_t count) {
    unsigned whole = count / digit_bits;
    count %= digit_bits;
    std::size_t zn = u.size() + whole;
    if (count) {
      digit_type s = LowLevel::lshift(z.get() + whole, u.get(), u.size(), count);
      if (s) z[zn++] = s;
    } else
      LowLevel::copy_backward(z.get() + whole, u.get(), u.size());
    LowLevel::fill_zero(z.get(), whole);
    z.set_size(zn);
  }

  static void left_shift_num(Con& z, const Con& u, std::size_t count) {
    if (u.is_empty())
      z.set_size(0);
    else if (!count)
      set_num(z, u);
    else {
      std::size_t n = u.size() + (count + digit_bits - 1) / digit_bits;
      if (!z.request_size(n)) {
        Con t(n);
        left_shift_num0(t, u, count);
        z.swap(t);
      } else
        left_shift_num0(z, u, count);
    }
  }

  // Binary shift right

  static void right_shift_num0(Con& z, const Con& u, std::size_t count) {
    unsigned whole = count / digit_bits;
    count %= digit_bits;
    std::size_t zn = u.size() - whole;
    if (count) {
      LowLevel::rshift(z.get(), u.get() + whole, zn, count);
      if (!z[zn-1]) --zn;
    } else
      LowLevel::copy_forward(z.get(), u.get() + whole, zn);
    z.set_size(zn);
  }

  static void right_shift_num(Con& z, const Con& u, std::size_t count) {
    if (u.is_empty() || count >= u.size()*digit_bits)
      z.set_size(0);
    else if (!count)
      set_num(z, u);
    else {
      std::size_t n = u.size() - count/digit_bits;
      if (!z.request_size(n)) {
        Con t(n);
        right_shift_num0(t, u, count);
        z.swap(t);
      } else
        right_shift_num0(z, u, count);
    }
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
  inline bool is_zero() const { return con.is_empty(); }
  inline operator bool() const { return !is_zero(); }

  inline void set(const numb& x) { set_num(con, x.con); }
  template <typename T>
  inline void set(T v) { set_int(con, to_unsigned<T>(v)); }

  inline void add(const numb& x, const numb& y) { add_num(con, x.con, y.con); }
  template <typename T>
  inline void add(const numb& x, T y) { add_int(con, x.con, to_unsigned<T>(y)); }
  template <typename T>
  inline void add(T x, const numb& y) { add_int(con, y.con, to_unsigned<T>(x)); }

  inline void sub(const numb& x, const numb& y) { sub_num(con, x.con, y.con); }
  template <typename T>
  inline void sub(const numb& x, T y) { sub_num_int(con, x.con, y); }
  template <typename T>
  static inline T sub(T x, const numb& y) { return sub_int_num(x, y.con); }

  inline void mul(const numb& x, const numb& y) { mul_num(con, x.con, y.con); }
  template <typename T>
  inline void mul(const numb& x, T y) { mul_int(con, x.con, y); }
  template <typename T>
  inline void mul(T x, const numb& y) { mul_int(con, y.con, x); }

  inline void div(const numb& x, const numb& y) { div_num(con, x.con, y.con); }
  template <typename T>
  inline void div(const numb& x, T y) { quot_int(con, x.con, y); }

  inline void rem(const numb& x, const numb& y) { rem_num(con, x.con, y.con); }
  template <typename T>
  static inline T rem(const numb& x, T y) { return rem_int(x.con, y); }

  static inline void divrem(numb& q, numb& r, const numb& x, const numb& y) {
    quotrem_num(q.con, r.con, x.con, y.con);
  }
  template <typename T>
  inline T divrem(const numb& x, T y) { return quotrem_i(con, x.con, y); }

  inline int cmp(const numb& v) const { return comp_num(con, v.con); }
  template <typename T>
  inline int cmp(T v) const {
    return (is_signed<T>::value && v < 0) ? 1 : comp_int(con, to_unsigned(v));
  }

  inline void bitwise_and(const numb& x, const numb& y) { bit_and_num(con, x.con, y.con); }
  template <typename T>
  static inline T bitwise_and(const numb& x, T y) { return bit_and_int(x.con, y); }
  template <typename T>
  static inline T bitwise_and(T x, const numb& y) { return bit_and_int(y.con, x); }

  inline void bitwise_or(const numb& x, const numb& y) { bit_or_num(con, x.con, y.con); }
  template <typename T>
  inline void bitwise_or(const numb& x, T y) { bit_or_int(con, x.con, y); }
  template <typename T>
  inline void bitwise_or(T x, const numb& y) { bit_or_int(con, y.con, x); }

  inline void bitwise_xor(const numb& x, const numb& y) { bit_xor_num(con, x.con, y.con); }
  template <typename T>
  inline void bitwise_xor(const numb& x, T y) { bit_or_int(con, x.con, y); }
  template <typename T>
  inline void bitwise_xor(T x, const numb& y) { bit_or_int(con, y.con, x); }

  inline void bitwise_and_not(const numb& x, const numb& y) { bit_and_not_num(con, x.con, y.con); }
  template <typename T>
  inline void bitwise_and_not(const numb& x, T y) { bit_and_not_num_int(con, x.con, y); }
  template <typename T>
  static inline T bitwise_and_not(T x, const numb& y) { return bit_and_not_int_num(x, y.con); }

  inline void shift_left(const numb& x, std::ptrdiff_t count) {
    if (count >= 0)
      left_shift_num(con, x.con, count);
    else
      right_shift_num(con, x.con, -count);
  }

  std::size_t floor_log2() const {
    std::size_t n = con.size();
    if (!n) return (std::size_t) -1;
    return sputsoft::numbers::floor_log2(con[n - 1]) + (n - 1)*digit_bits;
  }

};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_IMPL_HPP */
