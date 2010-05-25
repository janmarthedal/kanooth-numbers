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

template <typename Op, typename X, typename Y> struct binary;
template <typename Op, typename X> struct unary;

/*template <typename R, typename R2, typename X, typename Y>
struct expr<R, binary<ops::binary::add, X, expr<R2, unary<ops::unary::negate, Y> > > > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
  expr(const X& _x, const expr<R2, unary<ops::unary::negate, Y> >& _y) : x(_x), y(_y.x) {}
  inline void assign(R& r) const { sputsoft::numbers::sub(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::sub((x_type) x, (y_type) y); }
};*/

template <typename R, typename X, typename Y>
struct expr<R, binary<ops::binary::add, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::add(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::add((x_type) x, (y_type) y); }
};

template <typename R, typename X, typename Y>
class expr<R, binary<ops::binary::sub, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::sub(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::sub((x_type) x, (y_type) y); }
};

template <typename R, typename X, typename Y>
class expr<R, binary<ops::binary::mul, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::mul(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::mul((x_type) x, (y_type) y); }
};

template <typename R, typename X, typename Y>
class expr<R, binary<ops::binary::div, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::div(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::div((x_type) x, (y_type) y); }
};

template <typename R, typename X, typename Y>
class expr<R, binary<ops::binary::rem, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::rem(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::rem((x_type) x, (y_type) y); }
};

template <typename R, typename X, typename Y>
class expr<R, binary<ops::binary::lshift, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::binary_shift_left(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::binary_shift_left((x_type) x, (y_type) y); }
};

template <typename R, typename X, typename Y>
class expr<R, binary<ops::binary::rshift, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::binary_shift_right(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::binary_shift_right((x_type) x, (y_type) y); }
};

template <typename R, typename X>
struct expr<R, unary<ops::unary::negate, X> > {
  typename resolve_ref<X>::ref_type x;
  typedef typename resolve_type<X>::type x_type;
  expr(const X& _x) : x(_x) {}
  inline void assign(R& r) const { sputsoft::numbers::negate(r, (x_type) x); }
  inline operator R() const { return sputsoft::numbers::negate((x_type) x); }
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

#define SPUTSOFT_NUMBERS_OVERLOAD_SHIFT(TYPE) \
template <typename R, typename E> \
expr<typename resolve_binary<ops::binary::lshift, R, TYPE>::return_type, \
     binary<ops::binary::lshift, expr<R, E>, TYPE> > \
operator<<(const expr<R, E>& x, TYPE y) { \
  return expr<typename resolve_binary<ops::binary::lshift, R, TYPE>::return_type, \
     binary<ops::binary::lshift, expr<R, E>, TYPE> >(x, y); \
} \
template <typename N> \
expr<typename resolve_binary<ops::binary::lshift, numb<N>, TYPE>::return_type, \
     binary<ops::binary::lshift, numb<N>, TYPE> > \
operator<<(const numb<N>& x, TYPE y) { \
  return expr<typename resolve_binary<ops::binary::lshift, numb<N>, TYPE>::return_type, \
     binary<ops::binary::lshift, numb<N>, TYPE> >(x, y); \
} \
template <typename R, typename E> \
expr<typename resolve_binary<ops::binary::rshift, R, TYPE>::return_type, \
     binary<ops::binary::rshift, expr<R, E>, TYPE> > \
operator>>(const expr<R, E>& x, TYPE y) { \
  return expr<typename resolve_binary<ops::binary::rshift, R, TYPE>::return_type, \
     binary<ops::binary::rshift, expr<R, E>, TYPE> >(x, y); \
} \
template <typename N> \
expr<typename resolve_binary<ops::binary::rshift, numb<N>, TYPE>::return_type, \
     binary<ops::binary::rshift, numb<N>, TYPE> > \
operator>>(const numb<N>& x, TYPE y) { \
  return expr<typename resolve_binary<ops::binary::rshift, numb<N>, TYPE>::return_type, \
     binary<ops::binary::rshift, numb<N>, TYPE> >(x, y); \
}

SPUTSOFT_NUMBERS_OVERLOAD_SHIFT(unsigned short)
SPUTSOFT_NUMBERS_OVERLOAD_SHIFT(signed short)
SPUTSOFT_NUMBERS_OVERLOAD_SHIFT(unsigned)
SPUTSOFT_NUMBERS_OVERLOAD_SHIFT(signed)
SPUTSOFT_NUMBERS_OVERLOAD_SHIFT(unsigned long)
SPUTSOFT_NUMBERS_OVERLOAD_SHIFT(signed long)

template <typename N>
expr<typename resolve_unary<ops::unary::negate, numb<N> >::return_type,
     unary<ops::unary::negate, numb<N> > >
operator-(const numb<N>& x) {
  return expr<typename resolve_unary<ops::unary::negate, numb<N> >::return_type,
     unary<ops::unary::negate, numb<N> > >(x);
}

template <typename N, typename T>
inline numb<N>& operator+=(numb<N>& x, const T& y) {
  return x = x + y;
}

template <typename N, typename T>
inline numb<N>& operator-=(numb<N>& x, const T& y) {
  return x = x - y;
}

template <typename N, typename T>
inline numb<N>& operator*=(numb<N>& x, const T& y) {
  return x = x * y;
}

template <typename N, typename T>
inline numb<N>& operator/=(numb<N>& x, const T& y) {
  return x = x / y;
}

template <typename N, typename T>
inline numb<N>& operator%=(numb<N>& x, const T& y) {
  return x = x % y;
}

template <typename N, typename T>
inline numb<N>& operator<<=(numb<N>& x, const T& y) {
  return x = x << y;
}

template <typename N, typename T>
inline numb<N>& operator>>=(numb<N>& x, const T& y) {
  return x = x >> y;
}

template <typename R1, typename E1, typename R2, typename E2>
inline bool operator==(const expr<R1, E1>& x, const expr<R2, E2>& y) {
  return sputsoft::numbers::equal(x, y);
}
template <typename R, typename E, typename N>
inline bool operator==(const expr<R, E>& x, const numb<N>& y) {
  return sputsoft::numbers::equal(x, y);
}
template <typename R, typename E, typename N>
inline bool operator==(const numb<N>& x, const expr<R, E>& y) {
  return sputsoft::numbers::equal(x, y);
}
template <typename N1, typename N2>
inline bool operator==(const numb<N1>& x, const numb<N2>& y) {
  return sputsoft::numbers::equal(x, y);
}

#define SPUTSOFT_NUMBERS_EQUAL_INT(TYPE) \
template <typename R, typename E> \
inline bool operator==(const expr<R, E>& x, TYPE y) { return sputsoft::numbers::equal(x, y); } \
template <typename R, typename E> \
inline bool operator==(TYPE x, const expr<R, E>& y) { return sputsoft::numbers::equal(x, y); } \
template <typename N> \
inline bool operator==(const numb<N>& x, TYPE y) { return sputsoft::numbers::equal(x, y); } \
template <typename N> \
inline bool operator==(TYPE x, const numb<N>& y) { return sputsoft::numbers::equal(x, y); }

SPUTSOFT_NUMBERS_EQUAL_INT(unsigned short)
SPUTSOFT_NUMBERS_EQUAL_INT(signed short)
SPUTSOFT_NUMBERS_EQUAL_INT(unsigned)
SPUTSOFT_NUMBERS_EQUAL_INT(signed)
SPUTSOFT_NUMBERS_EQUAL_INT(unsigned long)
SPUTSOFT_NUMBERS_EQUAL_INT(signed long)
#ifdef SPUTSOFT_HAS_LONG_LONG
SPUTSOFT_NUMBERS_EQUAL_INT(unsigned long long)
SPUTSOFT_NUMBERS_EQUAL_INT(signed long long)
#endif

template <typename R1, typename E1, typename R2, typename E2>
inline bool operator!=(const expr<R1, E1>& x, const expr<R2, E2>& y) {
  return !sputsoft::numbers::equal(x, y);
}
template <typename R, typename E, typename N>
inline bool operator!=(const expr<R, E>& x, const numb<N>& y) {
  return !sputsoft::numbers::equal(x, y);
}
template <typename R, typename E, typename N>
inline bool operator!=(const numb<N>& x, const expr<R, E>& y) {
  return !sputsoft::numbers::equal(x, y);
}
template <typename N1, typename N2>
inline bool operator!=(const numb<N1>& x, const numb<N2>& y) {
  return !sputsoft::numbers::equal(x, y);
}

#define SPUTSOFT_NUMBERS_NOT_EQUAL_INT(TYPE) \
template <typename R, typename E> \
inline bool operator!=(const expr<R, E>& x, TYPE y) { return !sputsoft::numbers::equal(x, y); } \
template <typename R, typename E> \
inline bool operator!=(TYPE x, const expr<R, E>& y) { return !sputsoft::numbers::equal(x, y); } \
template <typename N> \
inline bool operator!=(const numb<N>& x, TYPE y) { return !sputsoft::numbers::equal(x, y); } \
template <typename N> \
inline bool operator!=(TYPE x, const numb<N>& y) { return !sputsoft::numbers::equal(x, y); }

SPUTSOFT_NUMBERS_NOT_EQUAL_INT(unsigned short)
SPUTSOFT_NUMBERS_NOT_EQUAL_INT(signed short)
SPUTSOFT_NUMBERS_NOT_EQUAL_INT(unsigned)
SPUTSOFT_NUMBERS_NOT_EQUAL_INT(signed)
SPUTSOFT_NUMBERS_NOT_EQUAL_INT(unsigned long)
SPUTSOFT_NUMBERS_NOT_EQUAL_INT(signed long)
#ifdef SPUTSOFT_HAS_LONG_LONG
SPUTSOFT_NUMBERS_NOT_EQUAL_INT(unsigned long long)
SPUTSOFT_NUMBERS_NOT_EQUAL_INT(signed long long)
#endif

#define SPUTSOFT_NUMBERS_CMP_INT(REL, TYPE) \
template <typename R, typename E> \
inline bool operator REL(const expr<R, E>& x, TYPE y) { return sputsoft::numbers::compare(x, y) REL 0; } \
template <typename R, typename E> \
inline bool operator REL(TYPE x, const expr<R, E>& y) { return sputsoft::numbers::compare(x, y) REL 0; } \
template <typename N> \
inline bool operator REL(const numb<N>& x, TYPE y) { return sputsoft::numbers::compare(x, y) REL 0; } \
template <typename N> \
inline bool operator REL(TYPE x, const numb<N>& y) { return sputsoft::numbers::compare(x, y) REL 0; }

#define SPUTSOFT_NUMBERS_CMP(REL) \
template <typename R1, typename E1, typename R2, typename E2> \
inline bool operator REL(const expr<R1, E1>& x, const expr<R2, E2>& y) { \
  return sputsoft::numbers::compare(x, y) REL 0; \
} \
template <typename R, typename E, typename N> \
inline bool operator REL(const expr<R, E>& x, const numb<N>& y) { \
  return sputsoft::numbers::compare(x, y) REL 0; \
} \
template <typename R, typename E, typename N> \
inline bool operator REL(const numb<N>& x, const expr<R, E>& y) { \
  return sputsoft::numbers::compare(x, y) REL 0; \
} \
template <typename N1, typename N2> \
inline bool operator REL(const numb<N1>& x, const numb<N2>& y) { \
  return sputsoft::numbers::compare(x, y) REL 0; \
} \
SPUTSOFT_NUMBERS_CMP_INT(REL, unsigned short) \
SPUTSOFT_NUMBERS_CMP_INT(REL, signed short) \
SPUTSOFT_NUMBERS_CMP_INT(REL, unsigned) \
SPUTSOFT_NUMBERS_CMP_INT(REL, signed) \
SPUTSOFT_NUMBERS_CMP_INT(REL, unsigned long) \
SPUTSOFT_NUMBERS_CMP_INT(REL, signed long)

SPUTSOFT_NUMBERS_CMP(<)
SPUTSOFT_NUMBERS_CMP(<=)
SPUTSOFT_NUMBERS_CMP(>)
SPUTSOFT_NUMBERS_CMP(>=)

#ifdef SPUTSOFT_HAS_LONG_LONG
SPUTSOFT_NUMBERS_CMP_INT(<, unsigned long long)
SPUTSOFT_NUMBERS_CMP_INT(<, signed long long)
SPUTSOFT_NUMBERS_CMP_INT(<=, unsigned long long)
SPUTSOFT_NUMBERS_CMP_INT(<=, signed long long)
SPUTSOFT_NUMBERS_CMP_INT(>, unsigned long long)
SPUTSOFT_NUMBERS_CMP_INT(>, signed long long)
SPUTSOFT_NUMBERS_CMP_INT(>=, unsigned long long)
SPUTSOFT_NUMBERS_CMP_INT(>=, signed long long)
#endif


template <typename R, typename E>
std::ostream& operator<<(std::ostream& os, const expr<R, E>& e) {
  return os << (R) e;
}

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_OVERLOAD_HPP
