/*
 * File:   main.cpp
 * Author: jmr
 *
 * Created on December 18, 2009, 10:48 AM
 */

#include <iostream>
#include <boost/integer_traits.hpp>
#include <sputsoft/numbers/detail/digit_array.hpp>
#include <sputsoft/numbers/generic/lowlevel.hpp>
#include <sputsoft/number_theory/common.hpp>
#include <sputsoft/types.hpp>


/* Global arithmetic methods */

template <typename R, typename V1, typename V2>
class evaluator_ref1_arg2;
template <typename R, typename V>
class evaluator_ref1_arg1;

template <typename R, typename V1, typename V2>
void add(R& r, const V1& v1, const V2& v2) {
  evaluator_ref1_arg2<R, V1, V2>::add(r, v1, v2);
}

template <typename R, typename V>
void set(R& r, const V& v) {
  evaluator_ref1_arg1<R, V>::set(r, v);
}

/* Abstract expressions */

template <typename R, typename E=R> class expr;

template <typename R, typename R1, typename E1, typename R2, typename E2>
class evaluator_ref1_arg2<expr<R, R>, expr<R1, E1>, expr<R2, E2> > {
public:
  static void add(expr<R, R>& r, const expr<R1, E1>& e1, const expr<R2, E2>& e2) {
    expr<R, R>::add(r, e1.eval(), e2.eval());
  }
};

template <typename R, typename R1, typename E1, typename V2>
class evaluator_ref1_arg2<expr<R, R>, expr<R1, E1>, V2> {
public:
  static void add(expr<R, R>& r, const expr<R1, E1>& e1, const V2& v2) {
    expr<R, R>::add(r, e1.eval(), v2);
  }
};

template <typename R, typename V1, typename R2, typename E2>
class evaluator_ref1_arg2<expr<R, R>, V1, expr<R2, E2> > {
public:
  static void add(expr<R, R>& r, const V1& v1, const expr<R2, E2>& e2) {
    expr<R, R>::add(r, e2.eval(), v1);
  }
};

template <typename R, typename V>
class evaluator_ref1_arg1<expr<R, R>, V> {
public:
  static void set(expr<R, R>& r, const V& v) {
    expr<R, R>::set(r, v);
  }
};

template <typename R, typename R1, typename V1>
class evaluator_ref1_arg1<expr<R, R>, expr<R1, V1> > {
public:
  static void set(expr<R, R>& r, const expr<R1, V1>& v) {
    expr<R, R>::set(r, v.eval());
  }
};

/* Array arithmetic natural number */

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
  static inline void add_int(Con& r, const Con& x, const T y) {
    if (x.is_empty())
      set_int(r, y);
    else if (!y)
      set_num(r, x);
    else if (sizeof(T) <= sizeof(digit_type))
      add_int1(r, x, y);
    else
      add_num(r, x, expr(y).con);
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

public:
  const expr& eval() const { return *this; }
  digit_type get() const {
    return con.is_empty() ? 0 : con[0];
  }  // DEBUG

  expr() : con() {}

  expr(unsigned short v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned short v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned short y) { add_int(r.con, x.con, y); }

  expr(unsigned v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned y) { add_int(r.con, x.con, y); }

  expr(unsigned long v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned long v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned long y) { add_int(r.con, x.con, y); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  expr(unsigned long long v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned long long v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned long long y) { add_int(r.con, x.con, y); }
#endif

  static inline bool is_zero(const expr& u) {
    return u.con.is_empty();
  }
  static inline void set(expr& r, const expr& x) {
    set_num(r.con, x.con);
  }
  static inline void add(expr& r, const expr& x, const expr& y) {
    add_num(r.con, x.con, y.con);
  }
};

typedef expr<
          nat_num_wrap<
            sputsoft::numbers::detail::digit_array<unsigned long>,
            sputsoft::numbers::generic::lowlevel<sputsoft::numbers::detail::array_allocator<void> >
          >
        > natural_number;

/* Operator overloading */

namespace ops {
  namespace binary {
    class add {};
    class subtract {};
    class multiply {};
    class divide {};
    class remainder {};
    class lshift {};
    class rshift {};
  }
  namespace unary {
    class negate {};
  }
}

template <class T>
struct resolve_ref {
  typedef T ref_type;
};
template <class R>
struct resolve_ref<expr<R, R> > {
  typedef const expr<R, R>& ref_type;
};

template <typename Op, typename X, typename Y>
class binary {
private:
  binary() {}
public:
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  binary(const X& _x, const Y& _y) : x(_x), y(_y) {}
};

template <typename Op, typename X>
class unary {
private:
  unary() {}
public:
  typename resolve_ref<X>::ref_type x;
  unary(const X& _x) : x(_x) {}
};


/* Test program */

int main()
{
  natural_number a, b=2u, c;

  set(c, 10u);
  set(a, b);
  add(c, b, 3u);
  add(c, 20u, b);

  std::cout << a.get() << std::endl;
  std::cout << c.get() << std::endl;

  return 0;
}
