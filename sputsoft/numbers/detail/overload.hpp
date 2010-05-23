/* 
 * File:   sputsoft/numbers/detail/overload.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-04-30 15:21Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_OVERLOAD_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_OVERLOAD_HPP

#include <sputsoft/numbers/detail/named_ops.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <class T>
struct resolve_ref {
  typedef T ref_type;
};

template <class T>
struct resolve_ref<numb<T> > {
  typedef const numb<T>& ref_type;
};

template <typename R, typename E> class expr;

template <typename Op, typename R1, typename E1, typename R2, typename E2>
struct resolve_binary<Op, expr<R1, E1>, expr<R2, E2> > {
  typedef typename resolve_binary<Op, R1, R2>::return_type return_type;
};

template <typename Op, typename R, typename E, typename T>
struct resolve_binary<Op, expr<R, E>, T> {
  typedef typename resolve_binary<Op, R, T>::return_type return_type;
};

template <typename Op, typename T, typename R, typename E>
struct resolve_binary<Op, T, expr<R, E> > {
  typedef typename resolve_binary<Op, T, R>::return_type return_type;
};

template <typename T>
struct resolve_type {
  typedef T type;
};

template <typename R, typename E>
struct resolve_type<expr<R, E> > {
  typedef R type;
};

template <typename Op, typename R, typename X, typename Y> struct expr_eval;

template <typename R, typename X, typename Y>
struct expr_eval<ops::binary::add, R, X, Y> {
  inline static void assign(R& r, const X& x, const Y& y) {
    sputsoft::numbers::add(r, (typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
  inline static R construct(const X& x, const Y& y) {
    return sputsoft::numbers::add((typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
};

template <typename R, typename X, typename Y>
struct expr_eval<ops::binary::sub, R, X, Y> {
  inline static void assign(R& r, const X& x, const Y& y) {
    sputsoft::numbers::sub(r, (typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
  inline static R construct(const X& x, const Y& y) {
    return sputsoft::numbers::sub((typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
};

template <typename R, typename X, typename Y>
struct expr_eval<ops::binary::mul, R, X, Y> {
  inline static void assign(R& r, const X& x, const Y& y) {
    sputsoft::numbers::mul(r, (typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
  inline static R construct(const X& x, const Y& y) {
    return sputsoft::numbers::mul((typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
};

template <typename R, typename X, typename Y>
struct expr_eval<ops::binary::div, R, X, Y> {
  inline static void assign(R& r, const X& x, const Y& y) {
    sputsoft::numbers::div(r, (typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
  inline static R construct(const X& x, const Y& y) {
    return sputsoft::numbers::div((typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
};

template <typename R, typename X, typename Y>
struct expr_eval<ops::binary::rem, R, X, Y> {
  inline static void assign(R& r, const X& x, const Y& y) {
    sputsoft::numbers::rem(r, (typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
  inline static R construct(const X& x, const Y& y) {
    return sputsoft::numbers::rem((typename resolve_type<X>::type) x, (typename resolve_type<Y>::type) y);
  }
};

template <typename Op, typename X, typename Y> struct binary;
template <typename Op, typename X, typename Y> struct unary;

template <typename R, typename Op, typename X, typename Y>
class expr<R, binary<Op, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { expr_eval<Op, R, X, Y>::assign(r, x, y); }
  inline operator R() const { return expr_eval<Op, R, X, Y>::construct(x, y); }
};

template <typename R1, typename R2, typename E2>
struct set_2_eval<numb<R1>, expr<R2, E2> > {
  static void set(numb<R1>& r, const expr<R2, E2>& v) {
    v.assign(r);
  }
};

#define SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(OVERLOAD, OPNAME, TYPE) \
template <typename R, typename E> \
expr<typename resolve_binary<ops::binary::OPNAME, TYPE, R>::return_type, \
     binary<ops::binary::OPNAME, TYPE, expr<R, E> > > \
OVERLOAD(const TYPE& x, const expr<R, E>& y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, TYPE, R>::return_type, \
     binary<ops::binary::OPNAME, TYPE, expr<R, E> > >(x, y); \
} \
template <typename R, typename E> \
expr<typename resolve_binary<ops::binary::OPNAME, R, TYPE>::return_type, \
     binary<ops::binary::OPNAME, expr<R, E>, TYPE> > \
OVERLOAD(const expr<R, E>& x, const TYPE& y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, R, TYPE>::return_type, \
     binary<ops::binary::OPNAME, expr<R, E>, TYPE> >(x, y); \
} \
template <typename N> \
expr<typename resolve_binary<ops::binary::OPNAME, numb<N>, TYPE>::return_type, \
     binary<ops::binary::OPNAME, numb<N>, TYPE> > \
OVERLOAD(const numb<N>& x, TYPE y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, numb<N>, TYPE>::return_type, \
     binary<ops::binary::OPNAME, numb<N>, TYPE> >(x, y); \
} \
template <typename N> \
expr<typename resolve_binary<ops::binary::OPNAME, TYPE, numb<N> >::return_type, \
     binary<ops::binary::OPNAME, TYPE, numb<N> > > \
OVERLOAD(TYPE x, const numb<N>& y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, TYPE, numb<N> >::return_type, \
     binary<ops::binary::OPNAME, TYPE, numb<N> > >(x, y); \
}

#define SPUTSOFT_NUMBERS_OVERLOAD_BINARY(OVERLOAD, OPNAME) \
template <typename R1, typename E1, typename R2, typename E2> \
expr<typename resolve_binary<ops::binary::OPNAME, R1, R2>::return_type, \
     binary<ops::binary::OPNAME, expr<R1, E1>, expr<R2, E2> > > \
OVERLOAD(const expr<R1, E1>& x, const expr<R2, E2>& y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, R1, R2>::return_type, \
     binary<ops::binary::OPNAME, expr<R1, E1>, expr<R2, E2> > >(x, y); \
} \
template <typename N1, typename R2, typename E2> \
expr<typename resolve_binary<ops::binary::OPNAME, numb<N1>, R2>::return_type, \
     binary<ops::binary::OPNAME, numb<N1>, expr<R2, E2> > > \
OVERLOAD(const numb<N1>& x, const expr<R2, E2>& y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, numb<N1>, R2>::return_type, \
     binary<ops::binary::OPNAME, numb<N1>, expr<R2, E2> > >(x, y); \
} \
template <typename R1, typename E1, typename N2> \
expr<typename resolve_binary<ops::binary::OPNAME, R1, numb<N2> >::return_type, \
     binary<ops::binary::OPNAME, expr<R1, E1>, numb<N2> > > \
OVERLOAD(const expr<R1, E1>& x, const numb<N2>& y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, R1, numb<N2> >::return_type, \
     binary<ops::binary::OPNAME, expr<R1, E1>, numb<N2> > >(x, y); \
} \
template <typename N1, typename N2> \
expr<typename resolve_binary<ops::binary::OPNAME, numb<N1>, numb<N2> >::return_type, \
     binary<ops::binary::OPNAME, numb<N1>, numb<N2> > > \
OVERLOAD(const numb<N1>& x, const numb<N2>& y) { \
  return expr<typename resolve_binary<ops::binary::OPNAME, numb<N1>, numb<N2> >::return_type, \
     binary<ops::binary::OPNAME, numb<N1>, numb<N2> > >(x, y); \
} \
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(OVERLOAD, OPNAME, unsigned short) \
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(OVERLOAD, OPNAME, signed short) \
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(OVERLOAD, OPNAME, unsigned) \
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(OVERLOAD, OPNAME, signed) \
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(OVERLOAD, OPNAME, unsigned long) \
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(OVERLOAD, OPNAME, signed long)

SPUTSOFT_NUMBERS_OVERLOAD_BINARY(operator+, add)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY(operator-, sub)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY(operator*, mul)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY(operator/, div)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY(operator%, rem)

#ifdef SPUTSOFT_HAS_LONG_LONG
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator+, add, unsigned long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator+, add, signed long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator-, sub, unsigned long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator-, sub, signed long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator*, mul, unsigned long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator*, mul, signed long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator/, div, unsigned long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator/, div, signed long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator%, rem, unsigned long long)
SPUTSOFT_NUMBERS_OVERLOAD_BINARY_INT(operator%, rem, signed long long)
#endif


template <typename N, typename T>
numb<N>& operator+=(numb<N>& x, const T& y) {
  return x = x + y;
}

template <typename N, typename T>
numb<N>& operator-=(numb<N>& x, const T& y) {
  return x = x - y;
}

template <typename N, typename T>
numb<N>& operator*=(numb<N>& x, const T& y) {
  return x = x * y;
}

template <typename N, typename T>
numb<N>& operator/=(numb<N>& x, const T& y) {
  return x = x / y;
}

template <typename N, typename T>
numb<N>& operator%=(numb<N>& x, const T& y) {
  return x = x % y;
}


} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_OVERLOAD_HPP
