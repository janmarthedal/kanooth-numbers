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

  // r.size() >= x.size(), !x.is_empty(), y != 0
  static void divide2(Con& r, const Con& x, digit_type y) {
    std::size_t n = x.size();
    LowLevel::divrem_1(r.get(), x.get(), n, y);
    r.set_size(r[n-1] ? n : n-1);
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

  static inline digit_type remainder(const Con& u, digit_type v) {
    return LowLevel::rem_1(u.get(), u.size(), v);
  }

  static inline void remainder(Con& r, digit_type u, const Con& v) {
    if (v.is_empty())
      division_by_zero();
    else if (v.size() > 1 || v[0] > u)
      set(r, v);
    else
      set(r, u % v[0]);
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

