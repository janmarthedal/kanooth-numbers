# Kanooth Numbers

A portable multiple-precision library in C++.

## Installation

The library is header-only, so no actual installation is needed. Just make sure the `kanooth` directory is in your include path.

## Usage

### `natural_number`

The type `natural_number` represents a multiple-precision non-negative integer. It is made available by 
```C++
#include <kanooth/numbers/natural_number.hpp>
typedef kanooth::numbers::natural_number<> natnum;
```

Here, we also made a `typedef`, which we will use in the following. A natural number can be constructed in a couple of ways:
```C++
natnum a = 1u;
natnum b(123456789lu);
natnum c = "1856066132809047973900594119949";
```

Note that only unsigned integer literals are compatible with `natural_number`.

Arithmetic operations are executed by statements such as
```C++
a.add(b, c);
```
meaning that the sum of `b` and `c` will be put in `a`. An unsigned integer can also be used as the second parameter,
```C++
b.subtract(b, 1000u);
```
Note that for `subtract` the result is undefined if the first parameter is smaller than the first.
