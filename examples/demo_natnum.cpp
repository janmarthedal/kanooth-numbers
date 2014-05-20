/*
 * Author: Jan Marthedal Rasmussen
 * Created 2014-05-08
 */

#include <iostream>
#include <kanooth/numbers/natural_number.hpp>

typedef kanooth::numbers::natural_number<> natnum;

natnum factorial(unsigned n)
{
    natnum result = 1u;
    for (unsigned k=2; k <= n; k++) {
        result.multiply(result, k);
    }
    return result;
}

natnum fibonacci(unsigned n)
{
    natnum a = 0u;
    natnum b = 1u;
    while (n--) {
        a.add(a, b);
        a.swap(b);
    }
    return a;
}

natnum gcd(const natnum& a, const natnum& b)
{
    natnum r;
    r.gcd(a, b);
    return r;
}

natnum power(natnum y, natnum x, unsigned n) {
  if (n) {
    while (n > 1) {
      if (n & 1) y.multiply(y, x);
      x.multiply(x, x);
      n >>= 1;
    }
    y.multiply(y, x);
  }
  return y;
}

natnum power_of_2(unsigned n) {
    natnum r = 1u;
    r.left_shift(r, n);
    return r;
}

int main()
{
    natnum a = 12u;
    natnum b(123456789lu);
    natnum c = "1856066132809047973900594119949";
    natnum d("103435020496695927794013500393");
    natnum q, r, u, v;    // initialized to zero

    r.add(a, b);
    r.subtract(r, 12u);
    std::cout << "r = " << r.str() << std::endl;

    r.multiply(r, r);
    std::cout << "r^2 = " << r.str() << std::endl;

    u.divide(b, a);
    v.modulus(b, a);
    std::cout << b.str() << " = " << u.str() << " * " << a.str() << " + " << v.str() << std::endl;

    natnum::quotrem(q, r, b, a);
    std::cout << b.str() << " = " << q.str() << " * " << a.str() << " + " << r.str() << std::endl;

    u.subtract(u, u);
    std::cout << (u.is_zero() ? "u is zero" : "u is nonzero") << std::endl;

    std::cout << "q is " << (q.compare(r) > 0 ? "larger than" : "smaller than or equal to") << " r" << std::endl;

    std::cout << "100! = " << factorial(100u).str() << std::endl;
    std::cout << "gcd(" << c.str() << "," << d.str() << ") = " << gcd(c, d).str() << std::endl;
    std::cout << "200th fibonacci number = " << fibonacci(200u).str() << std::endl;
    std::cout << "3^100 = " << power(1u, 3u, 100).str() << std::endl;
    std::cout << "2^120 = " << power_of_2(120).str() << std::endl;

    return 0;
}

