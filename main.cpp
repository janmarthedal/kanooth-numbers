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


template <typename R, typename E=R> class expr;

template <typename Con, typename LowLevel>
class nat_num_wrap;

template <typename Con, typename LowLevel, typename D>
class array_arith_1_noconv {
private:

public:

  static void set(expr<nat_num_wrap<Con, LowLevel> >& r, const D x) {
    if (x) {
      if (!r.con.request_size(1))
        Con(1).swap(r.con);
      r.con[0] = x;
      r.con.set_size(1);
    } else
      r.con.set_size(0);
  }

};

/*template <typename Con, typename D, typename B>
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
};*/

template <typename Con, typename LowLevel, typename D, typename B, bool Cnv>
class array_arith_1;

// Argument type equal to or smaller than digit type:

template <typename Con, typename LowLevel, typename D, typename B>
class array_arith_1<Con, LowLevel, D, B, false>
 : public array_arith_1_noconv<Con, LowLevel, D> {};

// Argument type larger than digit type:

template <typename Con, typename LowLevel, typename D, typename B>
class array_arith_1<Con, LowLevel, D, B, true> {
public:

  static inline void set(expr<nat_num_wrap<Con, LowLevel> >& r, const B u) {
    expr<nat_num_wrap<Con, LowLevel> >::set(r, expr<nat_num_wrap<Con, LowLevel> >(u, true));
  }

};

template <typename Con, typename LowLevel>
class expr<nat_num_wrap<Con, LowLevel> > {
  template <typename _Con, typename _LowLevel, typename _D>
  friend class array_arith_1_noconv;
  template <typename _Con, typename _LowLevel, typename _D, typename _B, bool _Cnv>
  friend class array_arith_1;
private:
  Con con;
  typedef typename Con::digit_type digit_type;
  static const unsigned digit_bits = boost::integer_traits<digit_type>::digits;
  static const unsigned short_bits = boost::integer_traits<unsigned short>::digits;
  static const unsigned int_bits = boost::integer_traits<unsigned>::digits;
  static const unsigned long_bits = boost::integer_traits<unsigned long>::digits;
  typedef array_arith_1<Con, LowLevel, digit_type, unsigned, digit_bits < int_bits> array_arith_1_int;

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

  template <typename B>
  expr(B v, bool) : con(boost::integer_traits<B>::digits / digit_bits) {
    static const B mask = (B(1) << digit_bits) - 1;
    unsigned n = 0;
    while (v) {
      con[n++] = v & mask;
      v >>= digit_bits;
    }
    con.set_size(n);    
  }

public:
  expr() : con() {}
  const expr& eval() const { return *this; }
  
  digit_type get() const {
    return con.is_empty() ? 0 : con[0];
  }  // DEBUG

  static inline bool is_zero(const expr& u) {
    return u.con.is_empty();
  }
  static void set(expr& r, const expr& x) {
    std::size_t n = x.con.size();
    if (!r.con.request_size(n)) {
      Con t(n);
      LowLevel::copy(t.get(), x.con.get(), n);
      r.con.swap(t);
    } else
      LowLevel::copy(r.con.get(), x.con.get(), x.con.size());
    r.con.set_size(n);
  }
  static void add(expr& r, const expr& x, const expr& y) {
    if (is_zero(y))                        set(r, x);
    else if (is_zero(x))                   set(r, y);
    else if (x.con.size() >= y.con.size()) add2(r.con, x.con, y.con);
    else                                   add2(r.con, x.con, x.con);
  }
  static inline void set(expr& r, unsigned u) {
    array_arith_1_int::set(r, u);
  }
};



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

template <typename R, typename R1, typename V1, typename R2, typename V2>
class evaluator_ref1_arg2<expr<R, R>, expr<R1, V1>, expr<R2, V2> > {
public:
  static void add(expr<R, R>& r, const expr<R1, V1>& v1, const expr<R2, V2>& v2) {
    expr<R, R>::add(r, v1.eval(), v2.eval());
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


typedef expr<
          nat_num_wrap<
            sputsoft::numbers::detail::digit_array<unsigned short>,
            sputsoft::numbers::generic::lowlevel<sputsoft::numbers::detail::array_allocator<void> >
          >
        > natural_number;

int main()
{
  natural_number a, b, c;

  set(b, 2u);
  set(a, b);
  add(c, a, b);

  std::cout << c.get() << std::endl;

  return 0;
}
