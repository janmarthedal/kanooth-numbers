#ifndef SHARED_HPP
#define SHARED_HPP

#include <math/numbers/number.hpp>
#include <boost/shared_ptr.hpp>

namespace sputsoft {
namespace math {
namespace numbers {

template <typename T>
class shared {
private:
  boost::shared_ptr<T> v;

  template <typename X> X strip(const X& x) { return x; }
  template <typename X> X strip(const shared<X>& x) { return x.value(); }

  template <typename X, typename Y, typename Op>
  binary_expr<X, Y, Op> strip(const binary_expr<shared<X>, shared<Y>, Op>& expr)
  { return binary_expr<X, Y, Op>(expr.x.value(), expr.y.value(), expr.op); }
  
  template <typename X, typename Y, typename Op>
  binary_expr<X, Y, Op> strip(const binary_expr<shared<X>, Y, Op>& expr)
  { return binary_expr<X, Y, Op>(expr.x.value(), expr.y, expr.op); }
  
  template <typename X, typename Y, typename Op>
  binary_expr<X, Y, Op> strip(const binary_expr<X, shared<Y>, Op>& expr)
  { return binary_expr<X, Y, Op>(expr.x, expr.y.value(), expr.op); }
  
public:
  template <typename P> shared(P* p) : v(p) {}
  template <typename S> shared(const S& s) : v(new T(strip(s))) {}
  bool unique() const { return v.unique(); }
  T& value() { return *v; }
  const T& value() const { return *v; }
  void swap(shared& other) { v.swap(other.v); }

  template <typename Expr>
  void assign(shared<T>& r, const Expr& expr)
  {
    if (r.unique())
      T::eval(r.value(), strip(expr));
    else
      r = shared<T>(strip(expr));
  }

};

template <typename Os, typename T>
Os& operator<<(Os& os, const shared<T>& n)
{
  return os << n.value();
}

} // namespace numbers
} // namespace math
} // namespace sputsoft

#endif // SHARED_HPP

