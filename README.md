# Kanooth Numbers

A portable multiple-precision library in C++.

## Installation

The library is header-only, so no actual installation is needed. Just make sure the `kanooth` directory is in your include path.

## Usage

### `natural_number`

The type `natural_number` represents a multiple-precision non-negative integer. It is made available by 
```C++
#include <kanooth/numbers/natural_number.hpp>
using kanooth::numbers::natural_number;
```

A natural number can be constructed in a couple of ways:
```C++
natural_number a = 12u;
natural_number b(123456789lu);
natural_number c = "1856066132809047973900594119949";
natural_number d("103435020496695927794013500393");
natural_number q, r;    // initialized to zero
```
Note that only unsigned integer literals are compatible with `natural_number`.

Arithmetic operations are executed by statements such as
```C++
r.add(a, b);
```
meaning that the sum of `a` and `b` will be put in `r`. An unsigned integer can also be used as the second parameter,
```C++
r.subtract(r, 12u);
```
Note that for `subtract` the result is undefined if the first parameter is smaller than the first.

Any `natural_number` can be output using the `str()` method, which will return a `std::string`:
```C++
std::cout << "r = " << r << std::endl;
```

Other basic arithmetic operations available are examplified here:
```C++
r.multiply(r, r);
std::cout << "r^2 = " << r.str() << std::endl;

u.divide(b, a);
v.modulus(b, a);
std::cout << b.str() << " = " << u.str() << " * " << a.str() << " + " << v.str() << std::endl;
```

A special static method `natnum::quotrem` can combine the `divide` and `modulus` operations:
```C++
natural_number::quotrem(q, r, b, a);
std::cout << b.str() << " = " << q.str() << " * " << a.str() << " + " << r.str() << std::endl;
```

A natural number can be checked for zero-ness using `is_zero`:
```C++
u.subtract(u, u);
std::cout << (u.is_zero() ? "u is zero" : "u is nonzero") << std::endl;
```
and compared to another natural number using `compare`:
```C++
std::cout << "q is " << (q.compare(r) > 0 ? "larger than" : "smaller than or equal to") << " r" << std::endl;
```

