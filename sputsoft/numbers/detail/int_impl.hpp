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
    typedef typename sputsoft::type_convert<T>::unsigned_type unsigned_type;
    if (v2 >= 0)
      _add(n1, p1, (unsigned_type) v2, true);
    else
      _add(n1, p1, (unsigned_type) -v2, false);
  }

public:
  expr() : num(), positive(true) {}
  expr(const expr& rhs) : num(rhs.num), positive(rhs.positive) {}
  expr(const NUM& n, bool pos=true) : num(n), positive(pos) {}
  expr& operator=(const expr& rhs) {
    num = rhs.num;
    positive = rhs.positive;
    return *this;
  }
  operator bool() const {
    return num;
  }

  inline void add(const expr& v1, const expr& v2) { _add(v1.num, v1.positive, v2.num, v2.positive); }
  inline void add(const expr& v1, const NUM& v2)  { _add(v1.num, v1.positive, v2, true); }
  inline void add(const NUM& v1,  const expr& v2) { _add(v1, true, v2.num, v2.positive); }
  inline void add(const NUM& v1,  const NUM& v2)  { _add(v1, true, v2, true); }
  inline void sub(const expr& v1, const expr& v2) { _add(v1.num, v1.positive, v2.num, !v2.positive); }
  inline void sub(const expr& v1, const NUM& v2)  { _add(v1.num, v1.positive, v2, false); }
  inline void sub(const NUM& v1,  const expr& v2) { _add(v1, true, v2.num, !v2.positive); }
  inline void sub(const NUM& v1,  const NUM& v2)  { _add(v1, true, v2, false); }

  inline void add(const expr& v1, unsigned short v2) { _add(v1.num, v1.positive, v2, true); }
  inline void add(unsigned short v1, const expr& v2) { _add(v2.num, v2.positive, v1, true); }
  inline void sub(const expr& v1, unsigned short v2) { _add(v1.num, v1.positive, v2, false); }
  inline void sub(unsigned short v1, const expr& v2) { _add(v2.num, v2.positive, v1, false); }

  inline void add(const expr& v1, short v2) { add_signed_int(v1.num, v1.positive, v2); }
  inline void add(short v1, const expr& v2) { add_signed_int(v2.num, v2.positive, v1); }
  inline void sub(const expr& v1, short v2) { add_signed_int(v1.num, v1.positive, -v2); }
  inline void sub(short v1, const expr& v2) { add_signed_int(v2.num, !v2.positive, v1); }

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

