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

#include <sputsoft/type_traits.hpp>
#include <sputsoft/numbers/detail/named_ops.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

// Local stuff
namespace {

template <typename R, typename E> class expr;
template <typename Op, typename X, typename Y> struct binary;
template <typename Op, typename X> struct unary;

template <class T>
struct resolve_ref {
  typedef T ref_type;
};

template <class T>
struct resolve_ref<numb<T> > {
  typedef const numb<T>& ref_type;
};

template <typename T>
struct resolve_type {
  typedef T type;
};

template <typename R, typename E>
struct resolve_type<expr<R, E> > {
  typedef R type;
};

template <typename T> struct is_numb_or_expr : public sputsoft::false_type {};
template <typename N> struct is_numb_or_expr<numb<N> > : public sputsoft::true_type {};
template <typename R, typename E> struct is_numb_or_expr<expr<R, E> >
  : public sputsoft::true_type {};

template <typename Op, typename T1, typename T2, bool Ok>
struct approve_binop_overload2 {};

template <typename Op, typename T1, typename T2>
struct approve_binop_overload2<Op, T1, T2, true> {
  typedef expr<typename resolve_binary<Op, typename resolve_type<T1>::type,
                                       typename resolve_type<T2>::type>::return_type,
               binary<Op, T1, T2> > return_type;
  return_type operator()(const T1& x, const T2& y) {
    return return_type(x, y);
  }
};

template <typename Op, typename T1, typename T2>
struct approve_binop_overload
  : approve_binop_overload2<Op, T1, T2,
        (is_numb_or_expr<T1>::value &&
          (is_numb_or_expr<T2>::value || is_native_int<T2>::value)) ||
        (is_numb_or_expr<T2>::value &&
          (is_numb_or_expr<T1>::value || is_native_int<T1>::value))> {};

template <typename Op, typename T1, typename T2>
struct approve_shift_overload
  : approve_binop_overload2<Op, T1, T2, is_numb_or_expr<T1>::value && is_native_int<T2>::value> {};

template <typename Op, typename T, bool Ok>
struct approve_unop_overload2 {};

template <typename Op, typename T>
struct approve_unop_overload2<Op, T, true> {
  typedef expr<typename resolve_unary<Op, typename resolve_type<T>::type >::return_type,
               unary<Op, T> > return_type;
  return_type operator()(const T& x) {
    return return_type(x);
  }
};

template <typename Op, typename T>
struct approve_unop_overload
  : approve_unop_overload2<Op, T, is_numb_or_expr<T>::value> {};

/*
template <typename R, typename R2, typename X, typename Y>
struct expr<R, binary<ops::binary::add, X, expr<R2, unary<ops::unary::negate, Y> > > > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
  expr(const X& _x, const expr<R2, unary<ops::unary::negate, Y> >& _y) : x(_x), y(_y.x) {}
  inline void assign(R& r) const { sputsoft::numbers::sub(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::sub((x_type) x, (y_type) y); }
};
*/

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
  inline void assign(R& r) const { sputsoft::numbers::bit_shift_left(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::bit_shift_left((x_type) x, (y_type) y); }
};

template <typename R, typename X, typename Y>
class expr<R, binary<ops::binary::rshift, X, Y> > {
  typename resolve_ref<X>::ref_type x;
  typename resolve_ref<Y>::ref_type y;
  typedef typename resolve_type<X>::type x_type;
  typedef typename resolve_type<Y>::type y_type;
public:
  expr(const X& _x, const Y& _y) : x(_x), y(_y) {}
  inline void assign(R& r) const { sputsoft::numbers::bit_shift_right(r, (x_type) x, (y_type) y); }
  inline operator R() const { return sputsoft::numbers::bit_shift_right((x_type) x, (y_type) y); }
};

template <typename R, typename X>
struct expr<R, unary<ops::unary::negate, X> > {
  typename resolve_ref<X>::ref_type x;
  typedef typename resolve_type<X>::type x_type;
  expr(const X& _x) : x(_x) {}
  inline void assign(R& r) const { sputsoft::numbers::negate(r, (x_type) x); }
  inline operator R() const { return sputsoft::numbers::negate((x_type) x); }
};

} // local namespace

template <typename R1, typename R2, typename E2>
struct evaluator_rv<ops::unary::identity, numb<R1>, expr<R2, E2> > {
  void operator()(numb<R1>& r, const expr<R2, E2>& v) const {
    v.assign(r);
  }
};

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::add, T1, T2>::return_type
operator+(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::add, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::sub, T1, T2>::return_type
operator-(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::sub, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::mul, T1, T2>::return_type
operator*(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::mul, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::div, T1, T2>::return_type
operator/(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::div, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_binop_overload<ops::binary::rem, T1, T2>::return_type
operator%(const T1& x, const T2& y) {
  return approve_binop_overload<ops::binary::rem, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_shift_overload<ops::binary::lshift, T1, T2>::return_type
operator<<(const T1& x, const T2& y) {
  return approve_shift_overload<ops::binary::lshift, T1, T2>()(x, y);
}

template <typename T1, typename T2>
typename approve_shift_overload<ops::binary::rshift, T1, T2>::return_type
operator>>(const T1& x, const T2& y) {
  return approve_shift_overload<ops::binary::rshift, T1, T2>()(x, y);
}

template <typename T>
typename approve_unop_overload<ops::unary::negate, T>::return_type
operator-(const T& x) {
  return approve_unop_overload<ops::unary::negate, T>()(x);
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
