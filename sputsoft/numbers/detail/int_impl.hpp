/* 
 * File:   int_impl.hpp
 * Author: jmr
 *
 * Created on May 4, 2010, 10:12 PM
 */

#ifndef _INT_IMPL_HPP
#define	_INT_IMPL_HPP

#include <sputsoft/detail/types.hpp>
#include <sputsoft/numbers/detail/expr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T>
class intnum;

template <typename NUM>
class expr<intnum<NUM> > {
  template <typename NUM2>
  friend std::ostream& operator<<(std::ostream& os, const expr<intnum<NUM2> >& n);
private:
  NUM num;
  bool positive;

  template <typename T>
  void set_int(T v) {
    positive = v >= 0;
    sputsoft::numbers::set(num, sputsoft::to_unsigned(positive ? v : -v));
  }

  template <typename T>
  void _add(const NUM& n1, bool p1, const T& n2, bool p2) {
    if (p1 == p2) {
      sputsoft::numbers::add(num, n1, n2);
      positive = p1;
    } else if (sputsoft::numbers::compare(n1, n2) >= 0) {
      sputsoft::numbers::sub(num, n1, n2);
      positive = p1;
    } else {
      sputsoft::numbers::sub(num, n2, n1);
      positive = p2;
    }
  }

  template <typename T>
  inline void add_signed_int(const NUM& n1, bool p1, T v2) {
    bool p2 = v2 >= 0;
    _add(n1, p1, sputsoft::to_unsigned(p2 ? v2 : -v2), p2);
  }

  template <typename T>
  inline void _mul(const NUM& n1, bool p1, const T& n2, bool p2) {
    sputsoft::numbers::mul(num, n1, n2);
    positive = p1 == p2;
  }

  template <typename T>
  inline void mul_signed_int(const NUM& n1, bool p1, T v2) {
    bool p2 = v2 >= 0;
    _mul(n1, p1, sputsoft::to_unsigned(p2 ? v2 : -v2), p2);
  }

  /* quotrem, div, rem floor */

  static void quotrem_floor(expr& q, expr& r, const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::quotrem(q.num, r.num, v1, v2);
    if (p1 != p2 && r.num) {
      sputsoft::numbers::add(q.num, q.num, 1u);
      sputsoft::numbers::sub(r.num, v2, r.num);
    }
    q.positive = p1 == p2;
    r.positive = p2;
  }

  template <typename T>
  T quotrem_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::quotrem(num, v1, v2u);
    positive = p1 == p2;
    if (!positive && ru) {
      sputsoft::numbers::add(num, num, 1u);
      ru = v2u - ru;
    }
    return (T) (p2 ? ru : -ru);
  }

  template <typename T>
  void div_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type r = sputsoft::numbers::quotrem(num, v1, (unsigned_type) (p2 ? v2 : -v2));
    positive = p1 == p2;
    if (!positive && r)
      sputsoft::numbers::add(num, num, 1u);
  }

  template <typename T>
  static T rem_floor_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::rem(v1, v2u);
    if (p1 != p2 && ru) ru = v2u - ru;
    return (T) (p2 ? ru : -ru);
  }

  /* quotrem, div, rem ceil */

  static void quotrem_ceil(expr& q, expr& r, const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::quotrem(q.num, r.num, v1, v2);
    if (p1 == p2 && r.num) {
      sputsoft::numbers::add(q.num, q.num, 1u);
      sputsoft::numbers::sub(r.num, v2, r.num);
    }
    q.positive = p1 == p2;
    r.positive = !p2;
  }

  template <typename T>
  T quotrem_ceil_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::quotrem(num, v1, v2u);
    positive = p1 == p2;
    if (positive && ru) {
      sputsoft::numbers::add(num, num, 1u);
      ru = v2u - ru;
    }
    return (T) (p2 ? -ru : ru);
  }

  template <typename T>
  void div_ceil_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type r = sputsoft::numbers::quotrem(num, v1, (unsigned_type) (p2 ? v2 : -v2));
    positive = p1 == p2;
    if (positive && r)
      sputsoft::numbers::add(num, num, 1u);
  }

  template <typename T>
  static T rem_ceil_int(const NUM& v1, bool p1, T v2) {
    typedef typename sputsoft::number_traits<T>::unsigned_type unsigned_type;
    bool p2 = v2 >= 0;
    unsigned_type v2u = p2 ? v2 : -v2;
    unsigned_type ru = sputsoft::numbers::rem(v1, v2u);
    if (p1 == p2 && ru)
      ru = v2u - ru;
    return (T) (p2 ? -ru : ru);
  }

  /* quotrem, div, rem trunc */

  static void quotrem_trunc(expr& q, expr& r, const NUM& v1, bool p1, const NUM& v2, bool p2) {
    sputsoft::numbers::quotrem(q.num, r.num, v1, v2);
    q.positive = p1 == p2;
    r.positive = p1;
  }

  template <typename T>
  void div_trunc_int(const NUM& v1, bool p1, T v2) {
    sputsoft::numbers::div(num, v1, v2);
    positive = p1 == (v2 >= 0);
  }

  /* comparisons */

  template <typename T>
  static int _cmp(const NUM& v1, bool p1, const T& v2, bool p2) {
    if (!v1)
      return !v2 ? 0 : p2 ? -1 : 1;
    if (!v2 || p1 != p2)
      return p1 ? 1 : -1;
    int c = sputsoft::numbers::compare(v1, v2);
    return p1 ? c : -c;
  }

  template <typename T>
  static int cmp_int(const NUM& v1, bool p1, const T& v2) {
    bool p2 = v2 >= 0;
    return _cmp(v1, p1, sputsoft::to_unsigned(p2 ? v2 : -v2), p2);
  }

public:
  expr() : num(), positive(true) {}
  expr(const expr& rhs) : num(rhs.num), positive(rhs.positive) {}
  expr(const NUM& n, bool pos=true) : num(n), positive(pos) {}
  expr& operator=(const expr& rhs) { set(rhs); return *this; }
  inline void set(const expr& v) { num = v.num; positive = v.positive; }
  inline void set(const NUM& n) { num = n; positive = true; }
  operator bool() const { return num; }

  expr(unsigned short v) : num(v), positive(true) {}
  inline void add(const expr& v1, unsigned short v2) { _add(v1.num, v1.positive, v2, true); }
  inline void add(unsigned short v1, const expr& v2) { _add(v2.num, v2.positive, v1, true); }
  inline void sub(const expr& v1, unsigned short v2) { _add(v1.num, v1.positive, v2, false); }
  inline void sub(unsigned short v1, const expr& v2) { _add(v2.num, v2.positive, v1, false); }
  inline void mul(const expr& v1, unsigned short v2) { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(unsigned short v1, const expr& v2) { _mul(v2.num, v2.positive, v1, true); }
  inline void div(const expr& v1, unsigned short v2) { div_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const expr& v1, unsigned short v2) { div_ceil_int(v1.num, v1.positive, v2); }
  inline void div_trunc(const expr& v1, unsigned short v2) { div_trunc_int(v1.num, v1.positive, v2); }

  expr(short v) : num(sputsoft::to_unsigned(v >= 0 ? v : -v)), positive(v >= 0) {}
  inline void add(const expr& v1, short v2) { add_signed_int(v1.num, v1.positive, v2); }
  inline void add(short v1, const expr& v2) { add_signed_int(v2.num, v2.positive, v1); }
  inline void sub(const expr& v1, short v2) { add_signed_int(v1.num, v1.positive, -v2); }
  inline void sub(short v1, const expr& v2) { add_signed_int(v2.num, !v2.positive, v1); }
  inline void mul(const expr& v1, short v2) { mul_signed_int(v1.num, v1.positive, v2); }
  inline void mul(short v1, const expr& v2) { mul_signed_int(v2.num, v2.positive, v1); }

  expr(unsigned v) : num(v), positive(true) {}
  inline void add(const expr& v1, unsigned v2) { _add(v1.num, v1.positive, v2, true); }
  inline void add(unsigned v1, const expr& v2) { _add(v2.num, v2.positive, v1, true); }
  inline void sub(const expr& v1, unsigned v2) { _add(v1.num, v1.positive, v2, false); }
  inline void sub(unsigned v1, const expr& v2) { _add(v2.num, v2.positive, v1, false); }
  inline void mul(const expr& v1, unsigned v2) { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(unsigned v1, const expr& v2) { _mul(v2.num, v2.positive, v1, true); }

  expr(int v) : num(sputsoft::to_unsigned(v >= 0 ? v : -v)), positive(v >= 0) {}
  inline void set(int v) { set_int(v); }
  inline void add(const expr& v1, int v2) { add_signed_int(v1.num, v1.positive, v2); }
  inline void add(int v1, const expr& v2) { add_signed_int(v2.num, v2.positive, v1); }
  inline void sub(const expr& v1, int v2) { add_signed_int(v1.num, v1.positive, -v2); }
  inline void sub(int v1, const expr& v2) { add_signed_int(v2.num, !v2.positive, v1); }
  inline void mul(const expr& v1, int v2) { mul_signed_int(v1.num, v1.positive, v2); }
  inline void mul(int v1, const expr& v2) { mul_signed_int(v2.num, v2.positive, v1); }
  inline void div(const expr& v1, int v2) { div_floor_int(v1.num, v1.positive, v2); }
  static inline int rem(const expr& v1, int v2) { return rem_floor_int(v1.num, v1.positive, v2); }
  inline int quotrem(const expr& v1, int v2) { return quotrem_floor_int(v1.num, v1.positive, v2); }
  inline void div_ceil(const expr& v1, int v2) { div_ceil_int(v1.num, v1.positive, v2); }
  static inline int rem_ceil(const expr& v1, int v2) { return rem_ceil_int(v1.num, v1.positive, v2); }
  inline int quotrem_ceil(const expr& v1, int v2) { return quotrem_ceil_int(v1.num, v1.positive, v2); }

  inline void add(const expr& v1, const expr& v2) { _add(v1.num, v1.positive, v2.num, v2.positive); }
  inline void add(const expr& v1, const NUM& v2)  { _add(v1.num, v1.positive, v2, true); }
  inline void add(const NUM& v1,  const expr& v2) { _add(v1, true, v2.num, v2.positive); }
  inline void add(const NUM& v1,  const NUM& v2)  { _add(v1, true, v2, true); }
  inline void sub(const expr& v1, const expr& v2) { _add(v1.num, v1.positive, v2.num, !v2.positive); }
  inline void sub(const expr& v1, const NUM& v2)  { _add(v1.num, v1.positive, v2, false); }
  inline void sub(const NUM& v1,  const expr& v2) { _add(v1, true, v2.num, !v2.positive); }
  inline void sub(const NUM& v1,  const NUM& v2)  { _add(v1, true, v2, false); }
  inline void mul(const expr& v1, const expr& v2) { _mul(v1.num, v1.positive, v2.num, v2.positive); }
  inline void mul(const expr& v1, const NUM& v2)  { _mul(v1.num, v1.positive, v2, true); }
  inline void mul(const NUM& v1,  const expr& v2) { _mul(v1, true, v2.num, v2.positive); }
  inline void mul(const NUM& v1,  const NUM& v2)  { _mul(v1, true, v2, true); }
  //static inline void quotrem_floor(expr& q, expr& r, const expr& u, const expr& v) { }
  inline int cmp(const expr& v) const { return _cmp(num, positive, v.num, v.positive); }

};

template <typename NUM>
inline std::ostream& operator<<(std::ostream& os, const expr<intnum<NUM> >& n) {
  if (n.num && !n.positive) os << "-";
  return os << n.num;
}

template <typename NUM, typename Forw>
struct set_4_eval<expr<intnum<NUM> >, Forw> {
  static void set(expr<intnum<NUM> >& n, Forw first, const Forw last, unsigned base) {
    bool pos=true;
    while (first != last) {
      char c = *first;
      if (c == '-') pos = !pos;
      else if (c != '+') break;
      ++first;
    }
    NUM t;
    sputsoft::numbers::set(t, first, last, base);
    n = expr<intnum<NUM> >(t, pos);
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _INT_IMPL_HPP */

