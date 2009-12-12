#ifndef GMP_INTEGER_HPP
#define GMP_INTEGER_HPP

#include <math/numbers/number.hpp>
#include <gmp.h>

namespace sputsoft {
namespace math {
namespace numbers {
namespace gmp {

class integer {
private:
  mpz_t v;
public:
  integer() { mpz_init(v);  }
  integer(const unsigned long int x) { mpz_init_set_ui(v, x); }
  integer(const int x) { mpz_init_set_ui(v, x); }
  integer(const integer& y) { mpz_init_set(v, y.v); }
  template <typename Expr>
  integer(const Expr& expr) {
    mpz_init(v);
    assign(*this, expr);
  }
  ~integer() { mpz_clear(v); }
  mpz_srcptr get() const { return v; }
  mpz_ptr get() { return v; }

  static void assign(integer& r, const binary_expr<integer, integer, ops::binary_addition>& expr)
  {
    mpz_add(r.get(), expr.x.get(), expr.y.get());
  }

};

template <typename Os>
Os& operator<<(Os& os, const integer& n)
{
  mpz_srcptr v = n.get();
  char st[mpz_sizeinbase(v, 10) + 2];
  mpz_get_str(st, 10, v);
  return os << st;
}

} // namespace gmp
} // namespace numbers
} // namespace math
} // namespace sputsoft

#endif // GMP_INTEGER_HPP

