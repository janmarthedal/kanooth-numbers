#include <sstream>

template <class T>
inline std::string to_string(const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

//namespace sputsoft { namespace math { namespace numbers {

#define STRING_CONSTRUCT_BINARY(op_type, op_char) \
  template <typename Xop, typename X1, typename X2, typename Yop, typename Y1, typename Y2> \
  string(const expr::binary<op_type, expr::binary<Xop, X1, X2>, expr::binary<Yop, Y1, Y2> >& expr) \
    : st("new(" + string(expr.x).st + op_char + string(expr.y).st + ")") {} \
  template <typename Op, typename X, typename Y> \
  string(const expr::binary<op_type, expr::binary<Op, X, Y>, string>& expr) \
    : st("new(" + string(expr.x).st + op_char + expr.y.st + ")") {} \
  template <typename Op, typename X, typename Y> \
  string(const expr::binary<op_type, string, expr::binary<Op, X, Y> >& expr) \
    : st("new(" + expr.x.st + op_char + string(expr.y).st + ")") {} \
  template <typename Op, typename X, typename Y, typename T> \
  string(const expr::binary<op_type, expr::binary<Op, X, Y>, T>& expr) \
    : st("new(" + string(expr.x).st + op_char + to_string(expr.y) + ")") {} \
  template <typename Op, typename X, typename Y, typename T> \
  string(const expr::binary<op_type, T, expr::binary<Op, X, Y> >& expr) \
    : st("new(" + to_string(expr.x) + op_char + string(expr.y).st + ")") {} \
  string(const expr::binary<op_type, string, string>& expr) \
    : st("new(" + expr.x.st + op_char + expr.y.st + ")") {} \
  template <typename T> \
  string(const expr::binary<op_type, string, T>& expr) \
    : st("new(" + expr.x.st + op_char + to_string(expr.y) + ")") {} \
  template <typename T> \
  string(const expr::binary<op_type, T, string>& expr) \
    : st("new(" + to_string(expr.x) + op_char + expr.y.st + ")") {} \
  template <typename S, typename T> \
  string(const expr::binary<op_type, S, T>& expr) \
    : st("new(" + to_string(expr.x) + op_char + to_string(expr.y) + ")") {}

#define STRING_CONSTRUCT_UNARY(op_type, op_char) \
  template <typename Xop, typename X1, typename X2> \
  string(const expr::unary<op_type, expr::binary<Xop, X1, X2> >& expr) \
    : st(to_string("new(") + op_char + string(expr.get()).st + ")") {}

class string {
private:
  std::string st;
public:
  string() : st("<>") {}
  string(const std::string _st) : st("<" + _st + ">") {}
  string(const char* ch) : st("<" + std::string(ch) + ">") {}
  const std::string& get() const { return st; }

  STRING_CONSTRUCT_BINARY(ops::binary::add, " + ")
  STRING_CONSTRUCT_BINARY(ops::binary::subtract, " - ")
  STRING_CONSTRUCT_UNARY(ops::unary::negate, "-")
  //STRING_CONSTRUCT_UNARY(ops::unary::identity, "+")
  template <typename Xop, typename X1, typename X2> \
  string(const expr::unary<ops::unary::identity, expr::binary<Xop, X1, X2> >& expr) \
    : st(to_string("new(") + string(expr.get()).st + ")") {}

  static void assign(string& r, const expr::binary<ops::binary::add, string, string>& expr)
  { r.st = r.st + " = " + expr.x.st + " + " + expr.y.st; }
  static void assign(string& r, const expr::binary<ops::binary::add, string, int>& expr)
  { r.st = r.st + " = " + expr.x.st + " + " + to_string(expr.y); }
  template <typename Op, typename X, typename Y>
  static void assign(string& r, const expr::binary<ops::binary::add, string, expr::binary<Op, X, Y> >& expr)
  { r.st = r.st + " = " + expr.x.st + " + " + string(expr.y).st; }

};

std::ostream& operator<<(std::ostream& os, const string& n)
{
  return os << n.get();
}

//}}}

