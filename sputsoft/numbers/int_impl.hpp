/* 
 * File:   int_impl.hpp
 * Author: jmr
 *
 * Created on May 4, 2010, 10:12 PM
 */

#ifndef _INT_IMPL_HPP
#define	_INT_IMPL_HPP

#include <sputsoft/numbers/detail/expr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T>
class intnum;

template <typename NUM>
class expr<intnum<NUM> > {
private:
  NUM num;
  bool positive;

  void add_num(const NUM& n1, bool p1, const NUM& n2, bool p2) {
    
  }

public:
  expr() : num(), positive(true) {}
  expr(const expr& rhs) : num(rhs.num), positive(rhs.positive) {}
  expr(const NUM& n) : num(n), positive(true) {}
  expr& operator=(const expr& rhs) {
    num = rhs.num;
    positive = rhs.positive;
    return *this;
  }
  operator bool() const {
    return num;
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _INT_IMPL_HPP */

