#ifndef NUMBER_HPP
#define NUMBER_HPP

namespace sputsoft {
namespace math {
namespace numbers {

namespace ops {
  class binary_addition {};
}

template <typename X, typename Y, typename Op>
class binary_expr {
private:
  template <typename S, typename T, typename Op2>
  binary_expr(const binary_expr<S, T, Op2>&) {}
public:
  const X& x;
  const Y& y;
  const Op& op;
  binary_expr(const X& _x, const Y& _y, const Op& _op) : x(_x), y(_y), op(_op) {}
};

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

template <typename Os, typename T>
Os& operator<<(Os& os, const number<T>& n)
{
  return os << n.get();
}

template <typename X, typename Y>
binary_expr<X, Y, ops::binary_addition>
operator+(const number<X>& x, const number<Y>& y)
{
  return binary_expr<X, Y, ops::binary_addition>(x.get(), y.get(), ops::binary_addition());
}

} // namespace numbers
} // namespace math
} // namespace sputsoft

#endif // NUMBER_HPP

