/* 
 * File:   overload.hpp
 * Author: jmr
 *
 * Created 2010-04-30 15:21Z
 */

#include <sputsoft/numbers/detail/named_ops.hpp>

#ifndef _OVERLOAD_HPP
#define	_OVERLOAD_HPP

namespace sputsoft {
namespace numbers {
namespace detail {

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

template <typename Op, typename R, typename X, typename Y> struct expr_eval;

template <typename R, typename X, typename Y>
struct expr_eval<ops::binary::, R, X, Y> {
  inline static void assign(R& r, const X& x, const Y& y) { ::add(r, x, y); }
  inline static R construct(const X& x, const Y& y) { return ::add(x, y); }
};

template <typename R, typename X, typename Y>
struct expr_eval<ops::binary::remainder, R, X, Y> {
  inline static void assign(R& r, const X& x, const Y& y) { ::rem(r, x, y); }
  inline static R construct(const X& x, const Y& y) { return ::rem(x, y); }
};

template <class T>
struct resolve_ref {
  typedef T ref_type;
};

template <class T>
struct resolve_ref<numb<T> > {
  typedef const numb<T>& ref_type;
};

template <typename Op, typename X, typename Y> struct binary;

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
struct set_eval<numb<R1>, expr<R2, E2> > {
  static void set(numb<R1>& r, const expr<R2, E2>& v) {
    v.assign(r);
  }
};

template <typename R1, typename E1, typename R2, typename E2>
expr<typename resolve_binary<ops::binary::plus, R1, R2>::return_type,
     binary<ops::binary::plus, expr<R1, E1>, expr<R2, E2> > >
operator+(const expr<R1, E1>& x, const expr<R2, E2>& y) {
  return expr<typename resolve_binary<ops::binary::plus, R1, R2>::return_type,
     binary<ops::binary::plus, expr<R1, E1>, expr<R2, E2> > >(x, y);
}

template <typename N1, typename R2, typename E2>
expr<typename resolve_binary<ops::binary::plus, numb<N1>, R2>::return_type,
     binary<ops::binary::plus, numb<N1>, expr<R2, E2> > >
operator+(const numb<N1>& x, const expr<R2, E2>& y) {
  return expr<typename resolve_binary<ops::binary::plus, numb<N1>, R2>::return_type,
     binary<ops::binary::plus, numb<N1>, expr<R2, E2> > >(x, y);
}

template <typename R1, typename E1, typename N2>
expr<typename resolve_binary<ops::binary::plus, R1, numb<N2> >::return_type,
     binary<ops::binary::plus, expr<R1, E1>, numb<N2> > >
operator+(const expr<R1, E1>& x, const numb<N2>& y) {
  return expr<typename resolve_binary<ops::binary::plus, R1, numb<N2> >::return_type,
     binary<ops::binary::plus, expr<R1, E1>, numb<N2> > >(x, y);
}

template <typename N1, typename N2>
expr<typename resolve_binary<ops::binary::plus, numb<N1>, numb<N2> >::return_type,
     binary<ops::binary::plus, numb<N1>, numb<N2> > >
operator+(const numb<N1>& x, const numb<N2>& y) {
  return expr<typename resolve_binary<ops::binary::plus, numb<N1>, numb<N2> >::return_type,
     binary<ops::binary::plus, numb<N1>, numb<N2> > >(x, y);
}

template <typename R, typename E>
expr<typename resolve_binary<ops::binary::plus, unsigned, R>::return_type,
     binary<ops::binary::plus, unsigned, expr<R, E> > >
operator+(const unsigned& x, const expr<R, E>& y) {
  return expr<typename resolve_binary<ops::binary::plus, unsigned, R>::return_type,
     binary<ops::binary::plus, unsigned, expr<R, E> > >(x, y);
}

template <typename R, typename E>
expr<typename resolve_binary<ops::binary::plus, R, unsigned>::return_type,
     binary<ops::binary::plus, expr<R, E>, unsigned> >
operator+(const expr<R, E>& x, const unsigned& y) {
  return expr<typename resolve_binary<ops::binary::plus, R, unsigned>::return_type,
     binary<ops::binary::plus, expr<R, E>, unsigned> >(x, y);
}

template <typename N>
expr<typename resolve_binary<ops::binary::plus, numb<N>, unsigned>::return_type,
     binary<ops::binary::plus, numb<N>, unsigned> >
operator+(const numb<N>& x, unsigned y) {
  return expr<typename resolve_binary<ops::binary::plus, numb<N>, unsigned>::return_type,
     binary<ops::binary::plus, numb<N>, unsigned> >(x, y);
}

template <typename N>
expr<typename resolve_binary<ops::binary::plus, unsigned, numb<N> >::return_type,
     binary<ops::binary::plus, unsigned, numb<N> > >
operator+(unsigned x, const numb<N>& y) {
  return expr<typename resolve_binary<ops::binary::plus, unsigned, numb<N> >::return_type,
     binary<ops::binary::plus, unsigned, numb<N> > >(x, y);
}

template <typename N>
expr<typename resolve_binary<ops::binary::remainder, numb<N>, unsigned>::return_type,
     binary<ops::binary::remainder, numb<N>, unsigned> >
operator%(const numb<N>& x, unsigned y) {
  return expr<typename resolve_binary<ops::binary::remainder, numb<N>, unsigned>::return_type,
     binary<ops::binary::remainder, numb<N>, unsigned> >(x, y);
}

template <typename N, typename T>
numb<N>& operator+=(numb<N>& x, const T& y) {
  return x = x + y;
}


} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif	/* _OVERLOAD_HPP */
