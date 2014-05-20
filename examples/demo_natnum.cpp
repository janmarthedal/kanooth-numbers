/*
 * Author: Jan Marthedal Rasmussen
 * Created 2014-05-08
 */

#include <iostream>
#include <kanooth/numbers/natural_number.hpp>
#include "demo_common.hpp"

using kanooth::numbers::natural_number;

natural_number factorial(unsigned n)
{
    natural_number result = 1u;
    for (unsigned k=2; k <= n; k++) {
        result.multiply(result, k);
    }
    return result;
}

natural_number fibonacci(unsigned n)
{
    natural_number a = 0u;
    natural_number b = 1u;
    while (n--) {
        a.add(a, b);
        a.swap(b);
    }
    return a;
}

natural_number power_of_2(unsigned n) {
    natural_number r = 1u;
    r.left_shift(r, n);
    return r;
}

int main()
{
    natural_number a = 12u;
    natural_number b(123456789lu);
    natural_number c = "1856066132809047973900594119949";
    natural_number d("103435020496695927794013500393");
    natural_number q, r, u, v;    // initialized to zero

    r.add(a, b);
    r.subtract(r, 12u);
    std::cout << "r = " << r.str() << std::endl;

    r.multiply(r, r);
    std::cout << "r^2 = " << r.str() << std::endl;

    u.divide(b, a);
    v.modulus(b, a);
    std::cout << b.str() << " = " << u.str() << " * " << a.str() << " + " << v.str() << std::endl;

    natural_number::quotrem(q, r, b, a);
    std::cout << b.str() << " = " << q.str() << " * " << a.str() << " + " << r.str() << std::endl;

    u.subtract(u, u);
    std::cout << "u = " << u.str() << " is " << (u.is_zero() ? "zero" : "nonzero") << std::endl;

    std::cout << "q = " << q.str() << " " << (q.compare(r) > 0 ? ">" : "<=")
              << " r = " << r.str() << std::endl;

    std::cout << "100! = " << factorial(100u).str() << std::endl;
    std::cout << "gcd(" << c.str() << "," << d.str() << ") = " << gcd(c, d).str() << std::endl;
    std::cout << "200th fibonacci number = " << fibonacci(200u).str() << std::endl;
    std::cout << "3^100 = " << power(natural_number(1u), natural_number(3u), 100).str() << std::endl;
    std::cout << "2^120 = " << power_of_2(120).str() << std::endl;

    return 0;
}
