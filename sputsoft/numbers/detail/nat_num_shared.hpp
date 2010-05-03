/* 
 * File:   nat_num_wrap.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:18 PM
 *
 * $Id$
 */

#ifndef _SHARED_NAT_NUM_WRAP_HPP
#define	_SHARED_NAT_NUM_WRAP_HPP

#include <sputsoft/numbers/detail/nat_num_impl.hpp>
#include <boost/shared_ptr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Con, typename LowLevel>
class expr<natnum<boost::shared_ptr<wrap2<Con, LowLevel> > > > {
private:
  typedef expr<natnum<wrap2<Con, LowLevel> > > number_type;
  boost::shared_ptr<number_type> num;

  template <typename T>
  void _set(T v) {
    if (!num.unique()) num.reset(new number_type);
    num->set(v);
  }

  template <typename V2>
  void _add(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->add(v1, v2);
  }

  template <typename V2>
  void _sub(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->sub(v1, v2);
  }

  template <typename V2>
  void _mul(const number_type& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    num->mul(v1, v2);
  }

  template <typename V2>
  V2 _quotrem(const number_type& v1, const V2 v2) {
    if (!num.unique()) num.reset(new number_type);
    return num->quotrem(v1, v2);
  }

public:
  expr() : num(new number_type) {}
  expr(const expr& rhs) : num(rhs.num) {}
  expr& operator=(const expr& rhs) {
    num = rhs.num;
    return *this;
  }
  operator bool() const {
    return *num;
  }
  std::size_t floor_log2() const {
    return num->floor_log2();
  }

  expr(unsigned short v) : num(new number_type(v)) {}
  inline void set(unsigned short v) { _set(v); }
  inline void add(const expr& x, unsigned short y) { _add(*x.num, y); }
  inline void add(unsigned short x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned short y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned short y) { _mul(*x.num, y); }
  inline unsigned short quotrem(const expr& x, unsigned short y)
    { return _quotrem(*x.num, y); }

  expr(unsigned v) : num(new number_type(v)) {}
  inline void set(unsigned v) { _set(v); }
  inline void add(const expr& x, unsigned y) { _add(*x.num, y); }
  inline void add(unsigned x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned y) { _mul(*x.num, y); }
  inline unsigned quotrem(const expr& x, unsigned y)
    { return _quotrem(*x.num, y); }

  expr(unsigned long v) : num(new number_type(v)) {}
  inline void set(unsigned long v) { _set(v); }
  inline void add(const expr& x, unsigned long y) { _add(*x.num, y); }
  inline void add(unsigned long x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned long y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned long y) { _mul(*x.num, y); }
  inline unsigned long quotrem(const expr& x, unsigned long y)
    { return _quotrem(*x.num, y); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  expr(unsigned long long v) : num(new number_type(v)) {}
  inline void set(unsigned long long v) { _set(v); }
  inline void add(const expr& x, unsigned long long y) { _add(*x.num, y); }
  inline void add(unsigned long long x, const expr& y) { _add(*y.num, x); }
  inline void sub(const expr& x, unsigned long long y) { _sub(*x.num, y); }
  inline void mul(const expr& x, unsigned long long y) { _mul(*x.num, y); }
  inline unsigned long long quotrem(const expr& x, unsigned long long y)
    { return _quotrem(*x.num, y); }
#endif

  inline void set(const expr& x) { num = x.num; }
  inline void add(const expr& x, const expr& y) { _add(*x.num, *y.num); }
  inline void sub(const expr& x, const expr& y) { _sub(*x.num, *y.num); }
  inline void mul(const expr& x, const expr& y) { _mul(*x.num, *y.num); }
  static inline void quotrem(expr& q, expr& r, const expr& u, const expr& v) {
    if (!q.num.unique()) q.num.reset(new number_type);
    if (!r.num.unique()) r.num.reset(new number_type);
    number_type::quotrem(*q.num, *r.num, *u.num, *v.num);
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _NAT_NUM_WRAP_HPP */

