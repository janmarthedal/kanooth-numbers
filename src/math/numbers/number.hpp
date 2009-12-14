#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <iostream>

/*namespace sputsoft {
namespace math {
namespace numbers {*/

namespace ops {
  namespace binary {
    class add {};
  }
  namespace unary {
    class identity {};
  }
}

namespace expr {

template <typename Op, typename X, typename Y>
class binary {
private:
  binary() {}
  template <typename T> binary(const T&) {}
public:
  const X& x;
  const Y& y;
  binary(const X& _x, const Y& _y) : x(_x), y(_y) {}
};

}

template <typename T>
class number {
private:
  T n;
public:
  number() : n() {}
  template <typename S> number(const S& s) : n(s) {}
  template <typename Expr>
    number& operator=(const Expr& expr) { T::assign(n, expr); return *this; }
  T& get() { return n; }
  const T& get() const { return n; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const number<T>& n)
{
  return os << n.get();
}

template <typename Xop, typename X1, typename X2, typename Yop, typename Y1, typename Y2>
expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, expr::binary<Yop, Y1, Y2> >
operator+(const expr::binary<Xop, X1, X2>& x, const expr::binary<Yop, Y1, Y2>& y)
{
  return expr::binary<ops::binary::add, expr::binary<Xop, X1, X2>, expr::binary<Yop, Y1, Y2> >(x, y);
}

template <typename X, typename Yop, typename Y1, typename Y2>
expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >
operator+(const number<X>& x, const expr::binary<Yop, Y1, Y2>& y)
{
  return expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >(x.get(), y);
}

template <typename X, typename Yop, typename Y1, typename Y2>
expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >
operator+(const X& x, const expr::binary<Yop, Y1, Y2>& y)
{
  return expr::binary<ops::binary::add, X, expr::binary<Yop, Y1, Y2> >(x.get(), y);
}

template <typename X, typename Y>
expr::binary<ops::binary::add, X, Y>
operator+(const number<X>& x, const number<Y>& y)
{
  return expr::binary<ops::binary::add, X, Y>(x.get(), y.get());
}

template <typename X, typename Y>
expr::binary<ops::binary::add, X, Y>
operator+(const number<X>& x, const Y& y)
{
  return expr::binary<ops::binary::add, X, Y>(x.get(), y);
}

template <typename X, typename Y>
expr::binary<ops::binary::add, X, Y>
operator+(const X& x, const number<Y>& y)
{
  return expr::binary<ops::binary::add, X, Y>(x, y.get());
}

/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // NUMBER_HPP

