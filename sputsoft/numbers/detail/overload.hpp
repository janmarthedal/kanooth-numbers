/* 
 * File:   overload.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:21 PM
 */

#ifndef _OVERLOAD_HPP
#define	_OVERLOAD_HPP

namespace sputsoft {
namespace numbers {
namespace detail {

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

} // namespace sputsoft
} // namespace detail
} // namespace numbers

#endif	/* _OVERLOAD_HPP */
