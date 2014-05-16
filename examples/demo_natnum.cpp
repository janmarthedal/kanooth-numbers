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

int main()
{
    std::cout << "100! = " << factorial(100u).str() << std::endl;
    
    natnum a = "1856066132809047973900594119949", b = "103435020496695927794013500393", c;
    c.gcd(a, b);
    std::cout << "gcd(" << a.str() << "," << b.str() << ") = " << c.str() << std::endl;

    std::cout << "200th fibonacci number = " << fibonacci(200u).str() << std::endl;

    std::cout << __cplusplus << std::endl;

    return 0;
}

