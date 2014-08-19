/*
 * Author: Jan Marthedal Rasmussen
 * Created 2014-05-08
 */

#include <iostream>
#include <kanooth/numbers/boost_integer.hpp>

typedef kanooth::numbers::boost_integer integer;

int main()
{
    integer a = 34;
    integer b(123456789l);
    //integer c = "1856066132809047973900594119949";
    //integer d("-103435020496695927794013500393");
    integer q, r, u, v;    // initialized to zero

    r = 12*a + 2*b;
    //r.subtract(r, 12u);
    std::cout << "r = " << r << std::endl;

    /*r.multiply(r, r);
    std::cout << "r^2 = " << r.str() << std::endl;

    u.divide_truncate(b, a);
    v.modulus_truncate(b, a);
    std::cout << b.str() << " = " << u.str() << " * " << a.str() << " + " << v.str() << std::endl;

    integer::quotrem(q, r, b, a);
    std::cout << b.str() << " = " << q.str() << " * " << a.str() << " + " << r.str() << std::endl;

    u.subtract(u, u);
    std::cout << "u = " << u.str() << " is " << (u.is_zero() ? "zero" : "nonzero") << std::endl;

    std::cout << "q = " << q.str() << " " << (q.compare(r) > 0 ? ">" : "<=")
              << " r = " << r.str() << std::endl;

    std::cout << "gcd(" << c.str() << "," << d.str() << ") = " << gcd(c, d).str() << std::endl;
    std::cout << "(-3)^99 = " << power(integer(1), integer(-3), 99).str() << std::endl;*/

    return 0;
}
