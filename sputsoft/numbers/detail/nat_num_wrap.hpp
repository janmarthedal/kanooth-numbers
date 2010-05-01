/* 
 * File:   nat_num_wrap.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:18 PM
 */

#ifndef _NAT_NUM_WRAP_HPP
#define	_NAT_NUM_WRAP_HPP

#include <sputsoft/numbers/detail/expr.hpp>

#define SPUTSOFT_HAS_LONG_LONG

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Con, typename LowLevel>
class nat_num_wrap;

template <typename Con, typename LowLevel>
class expr<nat_num_wrap<Con, LowLevel> > {
private:
  Con con;
  typedef typename Con::digit_type digit_type;
  static const unsigned digit_bits = boost::integer_traits<digit_type>::digits;

  static void ensure_size(Con& c, unsigned size) {
    if (!c.request_size(size))
      Con(size).swap(c);
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
  static void sub_int(Con& r, const Con& x, const T y) {
    if (x.is_empty())
      r.set_size(0);
    else if (!y)
      set_num(r, x);
    else if (sizeof(T) <= sizeof(digit_type))
      sub_int1(r, x, y);
    else
      sub_num(r, x, expr(y).con);
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
  static void mult_int(Con& r, const Con& x, const T y) {
    if (x.is_empty() || !y)
      r.set_size(0);
    else if (y == T(1))
      set_int(r, T(0));
    else if (sizeof(T) <= sizeof(digit_type))
      mul_int1(r, x, y);
    else
      mul_num(r, x, expr(y).con);
  }

public:
  digit_type get() const {
    return con.is_empty() ? 0 : con[0];
  }  // DEBUG

  expr() : con() {}
  expr(const expr& rhs) : con() {
    set_num(con, rhs.con);
  }
  expr& operator=(const expr& rhs) {
    set_num(con, rhs.con);
    return *this;
  }

  expr(unsigned short v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned short v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned short y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned short x, const expr& y) { add_int(r.con, y.con, x); }
  static inline void sub(expr& r, const expr& x, unsigned short y) { sub_int(r.con, x.con, y); }
  static inline void mul(expr& r, const expr& x, unsigned short y) { mul_int(r.con, x.con, y); }

  expr(unsigned v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned x, const expr& y) { add_int(r.con, y.con, x); }
  static inline void sub(expr& r, const expr& x, unsigned y) { sub_int(r.con, x.con, y); }
  static inline void mul(expr& r, const expr& x, unsigned y) { mul_int(r.con, x.con, y); }

  expr(unsigned long v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned long v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned long y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned long x, const expr& y) { add_int(r.con, y.con, x); }
  static inline void sub(expr& r, const expr& x, unsigned long y) { sub_int(r.con, x.con, y); }
  static inline void mul(expr& r, const expr& x, unsigned long y) { mul_int(r.con, x.con, y); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  expr(unsigned long long v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned long long v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned long long y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned long long x, const expr& y) { add_int(r.con, y.con, x); }
  static inline void sub(expr& r, const expr& x, unsigned long long y) { sub_int(r.con, x.con, y); }
  static inline void mul(expr& r, const expr& x, unsigned long long y) { mul_int(r.con, x.con, y); }
#endif

  static inline bool is_zero(const expr& u) { return u.con.is_empty(); }
  static inline void set(expr& r, const expr& x) { set_num(r.con, x.con); }
  static inline void add(expr& r, const expr& x, const expr& y) { add_num(r.con, x.con, y.con); }
  static inline void sub(expr& r, const expr& x, const expr& y) { sub_num(r.con, x.con, y.con); }
  static inline void mul(expr& r, const expr& x, const expr& y) { mul_num(r.con, x.con, y.con); }

};

} // namespace sputsoft
} // namespace detail
} // namespace numbers

#endif	/* _NAT_NUM_WRAP_HPP */

