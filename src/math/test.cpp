#include <iostream>
//#include <math/numbers/gmp/integer.hpp>
//#include <math/numbers/shared.hpp>
#include <math/numbers/number.hpp>
//#include <math/numbers/detail/digit_array.hpp>
//#include <math/numbers/natural_number.hpp>
#include <sstream>

template <class T>
inline std::string to_string(const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

//namespace sputsoft { namespace math { namespace numbers {

class string {
private:
  std::string st;
public:
  string() : st("<>") {}
  string(const std::string _st) : st("<" + _st + ">") {}
  string(const char* ch) : st("<" + std::string(ch) + ">") {}
  string(const expr::binary<ops::binary::add, string, string>& expr)
    : st("new(" + expr.x.st + " + " + expr.y.st + ")") {}
  string(const expr::binary<ops::binary::add, string, int>& expr)
    : st("new(" + expr.x.st + " + " + to_string(expr.y) + ")") {}
  template <typename Op, typename X, typename Y>
  string(const expr::binary<ops::binary::add, string, expr::binary<Op, X, Y> >& expr)
    : st("new(" + expr.x.st + " + " + string(expr.y).st + ")") {}
  const std::string& get() const { return st; }

  void assign(string& r, const expr::binary<ops::binary::add, string, string>& expr)
  { r.st = r.st + " = " + expr.x.st + " + " + expr.y.st; }
  void assign(string& r, const expr::binary<ops::binary::add, string, int>& expr)
  { r.st = r.st + " = " + expr.x.st + " + " + to_string(expr.y); }

};

std::ostream& operator<<(std::ostream& os, const string& n)
{
  return os << n.get();
}

//}}}


//typedef sputsoft::math::numbers::number<sputsoft::math::numbers::gmp::integer> num;
/*typedef sputsoft::math::numbers::number<
  sputsoft::math::numbers::shared<
    sputsoft::math::numbers::gmp::integer> > num;*/
/*typedef sputsoft::math::numbers::number<
  sputsoft::math::numbers::natural_number<
    sputsoft::math::numbers::detail::digit_array<unsigned>
  > > num;*/
//typedef sputsoft::math::numbers::number<sputsoft::math::numbers::string> num;
typedef number<string> num;

template <typename T>
T test(T x)
{
  T y = x;
  return y;
}

/*void run1() 
{
  num n = 1, m = 2;
  num r = test(n + m);
  num s = r;
  std::cout << r << std::endl;
  //std::cout << test<num>(n + m) << std::endl;
}

void run2()
{
  num n = 1;

  std::cout << n << std::endl;
}*/

int main()
{
  num n = "x", m = "y";
  //num r = test(n + 2);
  num s = n + (m + n);
  std::cout << s << std::endl;

  return 0;
}

